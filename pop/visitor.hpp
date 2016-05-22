// visitor.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_VISITOR_HPP
#define POP_VISITOR_HPP

namespace Pop
{

namespace Ast
{

struct Module;
struct NullLiteral;
struct BoolLiteral;
struct IntLiteral;
struct FloatLiteral;
struct StringLiteral;
struct Identifier;
struct ListLiteral;
struct FunctionLiteral;
struct ObjectLiteral;
struct UnaryExpr;
struct BinaryExpr;
struct SliceExpr;
struct IndexExpr;
struct MemberExpr;
struct CallExpr;
struct IfExpr;
struct ForExpr;
struct LetBinding;
struct LabelDecl;
struct EmptyStmt;
struct ExprStmt;
struct CompoundStmt;
struct BreakStmt;
struct ContinueStmt;
struct GotoStmt;
struct ReturnStmt;
struct IfStmt;
struct UnlessStmt;
struct DoWhileStmt;
struct DoUntilStmt;
struct WhileStmt;
struct UntilStmt;
struct ForStmt;

// clang-format off
struct Visitor
{
	virtual void visit(Module&) {}
	virtual void visit(NullLiteral&) {}
	virtual void visit(BoolLiteral&) {}
	virtual void visit(IntLiteral&) {}
	virtual void visit(FloatLiteral&) {}
	virtual void visit(StringLiteral&) {}
	virtual void visit(Identifier&) {}
	virtual void visit(ListLiteral&) {}
	virtual void visit(FunctionLiteral&) {}
	virtual void visit(ObjectLiteral&) {}
	virtual void visit(UnaryExpr&) {}
	virtual void visit(BinaryExpr&) {}
	virtual void visit(SliceExpr&) {}
	virtual void visit(IndexExpr&) {}
	virtual void visit(MemberExpr&) {}
	virtual void visit(CallExpr&) {}
	virtual void visit(IfExpr&) {}
	virtual void visit(ForExpr&) {}
	virtual void visit(LetBinding&) {}
	virtual void visit(LabelDecl&) {}
	virtual void visit(EmptyStmt&) {}
	virtual void visit(ExprStmt&) {}
	virtual void visit(CompoundStmt&) {}
	virtual void visit(BreakStmt&) {}
	virtual void visit(ContinueStmt&) {}
	virtual void visit(GotoStmt&) {}
	virtual void visit(ReturnStmt&) {}
	virtual void visit(IfStmt&) {}
	virtual void visit(UnlessStmt&) {}
	virtual void visit(DoWhileStmt&) {}
	virtual void visit(DoUntilStmt&) {}
	virtual void visit(WhileStmt&) {}
	virtual void visit(UntilStmt&) {}
	virtual void visit(ForStmt&) {}
};
// clang-format on

// namespace Ast
}

// namespace Pop
}

#endif // POP_VISITOR_HPP
