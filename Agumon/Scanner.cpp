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
	else if (isalpha(firstChar))
	{
		return getIdentifierToken();
	}
	else if (dict_.find(firstChar))
	{
		return getSignToken();
	}

	else
	{
		; // error
	}
}
