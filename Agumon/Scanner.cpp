#include "stdafx.h"
#include "Scanner.h"


using namespace Agumon;

Token Scanner::getToken()
{

	std::vector<char> skipList{ ' ' };
	while (std::find(skipList.begin(), skipList.end(), peekChar()) != skipList.end())
	{
		getChar();
	}


	auto firstChar = peekChar();

	if (isdigit(firstChar))
	{
		return getNumberToken();
	}
	else if (firstChar == '=')
	{
		getChar();
		return Token(TokenType::ASSIGN);
	}
	else if (firstChar == ';')
	{
		getChar();
		return Token(TokenType::SEMICOLON);
	}
	else if (isalpha(firstChar))
	{
		return getIdentifierToken();
	}
	else
	{
		; // error
	}
}
