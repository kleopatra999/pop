#include <pop/parser.hpp>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace std
{
template <>
struct hash<Pop::TokenKind>
{
	size_t operator()(Pop::TokenKind kind) const
	{
		using UT = std::underlying_type<Pop::TokenKind>::type;
		return std::hash<UT>()(static_cast<UT>(kind));
	}
};
}

namespace Pop
{

#define expect(t) expect_(t, __LINE__)

static int ind_lvl = 0;

static std::string ind_str()
{
	return std::string(ind_lvl, ' ');
}

struct ScopedTracer
{
	std::string fnc;
	ScopedTracer(const std::string &fnc) : fnc(fnc)
	{
		std::cerr << ind_str() << "-> " << fnc << std::endl;
		ind_lvl += 2;
	}
	~ScopedTracer()
	{
		ind_lvl -= 2;
		std::cerr << ind_str() << "<- " << fnc << std::endl;
	}
};

#if 0
#define TRACE_FUNC() ScopedTracer __tracer_##__COUNTER__##__(__func__)
#else
#define TRACE_FUNC() \
	do               \
	{                \
	} while (0)
#endif

using namespace Ast;

template <class T, class... Args>
static inline std::unique_ptr<T> mknode(Args &&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

struct BinOpInfo
{
	int prec;
	bool left_assoc;
};

// clang-format off
static const std::unordered_map<TokenKind, BinOpInfo> binop_precedence =
{
	{TokenKind::POSTINC, {15,true}},
	{TokenKind::POSTDEC, {15,true}},
	{TokenKind::UPLUS, {15,true}},
	{TokenKind::UMINUS, {15,true}},
	{TokenKind::MEMBER, {15,true}},
	{TokenKind::PREINC, {14,false}},
	{TokenKind::PREDEC, {14,false}},
	{TokenKind::L_NOT, {14,false}},
	{TokenKind::B_NOT, {14,false}},
	{TokenKind::MUL, {12,true}},
	{TokenKind::DIV, {12,true}},
	{TokenKind::MOD, {12,true}},
	{TokenKind::POW, {12,true}},
	{TokenKind::ADD, {11,true}},
	{TokenKind::SUB, {11,true}},
	{TokenKind::LSHIFT, {10,true}},
	{TokenKind::RSHIFT, {10,true}},
	{TokenKind::LT, {9,true}},
	{TokenKind::LE, {9,true}},
	{TokenKind::GT, {9,true}},
	{TokenKind::GE, {9,true}},
	{TokenKind::EQ, {8,true}},
	{TokenKind::NE, {8,true}},
	{TokenKind::B_AND, {7,true}},
	{TokenKind::B_XOR, {6,true}},
	{TokenKind::B_OR, {5,true}},
	{TokenKind::L_AND, {4,true}},
	{TokenKind::L_OR, {3,true}},
	{TokenKind::ASSIGN, {2,false}},
	{TokenKind::ADD_ASSIGN, {2,false}},
	{TokenKind::SUB_ASSIGN, {2,false}},
	{TokenKind::MUL_ASSIGN, {2,false}},
	{TokenKind::DIV_ASSIGN, {2,false}},
	{TokenKind::MOD_ASSIGN, {2,false}},
	{TokenKind::LEFT_ASSIGN, {2,false}},
	{TokenKind::RIGHT_ASSIGN, {2,false}},
	{TokenKind::AND_ASSIGN, {2,false}},
	{TokenKind::XOR_ASSIGN, {2,false}},
	{TokenKind::OR_ASSIGN, {2,false}},
};
// clang-format on

static inline int get_token_precedence(TokenKind kind)
{
	auto found = binop_precedence.find(kind);
	if (found != binop_precedence.end())
	{
		int prec = found->second.prec;
		if (prec <= 0)
			return -1;
		return prec;
	}
	return -1;
}

static inline bool is_left_associative(TokenKind kind)
{
	auto found = binop_precedence.find(kind);
	if (found != binop_precedence.end())
		return found->second.left_assoc;
	std::cerr << "unable to find precedence of operator '"
	          << token_kind_name(kind) << "'" << std::endl;
	return false;
}

std::uint64_t Parser::parse_int(const std::string &s)
{
	if (s.size() > 2 && s[0] == '0')
	{
		if (s[1] == 'x' || s[1] == 'X')
			return std::stoull(s.substr(2), 0, 16);
		else if (s[1] == 'b' || s[1] == 'B')
			return std::stoull(s.substr(2), 0, 2);
		else if (s[1] == 'd' || s[1] == 'D')
			return std::stoull(s.substr(2), 0, 10);
		else if (s[1] == 'o' || s[1] == 'O')
			return std::stoull(s.substr(2), 0, 8);
		else
			return std::stoull(s, 0, 8);
	}
	else if (s.size() > 0 && s[0] == '0')
		return std::stoull(s, 0, 8);
	else
		return std::stoull(s, 0, 10);
}

double Parser::parse_float(const std::string &s)
{
	auto dot_pos = s.find('.');
	if (dot_pos != s.npos)
	{
		std::string whole(s.substr(0, dot_pos));
		std::string fract(s.substr(dot_pos + 1));
		auto iwhole = parse_int(s);
		auto ifract = parse_int(s);
		std::string num(std::to_string(iwhole) + "." + std::to_string(ifract));
		return std::stod(num);
	}
	else
	{
		return std::stod(s);
	}
}

ModulePtr Parser::parse()
{
	TRACE_FUNC();
	auto mod = mknode<Module>(lex.filename);
	lex.next_token(tok);
	while (auto stmt = parse_stmt())
		mod->stmts.push_back(std::move(stmt));
	return mod;
}

StmtPtr Parser::parse_stmt()
{
	TRACE_FUNC();
	switch (tok.kind)
	{
		case TokenKind::LET:
			return parse_let_binding();
		case TokenKind::BREAK:
			return parse_break_stmt();
		case TokenKind::CONTINUE:
			return parse_continue_stmt();
		case TokenKind::RETURN:
			return parse_return_stmt();
		case TokenKind::GOTO:
			return parse_goto_stmt();
		case TokenKind::IF:
			return parse_if_stmt();
		case TokenKind::UNLESS:
			return parse_unless_stmt();
		case TokenKind::DO:
			return parse_do_stmt();
		case TokenKind::WHILE:
			return parse_while_stmt();
		case TokenKind::UNTIL:
			return parse_until_stmt();
		case TokenKind::FOR:
			return parse_for_stmt();
		case TokenKind::FUNCTION:
			return parse_func_decl();
		case TokenKind::LBRACE:
			return parse_compound_stmt();
		case TokenKind::SEMICOLON:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			expect(';');
			return mknode<EmptyStmt>(start, end);
		}
		case TokenKind::END:
			return nullptr;
		default:
		{
			/* expr stmt */
			auto start = tok.range.start;
			if (auto expr = parse_expr())
			{
				auto end = expr->range.end;
				expect(';');
				return mknode<ExprStmt>(std::move(expr), start, end);
			}
			else
			{
				/*
				std::stringstream ss;
				ss << "unexpected '" << tok.kind_name()
				   << "', expecting statement";
				throw SyntaxError(ss.str(), tok.range.start.line,
				                  tok.range.start.column);
				*/
			}
			break;
		}
	}
	return nullptr;
}

StmtPtr Parser::parse_let_binding()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::LET);
	std::string name = lex.text;
	expect(TokenKind::IDENTIFIER);
	expect(TokenKind::ASSIGN);
	auto init = parse_expr();
	auto end = tok.range.end;
	expect(';');
	return mknode<LetBinding>(name, std::move(init), start, end);
}

