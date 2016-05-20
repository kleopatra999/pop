#ifndef POP_CODEGEN_HPP
#define POP_CODEGEN_HPP

#include <pop/ast.hpp>
#include <pop/opcodes.hpp>
#include <cassert>
#include <stack>
#include <vector>

namespace Pop
{
using namespace Ast;

template <class T, class... Args>
static inline OpPtr mkop(Args &&... args)
{
	return std::make_unique<Operation>(std::forward<Args>(args)...);
}

struct CodeGenVisitor : public Visitor
{
	OpList decl_ops;
	OpList code_ops;
	std::vector<unsigned int> depth_stack;
	std::stack<OpList *> ops_stack;
	std::stack<std::string> control_stack;

	CodeGenVisitor()
	{
		depth_stack.emplace_back(0);
		begin_code();
	}

	OpList finish()
	{
		OpList combined;
		combined.emplace_back(new Jump("_pop_start_"));
		for (auto &op : decl_ops)
			combined.emplace_back(op.release());
		combined.emplace_back(new Label("_pop_start_"));
		combined.emplace_back(new OpenScope());
		for (auto &op : code_ops)
			combined.emplace_back(op.release());
		combined.emplace_back(new CloseScope());
		combined.emplace_back(new Halt());
		return combined;
	}

	template <class T, class... Args>
	void add_op(Args &&... args)
	{
		ops_stack.top()->emplace_back(new T(std::forward<Args>(args)...));
	}

	void enter()
	{
		depth_stack.emplace_back(0);
	}

	void leave()
	{
		depth_stack.pop_back();
	}

	void begin_decls()
	{
		ops_stack.push(&decl_ops);
	}

	void end_decls()
	{
		assert(ops_stack.top() == &decl_ops);
		ops_stack.pop();
	}

	void begin_code()
	{
		ops_stack.push(&code_ops);
	}

	void end_code()
	{
		assert(ops_stack.top() == &code_ops);
		ops_stack.pop();
	}

	std::string auto_name()
	{
		std::string name("_pop_");
		for (auto cnt : depth_stack)
			name += std::to_string(cnt) + "_";
		auto &cnt = depth_stack.back();
		cnt++;
		return name;
	}

	virtual void visit(Module &n)
	{
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
	}

	virtual void visit(NullLiteral &)
	{
		add_op<PushNull>();
	}

	virtual void visit(BoolLiteral &n)
	{
		if (n.value)
			add_op<PushTrue>();
		else
			add_op<PushFalse>();
	}

	virtual void visit(IntLiteral &n)
	{
		add_op<PushInt>(n.value);
	}

	virtual void visit(FloatLiteral &n)
	{
		add_op<PushFloat>(n.value);
	}

	virtual void visit(StringLiteral &n)
	{
		add_op<PushString>(n.value);
	}

	virtual void visit(Identifier &n)
	{
		add_op<PushSymbol>(n.name);
	}

	virtual void visit(ListLiteral &n)
	{
		add_op<PushList>(n.elements.size());
	}

	virtual void visit(FunctionLiteral &n)
	{
		auto name = auto_name();
		// the function definition code
		enter();
		begin_decls();
		add_op<Label>(name);
		add_op<OpenScope>();
		add_op<PopTop>(); // argument count, not used yet
		for (auto &argument : n.arguments)
			add_op<Bind>(argument);
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		add_op<CloseScope>();
		end_decls();
		leave();
		// the function expression code
		// add_op<Jump>(name);
		add_op<PushFunction>(name);
	}

	virtual void visit(ObjectLiteral &)
	{
	}

	virtual void visit(UnaryExpr &n)
	{
		n.operand->accept(*this);
		add_op<UnOp>(opcode_from_token(n.op));
	}

	virtual void visit(BinaryExpr &n)
	{
		n.left->accept(*this);
		n.right->accept(*this);
		add_op<BinOp>(opcode_from_token(n.op));
	}

	virtual void visit(SliceExpr &n)
	{
		if (n.start)
			n.start->accept(*this);
		else
			add_op<PushNull>();
		if (n.stop)
			n.stop->accept(*this);
		else
			add_op<PushNull>();
		if (n.step)
			n.step->accept(*this);
		else
			add_op<PushNull>();
		add_op<PushSlice>();
	}

	virtual void visit(IndexExpr &n)
	{
		n.object->accept(*this);
		n.index->accept(*this);
		add_op<Index>();
	}

