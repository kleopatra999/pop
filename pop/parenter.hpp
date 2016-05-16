#ifndef POP_PARENTER_HPP
#define POP_PARENTER_HPP

#include <pop/ast.hpp>
#include <cassert>
#include <stack>

namespace Pop
{

namespace Ast
{

struct ParentSetVisitor : public Visitor
{
	std::stack<Node *> parent_stack;

	ParentSetVisitor()
	{
		parent_stack.push(nullptr);
	}

	~ParentSetVisitor()
	{
		try
		{
			assert(parent_stack.top() == nullptr);
			parent_stack.pop();
		}
		catch (...)
		{
		}
	}

	void begin(Node &n)
	{
		parent_stack.push(&n);
	}

	Node *end(Node &n)
	{
		Node *t = parent_stack.top();
		assert(t == &n);
		parent_stack.pop();
		return t;
	}

	Node *top() const
	{
		assert(!parent_stack.empty());
		return parent_stack.top();
	}

	virtual void visit(Module &n)
	{
		n.parent = top();
		begin(n);
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		end(n);
	}

	virtual void visit(NullLiteral &n)
	{
		n.parent = top();
	}

	virtual void visit(BoolLiteral &n)
	{
		n.parent = top();
	}

	virtual void visit(IntLiteral &n)
	{
		n.parent = top();
	}

	virtual void visit(FloatLiteral &n)
	{
		n.parent = top();
	}

	virtual void visit(StringLiteral &n)
	{
		n.parent = top();
	}

	virtual void visit(Identifier &n)
	{
		n.parent = top();
	}

	virtual void visit(ListLiteral &n)
	{
		n.parent = top();
		begin(n);
		for (auto &elem : n.elements)
			elem->accept(*this);
		end(n);
	}

	virtual void visit(FunctionLiteral &n)
	{
		n.parent = top();
		begin(n);
		for (auto &arg : n.default_arguments)
			arg->accept(*this);
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		end(n);
	}

	virtual void visit(ObjectLiteral &n)
	{
		n.parent = top();
		begin(n);
		for (auto &memb : n.member_values)
			memb->accept(*this);
		end(n);
	}

	virtual void visit(UnaryExpr &n)
	{
		n.parent = top();
		begin(n);
		n.operand->accept(*this);
		end(n);
	}

	virtual void visit(BinaryExpr &n)
	{
		n.parent = top();
		begin(n);
		n.left->accept(*this);
		n.right->accept(*this);
		end(n);
	}

	virtual void visit(SliceExpr &n)
	{
		n.parent = top();
		begin(n);
		n.start->accept(*this);
		n.stop->accept(*this);
		n.step->accept(*this);
		end(n);
	}

	virtual void visit(IndexExpr &n)
	{
		n.parent = top();
		begin(n);
		n.object->accept(*this);
		n.index->accept(*this);
		end(n);
	}

	virtual void visit(MemberExpr &n)
	{
		n.parent = top();
		begin(n);
		n.object->accept(*this);
		n.member->accept(*this);
		end(n);
	}

	virtual void visit(CallExpr &n)
	{
		n.parent = top();
		begin(n);
		n.callee->accept(*this);
		for (auto &arg : n.arguments)
			arg->accept(*this);
		end(n);
	}

	virtual void visit(IfExpr &n)
	{
		n.parent = top();
		begin(n);
		n.predicate->accept(*this);
		n.consequence->accept(*this);
		n.alternative->accept(*this);
		end(n);
	}

	virtual void visit(ForExpr &n)
	{
		n.parent = top();
		begin(n);
		n.value->accept(*this);
		n.iterator->accept(*this);
		n.sequence->accept(*this);
		end(n);
	}

	virtual void visit(LetBinding &n)
	{
		n.parent = top();
		begin(n);
		n.value->accept(*this);
		end(n);
	}

	virtual void visit(LabelDecl &n)
	{
		n.parent = top();
	}

	virtual void visit(EmptyStmt &n)
	{
		n.parent = top();
	}

	virtual void visit(ExprStmt &n)
	{
		n.parent = top();
		begin(n);
		n.expr->accept(*this);
		end(n);
	}

	virtual void visit(CompoundStmt &n)
	{
		n.parent = top();
		begin(n);
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		end(n);
	}

	virtual void visit(BreakStmt &n)
	{
		n.parent = top();
	}

	virtual void visit(ContinueStmt &n)
	{
		n.parent = top();
	}

	virtual void visit(GotoStmt &n)
	{
		n.parent = top();
	}

	virtual void visit(ReturnStmt &n)
	{
		n.parent = top();
		if (n.expr)
		{
			begin(n);
			n.expr->accept(*this);
			end(n);
		}
	}

	virtual void visit(IfStmt &n)
	{
		n.parent = top();
		begin(n);
		n.predicate->accept(*this);
		n.consequence->accept(*this);
		if (n.alternative)
			n.alternative->accept(*this);
		end(n);
	}

	virtual void visit(UnlessStmt &n)
	{
		n.parent = top();
		begin(n);
		n.predicate->accept(*this);
		n.consequence->accept(*this);
		if (n.alternative)
			n.alternative->accept(*this);
		end(n);
	}

	virtual void visit(DoWhileStmt &n)
	{
		n.parent = top();
		begin(n);
		n.stmt->accept(*this);
		n.expr->accept(*this);
		end(n);
	}

	virtual void visit(DoUntilStmt &n)
	{
		n.parent = top();
		begin(n);
		n.stmt->accept(*this);
		n.expr->accept(*this);
		end(n);
	}

	virtual void visit(WhileStmt &n)
	{
		n.parent = top();
		begin(n);
		n.expr->accept(*this);
		n.stmt->accept(*this);
		end(n);
	}

	virtual void visit(UntilStmt &n)
	{
		n.parent = top();
		begin(n);
		n.expr->accept(*this);
		n.stmt->accept(*this);
		end(n);
	}

	virtual void visit(ForStmt &n)
	{
		n.parent = top();
		begin(n);
		n.iterator->accept(*this);
		n.sequence->accept(*this);
		end(n);
	}
};

// namespace Ast
}

// namespace Pop
}

#endif // POP_PARENTER_HPP
