// transformer.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_TRANSFORMER_HPP
#define POP_TRANSFORMER_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <pop/ast.hpp>
#include <pop/instructions.hpp>
#include <pop/opcodes.hpp>
#include <cassert>
#include <stack>
#include <vector>

namespace Pop
{
using namespace Ast;

struct Transformer : public Visitor
{
	InstructionList decl_ops;
	InstructionList code_ops;
	std::vector<unsigned int> depth_stack;
	std::stack<InstructionList *> ops_stack;
	std::stack<std::string> control_stack;

	Transformer()
	{
		depth_stack.emplace_back(0);
		begin_code();
	}

	InstructionList finish()
	{
		InstructionList combined;
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
		for (auto it = n.elements.rbegin(); it != n.elements.rend(); ++it)
		{
			auto &elem = *it;
			elem->accept(*this);
		}
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
		for (auto &argument : n.arguments)
			add_op<Bind>(argument);
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		add_op<CloseScope>();
		end_decls();
		leave();
		// the function expression code
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
		// reverse order
		n.right->accept(*this);
		n.left->accept(*this);
		add_op<BinOp>(opcode_from_token(n.op));
	}

	virtual void visit(SliceExpr &n)
	{
		// reverse order
		if (n.step)
			n.step->accept(*this);
		else
			add_op<PushNull>();
		if (n.stop)
			n.stop->accept(*this);
		else
			add_op<PushNull>();
		if (n.start)
			n.start->accept(*this);
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
		// special case for the builtin print function
		if (n.callee->kind == NodeKind::IDENTIFIER &&
		    static_cast<Identifier *>(n.callee.get())->name == "print")
		{
			add_op<Print>();
		}
		else
		{
			n.callee->accept(*this);
			add_op<Call>(n.arguments.size());
		}
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
		add_op<PopTop>();
	}

	virtual void visit(CompoundStmt &n)
	{
		enter();
		// add_op<OpenScope>();
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		// add_op<CloseScope>();
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
		add_op<CloseScope>();
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

inline InstructionList transform(ModulePtr &mod)
{
	Transformer xformer;
	mod->accept(xformer);
	return xformer.finish();
}

// namespace Pop
}

#endif // POP_TRANSFORMER_HPP
