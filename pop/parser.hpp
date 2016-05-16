#ifndef POP_PARSER_HPP
#define POP_PARSER_HPP

#include <pop/ast.hpp>
#include <pop/error.hpp>
#include <pop/lexer.hpp>
#include <pop/token.hpp>
#include <cassert>
#include <istream>
#include <memory>
#include <stack>
#include <sstream>

namespace Pop
{

class Parser
{
public:
	Parser(std::istream &inp, const char *fn = "<stream>") : lex(inp, fn)
	{
	}

	Ast::ModulePtr parse();

private:
	Lexer lex;
	Token tok;

	bool accept(int t)
	{
		if (t == int(tok.kind))
		{
			lex.next_token(tok);
			return true;
		}
		return false;
	}

	bool accept(TokenKind tk)
	{
		return accept(int(tk));
	}

	void expect(int t)
	{
		if (!accept(t))
		{
			std::stringstream ss;
			ss << "unexpected '" << token_kind_name(tok.kind) << "', expected '"
			   << token_kind_name(TokenKind(t)) << "' (" << int(t) << ")";
			throw SyntaxError(ss.str(), tok.range.start.line,
			                  tok.range.start.column);
		}
	}

	void expect(TokenKind tk)
	{
		expect(int(tk));
	}

	std::uint64_t parse_int(const std::string &s);
	double parse_float(const std::string &s);

	Ast::StmtPtr parse_stmt();
	Ast::StmtPtr parse_let_binding();
	Ast::StmtPtr parse_break_stmt();
	Ast::StmtPtr parse_continue_stmt();
	Ast::StmtPtr parse_return_stmt();
	Ast::StmtPtr parse_goto_stmt();
	Ast::StmtPtr parse_compound_stmt();
	Ast::StmtPtr parse_if_stmt();
	Ast::StmtPtr parse_unless_stmt();
	Ast::StmtPtr parse_do_stmt();
	Ast::StmtPtr parse_while_stmt();
	Ast::StmtPtr parse_until_stmt();
	Ast::StmtPtr parse_for_stmt();
	Ast::StmtPtr parse_func_decl();

	Ast::ExprPtr parse_expr();
	Ast::ExprPtr parse_primary_expr();
	Ast::ExprPtr parse_paren_expr();
	Ast::ExprPtr parse_ident_expr();
	Ast::ExprPtr parse_binop_rhs(int expr_prec, Ast::ExprPtr lhs);
	Ast::ExprPtr parse_unary_expr();
	Ast::ExprPtr parse_func_expr();
};

static inline Ast::ModulePtr parse(std::istream &inp,
                                   const char *fn = "<stream>")
{
	Parser p(inp, fn);
	return p.parse();
}

// namespace Pop
}

#endif // POP_PARSER_HPP
