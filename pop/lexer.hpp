#ifndef POP_LEXER_HPP
#define POP_LEXER_HPP

#include <pop/token.hpp>
#include <cstdint>
#include <istream>

namespace Pop
{

class Lexer
{
public:
	const char *filename;
	int chr;
	TokenKind kind;
	std::uint32_t offset;
	std::uint32_t line;
	std::uint32_t column;
	std::string text;
	std::istream &inp;

	Lexer(std::istream &inp, const char *fn = "<stream>")
	    : filename(fn), chr(' '), offset(std::uint32_t(-1)), line(1),
	      column(std::uint32_t(-1)), inp(inp)
	{
	}

	TokenKind next_token(Token &t);

private:
	int getch();
	void reset();
	void start_token(Token &out_token);
	TokenKind end_token(TokenKind k, Token &out_token);
};

// namespace Pop
}

#endif // POP_LEXER_HPP
