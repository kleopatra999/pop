#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/token.hpp>

namespace Pop
{

const char *token_kind_name(TokenKind kind)
{
	// clang-format off
	switch (kind)
	{
		// various symbols
		case TokenKind::COLON:                    return ",";
		case TokenKind::SEMICOLON:                return ";";
		case TokenKind::COMMA:                    return ",";
		case TokenKind::MEMBER:                   return ".";
		case TokenKind::LBRACE:                   return "{";
		case TokenKind::RBRACE:                   return "}";
		case TokenKind::LPAREN:                   return "(";
		case TokenKind::RPAREN:                   return ")";
		case TokenKind::LBRACKET:                 return "[";
		case TokenKind::RBRACKET:                 return "]";
		// comments
		case TokenKind::SINGLE_LINE_COMMENT:      return "SINGLE_LINE_COMMENT";
		case TokenKind::MULTI_LINE_COMMENT:       return "MULTI_LINE_COMMENT";
		// literals
		case TokenKind::NULL_LITERAL:             return "NULL_LITERAL";
		case TokenKind::TRUE_LITERAL:             return "TRUE_LITERAL";
		case TokenKind::FALSE_LITERAL:            return "FALSE_LITERAL";
		case TokenKind::INT_LITERAL:              return "INT_LITERAL";
		case TokenKind::FLOAT_LITERAL:            return "FLOAT_LITERAL";
		case TokenKind::STRING_LITERAL:           return "STRING_LITERAL";
		case TokenKind::IDENTIFIER:               return "IDENTIFIER";
		// keywords
		case TokenKind::BREAK:                    return "BREAK";
		case TokenKind::CATCH:                    return "CATCH";
		case TokenKind::CONTINUE:                 return "CONTINUE";
		case TokenKind::DO:                       return "DO";
		case TokenKind::ELSE:                     return "ELSE";
		case TokenKind::FALSE:                    return "FALSE";
		case TokenKind::FINALLY:                  return "FINALLY";
		case TokenKind::FOR:                      return "FOR";
		case TokenKind::FUNCTION:                 return "FUNCTION";
		case TokenKind::GOTO:                     return "GOTO";
		case TokenKind::IF:                       return "IF";
		case TokenKind::IN:                       return "IN";
		case TokenKind::LET:                      return "LET";
		case TokenKind::NUL:                      return "NUL";
		case TokenKind::RETURN:                   return "RETURN";
		case TokenKind::THROW:                    return "THROW";
		case TokenKind::TRUE:                     return "TRUE";
		case TokenKind::TRY:                      return "TRY";
		case TokenKind::UNLESS:                   return "UNLESS";
		case TokenKind::UNTIL:                    return "UNTIL";
		case TokenKind::WHILE:                    return "WHILE";
		// arithmetic ops
		case TokenKind::ADD:                      return "ADD";
		case TokenKind::SUB:                      return "SUB";
		case TokenKind::MUL:                      return "MUL";
		case TokenKind::DIV:                      return "DIV";
		case TokenKind::MOD:                      return "MOD";
		case TokenKind::POW:                      return "POW";
		case TokenKind::UPLUS:                    return "UPLUS";
		case TokenKind::UMINUS:                   return "UMINUS";
		// logical ops
		case TokenKind::L_AND:                    return "L_AND";
		case TokenKind::L_OR:                     return "L_OR";
		case TokenKind::L_NOT:                    return "L_NOT";
		// bitwise ops
		case TokenKind::B_AND:                    return "B_AND";
		case TokenKind::B_OR:                     return "B_OR";
		case TokenKind::B_XOR:                    return "B_XOR";
		case TokenKind::B_NOT:                    return "B_NOT";
		case TokenKind::LSHIFT:                   return "LSHIFT";
		case TokenKind::RSHIFT:                   return "RSHIFT";
		// in-places ops
		case TokenKind::ADD_ASSIGN:               return "ADD_ASSIGN";
		case TokenKind::SUB_ASSIGN:               return "SUB_ASSIGN";
		case TokenKind::MUL_ASSIGN:               return "MUL_ASSIGN";
		case TokenKind::DIV_ASSIGN:               return "DIV_ASSIGN";
		case TokenKind::MOD_ASSIGN:               return "MOD_ASSIGN";
		case TokenKind::POW_ASSIGN:               return "POW_ASSIGN";
		case TokenKind::AND_ASSIGN:               return "AND_ASSIGN";
		case TokenKind::OR_ASSIGN:                return "OR_ASSIGN";
		case TokenKind::XOR_ASSIGN:               return "XOR_ASSIGN";
		case TokenKind::NOT_ASSIGN:               return "NOT_ASSIGN";
		case TokenKind::LEFT_ASSIGN:              return "LEFT_ASSIGN";
		case TokenKind::RIGHT_ASSIGN:             return "RIGHT_ASSIGN";
		case TokenKind::ASSIGN:                   return "ASSIGN";
		case TokenKind::INCREMENT:                return "INCREMENT";
		case TokenKind::DECREMENT:                return "DECREMENT";
		case TokenKind::PREINC:                   return "PREINC";
		case TokenKind::PREDEC:                   return "PREDEC";
		case TokenKind::POSTINC:                  return "POSTINC";
		case TokenKind::POSTDEC:                  return "POSTDEC";
		// comparisons
		case TokenKind::EQ:                       return "EQ";
		case TokenKind::NE:                       return "NE";
		case TokenKind::GT:                       return "GT";
		case TokenKind::GE:                       return "GE";
		case TokenKind::LT:                       return "LT";
		case TokenKind::LE:                       return "LE";
		// other
		case TokenKind::END:                      return "END";
		case TokenKind::ERROR:                    return "ERROR";
		default:                       return "UNKNOWN";
	}
	// clang-format on
}

// namespace Pop
}