	virtual void visit(MemberExpr &n)
	{
		n.object->accept(*this);
		n.member->accept(*this);
	}

	virtual void visit(CallExpr &n)
	{
		// push args in reverse order
		for (auto it = n.arguments.rbegin(); it != n.arguments.rend(); ++it)
		{
			auto &arg = *it;
			arg->accept(*this);
		}
		// push arg count
		add_op<PushInt>(n.arguments.size());
		n.callee->accept(*this);
		add_op<Call>();
	}

	virtual void visit(IfExpr &n)
	{
		auto name = auto_name();
		n.predicate->accept(*this);
		add_op<JumpFalse>(name + "else_");
		n.consequence->accept(*this);
		add_op<Jump>(name + "endif_");
		add_op<Label>(name + "else_");
		n.alternative->accept(*this);
		add_op<Label>(name + "endif_");
	}

	virtual void visit(ForExpr &)
	{
	}

	virtual void visit(LetBinding &n)
	{
		if (n.value)
			n.value->accept(*this);
		else
			add_op<PushNull>();
		add_op<Bind>(n.name);
	}

	virtual void visit(LabelDecl &n)
	{
		add_op<Label>("_pop_label_" + n.name);
	}

	virtual void visit(EmptyStmt &)
	{
		// don't emit anything
	}

	virtual void visit(ExprStmt &n)
	{
		n.expr->accept(*this);
	}

	virtual void visit(CompoundStmt &n)
	{
		enter();
		add_op<OpenScope>();
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		add_op<CloseScope>();
		leave();
	}

	virtual void visit(BreakStmt &)
	{
		add_op<Jump>(control_stack.top() + "end_");
	}

	virtual void visit(ContinueStmt &)
	{
		add_op<Jump>(control_stack.top() + "begin_");
	}

	virtual void visit(GotoStmt &n)
	{
		add_op<Jump>("_pop_label_" + n.label);
	}

	virtual void visit(ReturnStmt &n)
	{
		if (n.expr)
			n.expr->accept(*this);
		else
			add_op<PushNull>();
		add_op<Return>();
	}

	virtual void visit(IfStmt &n)
	{
		auto name = auto_name();
		n.predicate->accept(*this);
		add_op<JumpFalse>(name + "else_");
		n.consequence->accept(*this);
		add_op<Jump>(name + "endif_");
		add_op<Label>(name + "else_");
		if (n.alternative)
			n.alternative->accept(*this);
		add_op<Label>(name + "endif_");
	}

	virtual void visit(UnlessStmt &n)
	{
		auto name = auto_name();
		n.predicate->accept(*this);
		add_op<JumpTrue>(name + "else_");
		n.consequence->accept(*this);
		add_op<Jump>(name + "endif_");
		add_op<Label>(name + "else_");
		if (n.alternative)
			n.alternative->accept(*this);
		add_op<Label>(name + "endif_");
	}

	virtual void visit(DoWhileStmt &n)
	{
		auto name = auto_name();
		add_op<Label>(name + "begin_");
		control_stack.push(name);
		n.stmt->accept(*this);
		control_stack.pop();
		n.expr->accept(*this);
		add_op<JumpTrue>(name + "begin_");
		add_op<Label>(name + "end_");
	}

	virtual void visit(DoUntilStmt &n)
	{
		auto name = auto_name();
		add_op<Label>(name + "begin_");
		control_stack.push(name);
		n.stmt->accept(*this);
		control_stack.pop();
		n.expr->accept(*this);
		add_op<JumpFalse>(name + "begin_");
		add_op<Label>(name + "end_");
	}

	virtual void visit(WhileStmt &n)
	{
		auto name = auto_name();
		add_op<Label>(name + "begin_");
		n.expr->accept(*this);
		control_stack.push(name);
		add_op<JumpTrue>(name + "end_");
		n.stmt->accept(*this);
		add_op<Jump>(name + "begin_");
		control_stack.pop();
		add_op<Label>(name + "end_");
	}

	virtual void visit(UntilStmt &n)
	{
		auto name = auto_name();
		add_op<Label>(name + "begin_");
		n.expr->accept(*this);
		control_stack.push(name);
		add_op<JumpFalse>(name + "end_");
		n.stmt->accept(*this);
		add_op<Jump>(name + "begin_");
		control_stack.pop();
		add_op<Label>(name + "end_");
	}

	virtual void visit(ForStmt &)
	{
	}
};

// namespace Pop
}

#endif // POP_CODEGEN_HPP
