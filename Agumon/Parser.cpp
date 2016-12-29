#include "stdafx.h"
#include "Parser.h"

using namespace Agumon;


Parser::Parser(std::string text)
{
	tokenList_ = Scanner(text).getTokenList();
	iter_ = tokenList_.begin();
}

AST Parser::node()
{
	if (isTypeSign())
	{
		return assignNode();
	}
	else
	{
		return expNode();
	}
}

AST Parser::assignNode()
{
	auto type = termNode();
	auto var = termNode();
	auto assign = termNode();
	auto rhs = expNode(); // termNode();

	return AST(assign.token(), std::vector<AST>{type, var, rhs});
}

AST Parser::expNode()
{
	return expNode11();
}

AST	Parser::expNode11()
{
	auto first = expNode3();

	if (peekToken().type() == TokenType::COMMA)
	{
		std::vector<AST> childList;

		childList.push_back(first);

		while (skipToken(TokenType::COMMA))
		{
			auto element = termNode();
			childList.push_back(element);
		}

		return AST(Token(TokenType::COMMA), childList);
	}
	else
	{
		return first;
	}
}


AST Parser::expNode3()
{
	auto lhs = expNode2();

	if (isOperator())
	{
		auto op = termNode();
		auto rhs = termNode();

		return AST(op.token(), std::vector<AST>{lhs, rhs});
	}
	else
	{
		return lhs;
	}
}

AST Parser::expNode2()
{
	auto lhs = expNode1();

	if (peekToken().type() == TokenType::PLUS || peekToken().type() == TokenType::MINUS)
	{
		auto op = termNode();
		auto rhs = termNode();

		return AST(op.token(), std::vector<AST>{lhs, rhs});
	}
	else
	{
		return lhs;
	}
}

AST Parser::expNode1()
{
	auto lhs = termNode();

	if (peekToken().type() == TokenType::MUL || peekToken().type() == TokenType::DIV)
	{
		auto op = termNode();
		auto rhs = termNode();
		return AST(op.token(), std::vector<AST>{lhs, rhs});
	}
	else if (skipToken(TokenType::LEFT_SQUARE_BRA))
	{
		auto size = expNode1();
		skipToken(TokenType::RIGHT_SQUARE_BAR);
		return AST(Token(TokenType::ARRAY), { lhs, size });
	}
	else if (skipToken(TokenType::LEFT_PAR))
	{
		auto parameter = expNode11();

		if (parameter.type() != TokenType::COMMA)
		{
			parameter = AST(TokenType::COMMA, { parameter });
		}

		skipToken(TokenType::RIGHT_SQUARE_BAR);
		return AST(Token(TokenType::FUNCTION), { lhs, parameter });
	}
	else
	{
		return lhs;
	}
}

AST Parser::termNode()
{
	if (skipToken(TokenType::LEFT_PAR))
	{
		auto result = expNode2();
		skipToken(TokenType::RIGHT_PAR);
		return result;
	}
	else if (peekToken().type() == TokenType::VARIABLE)
	{
		auto var = getToken();
		if (skipToken(TokenType::POINT))
		{
			auto member = expNode2();
			return AST(Token(TokenType::POINT), { var, member });
		}
		else if (skipToken(TokenType::POINT_TO_STRUCT))
		{
			auto member = expNode2();
			return AST(Token(TokenType::POINT_TO_STRUCT), { var, member });
		}
		else
		{
			return AST(var);
		}
	}
	else
	{
		return AST(getToken());
	}
}