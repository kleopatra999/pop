// debugvisitor.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_PARENTER_HPP
#define POP_PARENTER_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <pop/ast.hpp>
#include <cassert>
#include <ostream>

namespace Pop
{

namespace Ast
{

struct DebugVisitor : public Visitor
{
	std::ostream &out;
	int ind_level;
	int ind_width;

	DebugVisitor(std::ostream &out, int ind_width = 2)
	    : out(out), ind_level(0), ind_width(ind_width)
	{
	}

	std::string indent_str() const
	{
		return std::string(ind_level * ind_width, ' ');
	}

	void indent()
	{
		ind_level++;
	}

	void unindent()
	{
		ind_level--;
	}

	virtual void visit(Module &n)
	{
		out << indent_str() << "<Module filename='" << n.filename << "'>\n";
		indent();
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		unindent();
		out << indent_str() << "</Module>\n";
	}

	virtual void visit(NullLiteral &)
	{
		out << indent_str() << "<NullLiteral/>\n";
	}

	virtual void visit(BoolLiteral &n)
	{
		out << indent_str() << "<BoolLiteral value='" << n.value << "'/>\n";
	}

	virtual void visit(IntLiteral &n)
	{
		out << indent_str() << "<IntLiteral value='" << n.value << "'/>\n";
	}

	virtual void visit(FloatLiteral &n)
	{
		out << indent_str() << "<FloatLiteral value='" << n.value << "'/>\n";
	}

	virtual void visit(StringLiteral &n)
	{
		out << indent_str() << "<StringLiteral value='" << n.value << "'/>\n";
	}

	virtual void visit(Identifier &n)
	{
		out << indent_str() << "<Identifier name='" << n.name << "'/>\n";
	}

	virtual void visit(ListLiteral &n)
	{
		out << indent_str() << "<ListLiteral>\n";
		indent();
		for (auto &elem : n.elements)
			elem->accept(*this);
		unindent();
		out << indent_str() << "</ListLiteral>\n";
	}

	virtual void visit(FunctionLiteral &n)
	{
		out << indent_str() << "<FunctionLiteral>\n";
		indent();
		out << indent_str() << "<Arguments>\n";
		indent();
		for (size_t i = 0; i < n.arguments.size(); i++)
		{
			out << indent_str() << "<Argument name='" << n.arguments[i] << "'";
			if (i < n.default_arguments.size())
			{
				out << ">\n";
				n.default_arguments[i]->accept(*this);
				out << indent_str() << "</Argument>\n";
			}
			else
				out << "/>\n";
		}
		unindent();
		out << indent_str() << "</Arguments>\n";
		out << indent_str() << "<Body>\n";
		indent();
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		unindent();
		out << indent_str() << "</Body>\n";
		unindent();
		out << indent_str() << "</FunctionLiteral>\n";
	}

	virtual void visit(ObjectLiteral &n)
	{
		out << indent_str() << "<ObjectLiteral>\n";
		indent();
		for (size_t i = 0; i < n.member_names.size(); i++)
		{
			out << indent_str() << "<Member name='" << n.member_names[i]
			    << "'>\n";
			indent();
			n.member_values[i]->accept(*this);
			unindent();
			out << indent_str() << "</Member>\n";
		}
		unindent();
		out << indent_str() << "</ObjectLiteral>\n";
	}

	virtual void visit(UnaryExpr &n)
	{
		out << indent_str() << "<UnaryExpr op='" << token_kind_name(n.op)
		    << "'>\n";
		indent();
		n.operand->accept(*this);
		unindent();
		out << indent_str() << "</UnaryExpr>\n";
	}

	virtual void visit(BinaryExpr &n)
	{
		out << indent_str() << "<BinaryExpr op='" << token_kind_name(n.op)
		    << "'>\n";
		indent();
		n.left->accept(*this);
		n.right->accept(*this);
		unindent();
		out << indent_str() << "</BinaryExpr>\n";
	}

	virtual void visit(SliceExpr &n)
	{
		out << indent_str() << "<SliceExpr>\n";
		indent();
		n.start->accept(*this);
		n.stop->accept(*this);
		n.step->accept(*this);
		unindent();
		out << indent_str() << "</SliceExpr>\n";
	}

	virtual void visit(IndexExpr &n)
	{
		out << indent_str() << "<IndexExpr>\n";
		indent();
		n.object->accept(*this);
		n.index->accept(*this);
		unindent();
		out << indent_str() << "</IndexExpr>\n";
	}

	virtual void visit(MemberExpr &n)
	{
		out << indent_str() << "<MemberExpr>\n";
		indent();
		n.object->accept(*this);
		n.member->accept(*this);
		unindent();
		out << indent_str() << "</MemberExpr>\n";
	}