StmtPtr Parser::parse_break_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::BREAK);
	auto end = tok.range.end;
	expect(';');
	return mknode<BreakStmt>(start, end);
}

StmtPtr Parser::parse_continue_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::CONTINUE);
	auto end = tok.range.end;
	expect(';');
	return mknode<ContinueStmt>(start, end);
}

StmtPtr Parser::parse_return_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::RETURN);
	auto end = tok.range.end;
	ExprPtr expr;
	if (!accept(';'))
	{
		expr = parse_expr();
		end = tok.range.end;
		expect(';');
	}
	return mknode<ReturnStmt>(std::move(expr), start, end);
}

StmtPtr Parser::parse_goto_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::GOTO);
	std::string name = tok.text;
	expect(TokenKind::IDENTIFIER);
	auto end = tok.range.end;
	expect(';');
	return mknode<GotoStmt>(name, start, end);
}

StmtPtr Parser::parse_compound_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect('{');
	StmtList stmts;
	while (auto stmt = parse_stmt())
		stmts.emplace_back(std::move(stmt));
	auto end = tok.range.end;
	expect('}');
	return mknode<CompoundStmt>(std::move(stmts), start, end);
}

StmtPtr Parser::parse_if_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::IF);
	expect('(');
	auto predicate = parse_expr();
	expect(')');
	auto consequence = parse_stmt();
	StmtPtr alternative;
	if (accept(TokenKind::ELSE))
		alternative = parse_stmt();
	auto end = alternative ? alternative->range.end : consequence->range.end;
	return mknode<IfStmt>(std::move(predicate), std::move(consequence),
	                      std::move(alternative), start, end);
}

