#include <pop/pop.hpp>
#include <iostream>

int main()
{
	Pop::Token tok;
	Pop::Lexer lex(std::cin, "<stdin>");
	while (lex.next_token(tok) != Pop::TokenKind::END)
		tok.debug(std::cout);
	return 0;
}