	virtual void visit(CallExpr &n)
	{
		out << indent_str() << "<CallExpr>\n";
		indent();
		n.callee->accept(*this);
		out << indent_str() << "<Arguments>\n";
		indent();
		for (auto &arg : n.arguments)
			arg->accept(*this);
		unindent();
		out << indent_str() << "</Arguments>\n";
		unindent();
		out << indent_str() << "</CallExpr>\n";
	}

	virtual void visit(IfExpr &n)
	{
		out << indent_str() << "<IfExpr>\n";
		indent();
		n.predicate->accept(*this);
		n.consequence->accept(*this);
		n.alternative->accept(*this);
		unindent();
		out << indent_str() << "</IfExpr>\n";
	}

	virtual void visit(ForExpr &n)
	{
		out << indent_str() << "<ForExpr>\n";
		indent();
		n.value->accept(*this);
		n.iterator->accept(*this);
		n.sequence->accept(*this);
		unindent();
		out << indent_str() << "</ForExpr>\n";
	}

	virtual void visit(LetBinding &n)
	{
		out << indent_str() << "<LetBinding name='" << n.name << "'>\n";
		indent();
		n.value->accept(*this);
		unindent();
		out << indent_str() << "</LetBinding>\n";
	}

	virtual void visit(LabelDecl &n)
	{
		out << indent_str() << "<LabelDecl name='" << n.name << "'/>\n";
	}

	virtual void visit(EmptyStmt &)
	{
		out << indent_str() << "<EmptyStmt/>\n";
	}

	virtual void visit(ExprStmt &n)
	{
		out << indent_str() << "<ExprStmt>\n";
		indent();
		n.expr->accept(*this);
		unindent();
		out << indent_str() << "</ExprStmt>\n";
	}

	virtual void visit(CompoundStmt &n)
	{
		out << indent_str() << "<CompoundStmt>\n";
		indent();
		for (auto &stmt : n.stmts)
			stmt->accept(*this);
		unindent();
		out << indent_str() << "</CompoundStmt>\n";
	}

	virtual void visit(BreakStmt &)
	{
		out << indent_str() << "<BreakStmt/>\n";
	}

	virtual void visit(ContinueStmt &)
	{
		out << indent_str() << "<ContinueStmt/>\n";
	}

	virtual void visit(GotoStmt &n)
	{
		out << indent_str() << "<GotoStmt label='" << n.label << "'/>\n";
	}

	virtual void visit(ReturnStmt &n)
	{
		if (n.expr)
		{
			out << indent_str() << "<ReturnStmt>\n";
			indent();
			n.expr->accept(*this);
			unindent();
			out << indent_str() << "</ReturnStmt>\n";
		}
		else
		{
			out << indent_str() << "<ReturnStmt/>\n";
		}
	}

	virtual void visit(IfStmt &n)
	{
		out << indent_str() << "<IfStmt>\n";
		indent();
		n.predicate->accept(*this);
		n.consequence->accept(*this);
		if (n.alternative)
			n.alternative->accept(*this);
		unindent();
		out << indent_str() << "</IfStmt>\n";
	}

	virtual void visit(UnlessStmt &n)
	{
		out << indent_str() << "<UnlessStmt>\n";
		indent();
		n.predicate->accept(*this);
		n.consequence->accept(*this);
		if (n.alternative)
			n.alternative->accept(*this);
		unindent();
		out << indent_str() << "</UnlessStmt>\n";
	}

	virtual void visit(DoWhileStmt &n)
	{
		out << indent_str() << "<DoWhileStmt>\n";
		indent();
		n.stmt->accept(*this);
		n.expr->accept(*this);
		unindent();
		out << indent_str() << "</DoWhileStmt>\n";
	}

	virtual void visit(DoUntilStmt &n)
	{
		out << indent_str() << "<DoUntilStmt>\n";
		indent();
		n.stmt->accept(*this);
		n.expr->accept(*this);
		unindent();
		out << indent_str() << "</DoUntilStmt>\n";
	}

	virtual void visit(WhileStmt &n)
	{
		out << indent_str() << "<WhileStmt>\n";
		indent();
		n.expr->accept(*this);
		n.stmt->accept(*this);
		unindent();
		out << indent_str() << "</WhileStmt>\n";
	}

	virtual void visit(UntilStmt &n)
	{
		out << indent_str() << "<UntilStmt>\n";
		indent();
		n.expr->accept(*this);
		n.stmt->accept(*this);
		unindent();
		out << indent_str() << "</UntilStmt>\n";
	}

	virtual void visit(ForStmt &n)
	{
		out << indent_str() << "<ForStmt>\n";
		indent();
		n.iterator->accept(*this);
		n.sequence->accept(*this);
		unindent();
		out << indent_str() << "</ForStmt>\n";
	}
};

// namespace Ast
}

// namespace Pop
}

#endif // POP_PARENTER_HPP