StmtPtr Parser::parse_unless_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::UNLESS);
	expect('(');
	auto predicate = parse_expr();
	expect(')');
	auto consequence = parse_stmt();
	StmtPtr alternative;
	if (accept(TokenKind::ELSE))
		alternative = parse_stmt();
	auto end = alternative ? alternative->range.end : consequence->range.end;
	return mknode<UnlessStmt>(std::move(predicate), std::move(consequence),
	                          std::move(alternative), start, end);
}

StmtPtr Parser::parse_do_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::DO);
	auto stmt = parse_stmt();
	if (accept(TokenKind::WHILE))
	{
		expect('(');
		auto expr = parse_expr();
		expect(')');
		auto end = tok.range.end;
		expect(';');
		return mknode<DoWhileStmt>(std::move(expr), std::move(stmt), start,
		                           end);
	}
	else
	{
		expect(TokenKind::UNLESS);
		expect('(');
		auto expr = parse_expr();
		expect(')');
		auto end = tok.range.end;
		expect(';');
		return mknode<DoUntilStmt>(std::move(expr), std::move(stmt), start,
		                           end);
	}
}

StmtPtr Parser::parse_while_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::WHILE);
	expect('(');
	auto expr = parse_expr();
	expect(')');
	auto stmt = parse_stmt();
	auto end = stmt->range.end;
	return mknode<WhileStmt>(std::move(expr), std::move(stmt), start, end);
}

StmtPtr Parser::parse_until_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::UNTIL);
	expect('(');
	auto expr = parse_expr();
	expect(')');
	auto stmt = parse_stmt();
	auto end = stmt->range.end;
	return mknode<UntilStmt>(std::move(expr), std::move(stmt), start, end);
}

StmtPtr Parser::parse_for_stmt()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::FOR);
	expect('(');
	std::string text(tok.text);
	auto id_start = tok.range.start;
	auto id_end = tok.range.end;
	expect(TokenKind::IDENTIFIER);
	auto iterator = mknode<Identifier>(text, id_start, id_end);
	expect(TokenKind::IN);
	auto sequence = parse_expr();
	expect(')');
	auto stmt = parse_stmt();
	auto end = stmt->range.end;
	return mknode<ForStmt>(std::move(iterator), std::move(sequence), start,
	                       end);
}

StmtPtr Parser::parse_func_decl()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::FUNCTION);
	std::string text(tok.text);
	expect(TokenKind::IDENTIFIER);
	StringList arguments;
	expect('(');
	if (!accept(')'))
	{
		while (true)
		{
			std::string name(tok.text);
			expect(TokenKind::IDENTIFIER);
			arguments.emplace_back(std::move(name));
			if (!accept(','))
				break;
		}
		expect(')');
	}
	expect('{');
	StmtList stmts;
	while (auto stmt = parse_stmt())
		stmts.emplace_back(std::move(stmt));
	auto end = tok.range.end;
	expect('}');
	auto func = mknode<FunctionLiteral>(std::move(arguments), std::move(stmts),
	                                    start, end);
	return mknode<LetBinding>(text, std::move(func), start, end);
}

ExprPtr Parser::parse_expr()
{
	TRACE_FUNC();
	if (auto lhs = parse_unary_expr())
		return parse_binop_rhs(0, std::move(lhs));
	return nullptr;
}

ExprPtr Parser::parse_paren_expr()
{
	TRACE_FUNC();
	expect('(');
	auto expr = parse_expr();
	expect(')');
	return expr;
}

