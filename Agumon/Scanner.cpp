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

	auto test = peekChar(2);

	if (isdigit(peekChar()))  // isdigit(firstChar))
	{
		return getNumberToken();
	}
	else if (isalpha(peekChar()))  // isalpha(firstChar))
	{
		return getIdentifierToken();
	}
	else if (dict_.find(firstChar) || dict_.find(peekChar(2)))
	{
		return getSignToken();
	}
	else
	{
		return Token(TokenType::INVAILD); // error
	}
}
