#include <pop/lexer.hpp>
#include <pop/error.hpp>
#include <cassert>
#include <cctype>
#include <iostream>
#include <unordered_map>

namespace Pop
{

static const std::unordered_map<std::string, TokenKind> keyword_map = {
    {"break", TokenKind::BREAK},
    {"catch", TokenKind::CATCH},
    {"continue", TokenKind::CONTINUE},
    {"do", TokenKind::DO},
    {"else", TokenKind::ELSE},
    {"false", TokenKind::FALSE},
    {"finally", TokenKind::FINALLY},
    {"for", TokenKind::FOR},
    {"function", TokenKind::FUNCTION},
    {"goto", TokenKind::GOTO},
    {"if", TokenKind::IF},
    {"in", TokenKind::IN},
    {"let", TokenKind::LET},
    {"null", TokenKind::NUL},
    {"return", TokenKind::RETURN},
    {"throw", TokenKind::THROW},
    {"true", TokenKind::TRUE},
    {"try", TokenKind::TRY},
    {"unless", TokenKind::UNLESS},
    {"until", TokenKind::UNTIL},
    {"while", TokenKind::WHILE},
};

static inline bool is_hex(int ch)
{
	return (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch < 'F') ||
	       (ch >= '0' && ch <= '9');
}

static inline bool is_binary(int ch)
{
	return (ch == '0') || (ch == '1');
}

static inline bool is_octal(int ch)
{
	return (ch >= '0' && ch <= '7');
}

static inline bool is_decimal(int ch)
{
	return (ch >= '0' && ch <= '9');
}

int Lexer::getch()
{
	chr = inp.get();
	offset++;
	if (chr == '\n')
	{
		line++;
		column = std::uint32_t(-1);
	}
	else
	{
		column++;
	}
	return chr;
}

void Lexer::reset()
{
	kind = TokenKind::ERROR;
	text.clear();
}

void Lexer::start_token(Token &tok)
{
	tok.kind = TokenKind::ERROR;
	tok.text.clear();
	tok.range.start.offset = offset;
	tok.range.start.line = line;
	tok.range.start.column = column;
	tok.range.end.offset = offset;
	tok.range.end.line = line;
	tok.range.end.column = column;
}

TokenKind Lexer::end_token(TokenKind k, Token &tok)
{
	tok.kind = k;
	tok.text = text;
	tok.range.end.offset = offset;
	tok.range.end.line = line;
	tok.range.end.column = column;
	return k;
}

TokenKind Lexer::next_token(Token &tok)
{
	TokenKind k = TokenKind::ERROR;

	reset();
	while (isspace(chr))
		getch();
	start_token(tok);

	if (isalpha(chr) || chr == '_') // identifiers or keywords
	{
		do
		{
			text += chr;
			getch();
		} while (isalnum(chr) || chr == '_');
		auto found = keyword_map.find(text);
		if (found != keyword_map.end())
		{
			text.clear();
			k = end_token(found->second, tok);
		}
		else
			k = end_token(TokenKind::IDENTIFIER, tok);
	}
	else if (isdigit(chr) || chr == '.') // numeric literals
	{
		bool is_float = (chr == '.');
		if (chr == '0')
		{
			text += chr;
			getch();
			if (chr == 'x' || chr == 'X') // hex literal
			{
				do
				{
					text += chr;
					if (chr == '.')
					{
						if (is_float)
						{
							throw SyntaxError("multiple decimal points in "
							                  "floating-point hexidecimal "
							                  "literal",
							                  line, column);
						}
						else
						{
							is_float = true;
						}
					}
					getch();
				} while (is_hex(chr) || chr == '.');
				if (is_float)
					k = end_token(TokenKind::FLOAT_LITERAL, tok);
				else
					k = end_token(TokenKind::INT_LITERAL, tok);
			}
			else if (chr == 'b' || chr == 'B') // binary literal
			{
				do
				{
					text += chr;
					if (chr == '.')
					{
						if (is_float)
						{
							throw SyntaxError("multiple decimal points in "
							                  "floating-point binary literal",
							                  line, column);
						}
						else
						{
							is_float = true;
						}
					}
					getch();
				} while (is_binary(chr) || chr == '.');
				if (is_float)
					k = end_token(TokenKind::FLOAT_LITERAL, tok);
				else
					k = end_token(TokenKind::INT_LITERAL, tok);
			}
			else if (chr == 'o' || chr == 'O') // octal literal
			{
				do
				{
					text += chr;
					if (chr == '.')
					{
						if (is_float)
						{
							throw SyntaxError("multiple decimal points in "
							                  "floating-point octal literal",
							                  line, column);
						}
						else
						{
							is_float = true;
						}
					}
					getch();
				} while (is_octal(chr) || chr == '.');
				if (is_float)
					k = end_token(TokenKind::FLOAT_LITERAL, tok);
				else
					k = end_token(TokenKind::INT_LITERAL, tok);
			}
			else if (chr == 'd' || chr == 'D') // decimal literal
			{
				do
				{
					text += chr;
					if (chr == '.')
					{
						if (is_float)
						{
							throw SyntaxError("multiple decimal points in "
							                  "floating-point decimal literal",
							                  line, column);
						}
						else
						{
							is_float = true;
						}
					}
					getch();
				} while (is_decimal(chr) || chr == '.');
				if (is_float)
					k = end_token(TokenKind::FLOAT_LITERAL, tok);
				else
					k = end_token(TokenKind::INT_LITERAL, tok);
			}
			else // 0-prefixed octal literal
			{
				while (is_octal(chr) || chr == '.')
				{
					text += chr;
					if (chr == '.')
					{
						if (is_float)
						{
							throw SyntaxError("multiple decimal points in "
							                  "floating-point octal literal",
							                  line, column);
						}
						else
						{
							is_float = true;
						}
					}
					getch();
				}
				if (is_float)
					k = end_token(TokenKind::FLOAT_LITERAL, tok);
				else
					k = end_token(TokenKind::INT_LITERAL, tok);
			}
		}
		else if (is_decimal(chr) ||
		         (chr == '.' && is_decimal(inp.peek()))) // decimal literal
		{
			do
			{
				text += chr;
				if (chr == '.')
				{
					if (is_float)
					{
						throw SyntaxError("multiple decimal points in "
						                  "floating-point decimal literal",
						                  line, column);
					}
					else
					{
						is_float = true;
					}
				}
				getch();
			} while (is_decimal(chr) || chr == '.');
			if (is_float)
				k = end_token(TokenKind::FLOAT_LITERAL, tok);
			else
				k = end_token(TokenKind::INT_LITERAL, tok);
		}
		else if (chr == '.')
		{
			k = end_token(TokenKind::MEMBER, tok);
			getch();
		}
		else
		{
			assert(false);
		}
	}
	else if (chr == '"' || chr == '\'')
	{
		int quote = chr;
		getch();          // skip starting quote
		if (chr == quote) // empty string
		{
			k = end_token(TokenKind::STRING_LITERAL, tok);
			getch(); // skip closing quote
		}
		else
		{
			do
			{
				if (chr == '\\' && inp.peek() == quote)
				{
					text += quote;
					getch();
				}
				else
				{
					text += chr;
				}
				getch();
				if (chr == quote)
				{
					getch();
					break;
				}
			} while (chr != EOF);
			if (chr == EOF)
			{
				throw SyntaxError("EOF encountered in string literal", line,
				                  column);
			}
			k = end_token(TokenKind::STRING_LITERAL, tok);
		}
	}
	else if (chr == '+')
	{
		getch();
		if (chr == '=') // add-assign
		{
			k = end_token(TokenKind::ADD_ASSIGN, tok);
			getch();
		}
		else if (chr == '+') // increment
		{
			k = end_token(TokenKind::INCREMENT, tok);
			getch();
		}
		else // add
		{
			k = end_token(TokenKind::ADD, tok);
		}
	}
	else if (chr == '-')
	{
		getch();
		if (chr == '=') // sub-assign
		{
			k = end_token(TokenKind::SUB_ASSIGN, tok);
			getch();
		}
		else if (chr == '-') // decrement
		{
			k = end_token(TokenKind::DECREMENT, tok);
			getch();
		}
		else // subtract
		{
			k = end_token(TokenKind::SUB, tok);
		}
	}
	else if (chr == '*')
	{
		getch();
		if (chr == '=') // mul-assign
		{
			k = end_token(TokenKind::MUL_ASSIGN, tok);
			getch();
		}
		else if (chr == '*') // power-of
		{
			getch();
			if (chr == '=')
			{
				k = end_token(TokenKind::POW_ASSIGN, tok);
				getch();
			}
			else
			{
				k = end_token(TokenKind::POW, tok);
			}
		}
		else // multiply
		{
			k = end_token(TokenKind::MUL, tok);
		}
	}
	else if (chr == '/')
	{
		getch();
		if (chr == '/') // single-line comment
		{
			text = "/";
			do
			{
				text += chr;
			} while (getch() != '\n');
			k = end_token(TokenKind::SINGLE_LINE_COMMENT, tok);
		}
		else if (chr == '*') // multi-line comment
		{
			text = "/";
			do
			{
				text += chr;
				getch();
				if (chr == '*' && inp.peek() == '/')
				{
					text += "*/";
					getch(); // skip asterisk
					getch(); // skip peeked trailing slash
					break;
				}
			} while (chr != EOF);
			if (chr == EOF)
			{
				throw SyntaxError("EOF encountered in multi-line comment", line,
				                  column);
			}
			k = end_token(TokenKind::MULTI_LINE_COMMENT, tok);
		}
		else if (chr == '=') // divide-assign
		{
			k = end_token(TokenKind::DIV_ASSIGN, tok);
			getch();
		}
		else // divide
		{
			k = end_token(TokenKind::DIV, tok);
		}
	}
	else if (chr == '%')
	{
		getch();
		if (chr == '=') // mod-assign
		{
			k = end_token(TokenKind::MOD_ASSIGN, tok);
			getch();
		}
		else // modulo
		{
			k = end_token(TokenKind::MOD, tok);
		}
	}
	else if (chr == '&')
	{
		getch();
		if (chr == '&') // logical and
		{
			k = end_token(TokenKind::L_AND, tok);
			getch();
		}
		else if (chr == '=') // and-assign
		{
			k = end_token(TokenKind::AND_ASSIGN, tok);
			getch();
		}
		else // bitwise and
		{
			k = end_token(TokenKind::B_AND, tok);
		}
	}
	else if (chr == '|')
	{
		getch();
		if (chr == '|') // logical or
		{
			k = end_token(TokenKind::L_OR, tok);
			getch();
		}
		else if (chr == '=') // or-assign
		{
			k = end_token(TokenKind::OR_ASSIGN, tok);
			getch();
		}
		else // bitwise or
		{
			k = end_token(TokenKind::B_OR, tok);
		}
	}
	else if (chr == '^')
	{
		getch();
		if (chr == '=') // xor-assign
		{
			k = end_token(TokenKind::XOR_ASSIGN, tok);
			getch();
		}
		else // bitwise xor
		{
			k = end_token(TokenKind::B_XOR, tok);
		}
	}
	else if (chr == '~')
	{
		getch();
		if (chr == '=') // bitwise complement-assign
		{
			k = end_token(TokenKind::NOT_ASSIGN, tok);
			getch();
		}
		else // bitwise complement
		{
			k = end_token(TokenKind::B_NOT, tok);
		}
	}
	else if (chr == '=')
	{
		getch();
		if (chr == '=') // equals comparison
		{
			k = end_token(TokenKind::EQ, tok);
			getch();
		}
		else // assignment
		{
			k = end_token(TokenKind::ASSIGN, tok);
		}
	}
	else if (chr == '!')
	{
		getch();
		if (chr == '=') // not-equals comparison
		{
			k = end_token(TokenKind::NE, tok);
			getch();
		}
		else // logical not
		{
			k = end_token(TokenKind::L_NOT, tok);
		}
	}
	else if (chr == '<')
	{
		getch();
		if (chr == '<')
		{
			getch();
			if (chr == '=') // left-shift assign
			{
				k = end_token(TokenKind::LEFT_ASSIGN, tok);
				getch();
			}
			else // left-shift
			{
				k = end_token(TokenKind::LSHIFT, tok);
			}
		}
		else if (chr == '=') // <= comparison
		{
			k = end_token(TokenKind::LE, tok);
			getch();
		}
		else // less-than comparison
		{
			k = end_token(TokenKind::LT, tok);
		}
	}
	else if (chr == '>')
	{
		getch();
		if (chr == '>')
		{
			getch();
			if (chr == '=') // right-shift assign
			{
				k = end_token(TokenKind::RIGHT_ASSIGN, tok);
				getch();
			}
			else // right-shift
			{
				k = end_token(TokenKind::RSHIFT, tok);
			}
		}
		else if (chr == '=') // >= comparison
		{
			k = end_token(TokenKind::GE, tok);
			getch();
		}
		else // greater-than comparison
		{
			k = end_token(TokenKind::GT, tok);
		}
	}
	else if (chr == EOF)
	{
		k = end_token(TokenKind::END, tok);
	}
	else
	{
		int c = chr;
		k = end_token(static_cast<TokenKind>(c), tok);
		getch();
	}

	return k;
}

// namespace Pop
}