ExprPtr Parser::parse_primary_expr()
{
	TRACE_FUNC();
	switch (tok.kind)
	{
		case TokenKind::NUL:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			expect(TokenKind::NUL);
			return mknode<NullLiteral>(start, end);
		}
		case TokenKind::TRUE:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			expect(TokenKind::TRUE);
			return mknode<BoolLiteral>(true, start, end);
		}
		case TokenKind::FALSE:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			expect(TokenKind::FALSE);
			return mknode<BoolLiteral>(false, start, end);
		}
		case TokenKind::INT_LITERAL:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			std::string text(tok.text);
			expect(TokenKind::INT_LITERAL);
			return mknode<IntLiteral>(parse_int(text), start, end);
		}
		case TokenKind::FLOAT_LITERAL:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			std::string text(tok.text);
			expect(TokenKind::FLOAT_LITERAL);
			return mknode<FloatLiteral>(parse_float(text), start, end);
		}
		case TokenKind::STRING_LITERAL:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			std::string text(tok.text);
			expect(TokenKind::STRING_LITERAL);
			return mknode<StringLiteral>(text, start, end);
		}
		case TokenKind::IDENTIFIER:
		{
			auto start = tok.range.start;
			auto end = tok.range.end;
			std::string text(tok.text);
			expect(TokenKind::IDENTIFIER);
			return mknode<Identifier>(text, start, end);
		}
		case TokenKind::FUNCTION:
			return parse_func_expr();
		case TokenKind::LPAREN:
			return parse_paren_expr();
		case TokenKind::LBRACE:
			return parse_object_expr();
		case TokenKind::END:
			return nullptr;
		default:
		{
			/*
			std::stringstream ss;
			ss << "unexpected '" << tok.kind_name()
			   << "', expecting an expression";
			throw SyntaxError(ss.str(), tok.range.start.line,
			                  tok.range.start.column);
			*/
			break;
		}
	}
	return nullptr;
}

ExprPtr Parser::parse_binop_rhs(int expr_prec, ExprPtr lhs)
{
	TRACE_FUNC();
	while (true)
	{
		int tok_prec = get_token_precedence(tok.kind);
		if (tok_prec < expr_prec)
			return lhs;
		auto binop = tok.kind;
		expect(tok.kind);
		auto rhs = parse_unary_expr();
		if (!rhs)
			return nullptr;
		int next_prec = get_token_precedence(tok.kind);
		if (tok_prec <= next_prec)
		{
			if (is_left_associative(tok.kind))
				rhs = parse_binop_rhs(tok_prec + 1, std::move(rhs));
			else
				rhs = parse_binop_rhs(tok_prec, std::move(rhs));
			if (!rhs)
				return nullptr;
		}
		auto start = lhs->range.start;
		auto end = rhs->range.end;
		lhs = mknode<BinaryExpr>(binop, std::move(lhs), std::move(rhs), start,
		                         end);
	}
}

static inline bool is_unary_pre_op(TokenKind kind)
{
	switch (kind)
	{
		case TokenKind::ADD:
		case TokenKind::SUB:
		case TokenKind::L_NOT:
		case TokenKind::B_NOT:
		case TokenKind::INCREMENT:
		case TokenKind::DECREMENT:
			return true;
		default:
			return false;
	}
}

static inline bool is_unary_post_op(TokenKind kind)
{
	switch (kind)
	{
		case TokenKind::INCREMENT:
		case TokenKind::DECREMENT:
		case TokenKind::IF:
		case TokenKind::LBRACKET:
		case TokenKind::LPAREN:
		case TokenKind::MEMBER:
			return true;
		default:
			return false;
	}
}

