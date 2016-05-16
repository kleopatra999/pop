#ifndef POP_AST_HPP
#define POP_AST_HPP

#include <pop/location.hpp>
#include <pop/token.hpp>
#include <pop/visitor.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Pop
{

namespace Ast
{

#define POP_AST_NODE                \
	virtual void accept(Visitor &v) \
	{                               \
		v.visit(*this);             \
	}

enum class NodeKind
{
	// expressions
	NULL_LITERAL,
	BOOL_LITERAL,
	INT_LITERAL,
	FLOAT_LITERAL,
	STRING_LITERAL,
	IDENTIFIER,
	LIST_LITERAL,
	FUNCTION_LITERAL,
	OBJECT_LITERAL,
	UNARY_EXPR,
	BINARY_EXPR,
	SLICE_EXPR,
	INDEX_EXPR,
	MEMBER_EXPR,
	CALL_EXPR,
	IF_EXPR,
	FOR_EXPR,
	// statements
	LET_BINDING,
	LABEL_DECL,
	EMPTY_STMT,
	EXPR_STMT,
	COMPOUND_STMT,
	BREAK_STMT,
	CONTINUE_STMT,
	GOTO_STMT,
	RETURN_STMT,
	IF_STMT,
	UNLESS_STMT,
	DO_WHILE_STMT,
	DO_UNTIL_STMT,
	WHILE_STMT,
	UNTIL_STMT,
	FOR_STMT,
	// other nodes
	MODULE,
};

//
// Base types
//

struct Node;
typedef std::unique_ptr<Node> NodePtr;

struct Node
{
	NodeKind kind;
	Node *parent;
	SourceRange range;

	virtual ~Node()
	{
	}

	virtual void accept(Visitor &) = 0;

protected:
	Node(NodeKind kind, const SourcePosition &start, const SourcePosition &end)
	    : kind(kind), range(start, end)
	{
	}
};

struct Stmt : public Node
{
protected:
	Stmt(NodeKind kind, const SourcePosition &start, const SourcePosition &end)
	    : Node(kind, start, end)
	{
	}
};

struct Expr : public Node
{
protected:
	Expr(NodeKind kind, const SourcePosition &start, const SourcePosition &end)
	    : Node(kind, start, end)
	{
	}
};

typedef std::unique_ptr<Stmt> StmtPtr;
typedef std::unique_ptr<Expr> ExprPtr;
typedef std::vector<StmtPtr> StmtList;
typedef std::vector<ExprPtr> ExprList;
typedef std::string String;
typedef std::vector<String> StringList;

//
// Top-level module
//

struct Module final : public Node
{
	String filename;
	StmtList stmts;
	Module(const String &fn = "<stream>")
	    : Node(NodeKind::MODULE, SourcePosition{0, 0, 0},
	           SourcePosition{0, 0, 0}),
	      filename(fn)
	{
	}
	Module(StmtList stmts, const String &fn = "<stream>")
	    : Node(NodeKind::MODULE, SourcePosition{0, 0, 0},
	           SourcePosition{0, 0, 0}),
	      filename(fn), stmts(std::move(stmts))
	{
	}
	POP_AST_NODE
};

typedef std::unique_ptr<Module> ModulePtr;

//
// Literal expressions
//

struct NullLiteral final : public Expr
{
	NullLiteral(const SourcePosition &start, const SourcePosition &end)
	    : Expr(NodeKind::NULL_LITERAL, start, end)
	{
	}
	POP_AST_NODE
};

struct BoolLiteral final : public Expr
{
	bool value;
	BoolLiteral(bool value, const SourcePosition &start,
	            const SourcePosition &end)
	    : Expr(NodeKind::BOOL_LITERAL, start, end), value(value)
	{
	}
	POP_AST_NODE
};

struct IntLiteral final : public Expr
{
	std::uint64_t value;
	IntLiteral(std::uint64_t value, const SourcePosition &start,
	           const SourcePosition &end)
	    : Expr(NodeKind::INT_LITERAL, start, end), value(value)
	{
	}
	POP_AST_NODE
};

struct FloatLiteral final : public Expr
{
	double value;
	FloatLiteral(double value, const SourcePosition &start,
	             const SourcePosition &end)
	    : Expr(NodeKind::FLOAT_LITERAL, start, end), value(value)
	{
	}
	POP_AST_NODE
};

struct StringLiteral final : public Expr
{
	String value;
	StringLiteral(const String &value, const SourcePosition &start,
	              const SourcePosition &end)
	    : Expr(NodeKind::STRING_LITERAL, start, end), value(value)
	{
	}
	POP_AST_NODE
};

struct Identifier final : public Expr
{
	String name;
	Identifier(const String &name, const SourcePosition &start,
	           const SourcePosition &end)
	    : Expr(NodeKind::IDENTIFIER, start, end), name(name)
	{
	}
	POP_AST_NODE
};

//
// Compound expressions
//

struct ListLiteral final : public Expr
{
	ExprList elements;
	ListLiteral(const SourcePosition &start, const SourcePosition &end)
	    : Expr(NodeKind::LIST_LITERAL, start, end)
	{
	}
	POP_AST_NODE
};

struct FunctionLiteral final : public Expr
{
	StringList arguments;
	ExprList default_arguments;
	StmtList stmts;
	FunctionLiteral(StringList arguments, StmtList stmts,
	                const SourcePosition &start, const SourcePosition &end)
	    : Expr(NodeKind::FUNCTION_LITERAL, start, end),
	      arguments(std::move(arguments)), stmts(std::move(stmts))
	{
	}
	POP_AST_NODE
};

struct ObjectLiteral final : public Expr
{
	StringList member_names;
	ExprList member_values;
	ObjectLiteral(const SourcePosition &start, const SourcePosition &end)
	    : Expr(NodeKind::OBJECT_LITERAL, start, end)
	{
	}
	POP_AST_NODE
};

struct UnaryExpr final : public Expr
{
	TokenKind op;
	ExprPtr operand;
	UnaryExpr(TokenKind op, ExprPtr operand, const SourcePosition &start,
	          const SourcePosition &end)
	    : Expr(NodeKind::UNARY_EXPR, start, end), op(op),
	      operand(std::move(operand))
	{
	}
	POP_AST_NODE
};

struct BinaryExpr final : public Expr
{
	TokenKind op;
	ExprPtr left;
	ExprPtr right;
	BinaryExpr(TokenKind op, ExprPtr left, ExprPtr right,
	           const SourcePosition &start, const SourcePosition &end)
	    : Expr(NodeKind::BINARY_EXPR, start, end), op(op),
	      left(std::move(left)), right(std::move(right))
	{
	}
	POP_AST_NODE
};

struct SliceExpr final : public Expr
{
	ExprPtr start;
	ExprPtr stop;
	ExprPtr step;
	SliceExpr(ExprPtr start, ExprPtr stop, ExprPtr step,
	          const SourcePosition &start_pos, const SourcePosition &end_pos)
	    : Expr(NodeKind::SLICE_EXPR, start_pos, end_pos),
	      start(std::move(start)), stop(std::move(stop)), step(std::move(step))
	{
	}
	POP_AST_NODE
};

struct IndexExpr final : public Expr
{
	ExprPtr object;
	ExprPtr index;
	IndexExpr(ExprPtr object, ExprPtr index, const SourcePosition &start,
	          const SourcePosition &end)
	    : Expr(NodeKind::INDEX_EXPR, start, end), object(std::move(object)),
	      index(std::move(index))
	{
	}
	POP_AST_NODE
};

struct MemberExpr final : public Expr
{
	ExprPtr object;
	ExprPtr member;
	MemberExpr(ExprPtr object, ExprPtr member, const SourcePosition &start,
	           const SourcePosition &end)
	    : Expr(NodeKind::MEMBER_EXPR, start, end), object(std::move(object)),
	      member(std::move(member))
	{
	}
	POP_AST_NODE
};

struct CallExpr final : public Expr
{
	ExprPtr callee;
	ExprList arguments;
	CallExpr(ExprPtr callee, ExprList arguments, const SourcePosition &start,
	         const SourcePosition &end)
	    : Expr(NodeKind::CALL_EXPR, start, end), callee(std::move(callee)),
	      arguments(std::move(arguments))
	{
	}
	CallExpr(ExprPtr callee, const SourcePosition &start,
	         const SourcePosition &end)
	    : Expr(NodeKind::CALL_EXPR, start, end), callee(std::move(callee))
	{
	}
	POP_AST_NODE
};

struct IfExpr final : public Expr
{
	ExprPtr predicate;
	ExprPtr consequence;
	ExprPtr alternative;
	IfExpr(ExprPtr predicate, ExprPtr consequence, ExprPtr alternative,
	       const SourcePosition &start, const SourcePosition &end)
	    : Expr(NodeKind::IF_EXPR, start, end), predicate(std::move(predicate)),
	      consequence(std::move(consequence)),
	      alternative(std::move(alternative))
	{
	}
	POP_AST_NODE
};

struct ForExpr final : public Expr
{
	ExprPtr value;
	ExprPtr iterator;
	ExprPtr sequence;
	ForExpr(ExprPtr value, ExprPtr iterator, ExprPtr sequence,
	        const SourcePosition &start, const SourcePosition &end)
	    : Expr(NodeKind::FOR_EXPR, start, end), value(std::move(value)),
	      iterator(std::move(iterator)), sequence(std::move(sequence))
	{
	}
	POP_AST_NODE
};

//
// Declaration statements
//

struct LetBinding final : public Stmt
{
	String name;
	ExprPtr value;
	LetBinding(const String &name, ExprPtr value, const SourcePosition &start,
	           const SourcePosition &end)
	    : Stmt(NodeKind::LET_BINDING, start, end), name(name),
	      value(std::move(value))
	{
	}
	POP_AST_NODE
};

struct LabelDecl final : public Stmt
{
	String name;
	LabelDecl(const String &name, const SourcePosition &start,
	          const SourcePosition &end)
	    : Stmt(NodeKind::LABEL_DECL, start, end), name(name)
	{
	}
	POP_AST_NODE
};

//
// Other statements
//

struct EmptyStmt final : public Stmt
{
	EmptyStmt(const SourcePosition &start, const SourcePosition &end)
	    : Stmt(NodeKind::EMPTY_STMT, start, end)
	{
	}
	POP_AST_NODE
};

struct ExprStmt final : public Stmt
{
	ExprPtr expr;
	ExprStmt(ExprPtr expr, const SourcePosition &start,
	         const SourcePosition &end)
	    : Stmt(NodeKind::EXPR_STMT, start, end), expr(std::move(expr))
	{
	}
	POP_AST_NODE
};

struct CompoundStmt final : public Stmt
{
	StmtList stmts;
	CompoundStmt(const SourcePosition &start, const SourcePosition &end)
	    : Stmt(NodeKind::COMPOUND_STMT, start, end)
	{
	}
	CompoundStmt(StmtList stmts, const SourcePosition &start,
	             const SourcePosition &end)
	    : Stmt(NodeKind::COMPOUND_STMT, start, end), stmts(std::move(stmts))
	{
	}
	POP_AST_NODE
};

struct BreakStmt final : public Stmt
{
	BreakStmt(const SourcePosition &start, const SourcePosition &end)
	    : Stmt(NodeKind::BREAK_STMT, start, end)
	{
	}
	POP_AST_NODE
};

struct ContinueStmt final : public Stmt
{
	ContinueStmt(const SourcePosition &start, const SourcePosition &end)
	    : Stmt(NodeKind::CONTINUE_STMT, start, end)
	{
	}
	POP_AST_NODE
};

struct GotoStmt final : public Stmt
{
	String label;
	GotoStmt(const String &label, const SourcePosition &start,
	         const SourcePosition &end)
	    : Stmt(NodeKind::GOTO_STMT, start, end), label(label)
	{
	}
	POP_AST_NODE
};

struct ReturnStmt final : public Stmt
{
	ExprPtr expr;
	ReturnStmt(ExprPtr expr, const SourcePosition &start,
	           const SourcePosition &end)
	    : Stmt(NodeKind::RETURN_STMT, start, end), expr(std::move(expr))
	{
	}
	POP_AST_NODE
};

struct IfStmt final : public Stmt
{
	ExprPtr predicate;
	StmtPtr consequence;
	StmtPtr alternative;
	IfStmt(ExprPtr predicate, StmtPtr consequence, StmtPtr alternative,
	       const SourcePosition &start, const SourcePosition &end)
	    : Stmt(NodeKind::IF_STMT, start, end), predicate(std::move(predicate)),
	      consequence(std::move(consequence)),
	      alternative(std::move(alternative))
	{
	}
	POP_AST_NODE
};

struct UnlessStmt final : public Stmt
{
	ExprPtr predicate;
	StmtPtr consequence;
	StmtPtr alternative;
	UnlessStmt(ExprPtr predicate, StmtPtr consequence, StmtPtr alternative,
	           const SourcePosition &start, const SourcePosition &end)
	    : Stmt(NodeKind::UNLESS_STMT, start, end),
	      predicate(std::move(predicate)), consequence(std::move(consequence)),
	      alternative(std::move(alternative))
	{
	}
	POP_AST_NODE
};

struct DoWhileStmt final : public Stmt
{
	ExprPtr expr;
	StmtPtr stmt;
	DoWhileStmt(ExprPtr expr, StmtPtr stmt, const SourcePosition &start,
	            const SourcePosition &end)
	    : Stmt(NodeKind::DO_WHILE_STMT, start, end), expr(std::move(expr)),
	      stmt(std::move(stmt))
	{
	}
	POP_AST_NODE
};

struct DoUntilStmt final : public Stmt
{
	ExprPtr expr;
	StmtPtr stmt;
	DoUntilStmt(ExprPtr expr, StmtPtr stmt, const SourcePosition &start,
	            const SourcePosition &end)
	    : Stmt(NodeKind::DO_UNTIL_STMT, start, end), expr(std::move(expr)),
	      stmt(std::move(stmt))
	{
	}
	POP_AST_NODE
};

struct WhileStmt final : public Stmt
{
	ExprPtr expr;
	StmtPtr stmt;
	WhileStmt(ExprPtr expr, StmtPtr stmt, const SourcePosition &start,
	          const SourcePosition &end)
	    : Stmt(NodeKind::WHILE_STMT, start, end), expr(std::move(expr)),
	      stmt(std::move(stmt))
	{
	}
	POP_AST_NODE
};

struct UntilStmt final : public Stmt
{
	ExprPtr expr;
	StmtPtr stmt;
	UntilStmt(ExprPtr expr, StmtPtr stmt, const SourcePosition &start,
	          const SourcePosition &end)
	    : Stmt(NodeKind::UNTIL_STMT, start, end), expr(std::move(expr)),
	      stmt(std::move(stmt))
	{
	}
	POP_AST_NODE
};

struct ForStmt final : public Stmt
{
	ExprPtr iterator;
	ExprPtr sequence;
	ForStmt(ExprPtr iterator, ExprPtr sequence, const SourcePosition &start,
	        const SourcePosition &end)
	    : Stmt(NodeKind::FOR_STMT, start, end), iterator(std::move(iterator)),
	      sequence(std::move(sequence))
	{
	}
	POP_AST_NODE
};

// namespace Ast
}

// namespace Pop
}

#endif // POP_AST_HPP