ExprPtr Parser::parse_unary_expr()
{
	TRACE_FUNC();
	ExprPtr expr;
	auto start = tok.range.start;
	auto kind = tok.kind;
	if (is_unary_pre_op(kind))
	{
		expect(kind);
		auto operand = parse_unary_expr();
		auto end = operand->range.end;
		switch (kind)
		{
			case TokenKind::ADD:
				expr = mknode<UnaryExpr>(TokenKind::UPLUS, std::move(operand),
				                         start, end);
				break;
			case TokenKind::SUB:
				expr = mknode<UnaryExpr>(TokenKind::UMINUS, std::move(operand),
				                         start, end);
				break;
			case TokenKind::L_NOT:
				expr = mknode<UnaryExpr>(TokenKind::L_NOT, std::move(operand),
				                         start, end);
				break;
			case TokenKind::B_NOT:
				expr = mknode<UnaryExpr>(TokenKind::B_NOT, std::move(operand),
				                         start, end);
				break;
			case TokenKind::INCREMENT:
				expr = mknode<UnaryExpr>(TokenKind::PREINC, std::move(operand),
				                         start, end);
				break;
			case TokenKind::DECREMENT:
				expr = mknode<UnaryExpr>(TokenKind::PREDEC, std::move(operand),
				                         start, end);
				break;
			default:
				break;
		}
	}
	else if (!is_unary_post_op(kind) || kind == TokenKind::LPAREN)
	{
		expr = parse_primary_expr();
		kind = tok.kind;
		while (is_unary_post_op(kind))
		{
			auto end = tok.range.end;
			expect(kind);
			switch (kind)
			{
				case TokenKind::INCREMENT:
					expr = mknode<UnaryExpr>(TokenKind::POSTINC,
					                         std::move(expr), start, end);
					break;
				case TokenKind::DECREMENT:
					expr = mknode<UnaryExpr>(TokenKind::POSTDEC,
					                         std::move(expr), start, end);
					break;
				case TokenKind::IF:
				{
					auto predicate = parse_expr();
					expect(TokenKind::ELSE);
					auto alternative = parse_expr();
					end = alternative->range.end;
					expr = mknode<IfExpr>(std::move(predicate), std::move(expr),
					                      std::move(alternative), start, end);
					break;
				}
				case TokenKind::LBRACKET:
				{
					auto index = parse_expr();
					auto end = tok.range.end;
					expect(']');
					expr = mknode<IndexExpr>(std::move(expr), std::move(index),
					                         start, end);
					break;
				}
				case TokenKind::LPAREN:
				{
					ExprList arguments;
					while (tok.kind != TokenKind::RPAREN)
					{
						arguments.emplace_back(parse_expr());
						if (!accept(','))
							break;
					}
					auto end = tok.range.end;
					expect(')');
					expr = mknode<CallExpr>(std::move(expr),
					                        std::move(arguments), start, end);
					break;
				}
				case TokenKind::MEMBER:
				{
					auto id_start = tok.range.start;
					auto id_end = tok.range.end;
					std::string id_name = tok.text;
					expect(TokenKind::IDENTIFIER);
					auto id_expr =
					    mknode<Identifier>(id_name, id_start, id_end);
					auto start = expr->range.start;
					auto end = id_expr->range.end;
					expr = mknode<MemberExpr>(std::move(expr),
					                          std::move(id_expr), start, end);
					break;
				}
				default:
					break;
			}
			kind = tok.kind;
		}
	}
	return expr;
}

Ast::ExprPtr Parser::parse_func_expr()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect(TokenKind::FUNCTION);
	expect('(');
	StringList arguments;
	if (!accept(')'))
	{
		while (true)
		{
			std::string name(tok.text);
			expect(TokenKind::IDENTIFIER);
			arguments.emplace_back(std::move(name));
			if (!accept(','))
				break;
		}
		expect(')');
	}
	expect('{');
	StmtList stmts;
	while (auto stmt = parse_stmt())
		stmts.emplace_back(std::move(stmt));
	auto end = tok.range.end;
	expect('}');
	return mknode<FunctionLiteral>(std::move(arguments), std::move(stmts),
	                               start, end);
}

Ast::ExprPtr Parser::parse_object_expr()
{
	TRACE_FUNC();
	auto start = tok.range.start;
	expect('{');
	StringList member_names;
	ExprList member_values;
	while (tok.kind == TokenKind::IDENTIFIER)
	{
		std::string key(tok.text);
		expect(TokenKind::IDENTIFIER);
		expect(':');
		auto expr = parse_expr();
		member_names.emplace_back(std::move(key));
		member_values.emplace_back(std::move(expr));
		if (!accept(','))
			break;
	}
	auto end = tok.range.end;
	expect('}');
	return mknode<ObjectLiteral>(std::move(member_names),
	                             std::move(member_values), start, end);
}

// namespace Pop
}
