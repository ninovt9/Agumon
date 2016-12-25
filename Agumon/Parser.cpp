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
	auto lhs = termNode(); // AST(getToken());

	if (peekToken().type() == TokenType::MUL || peekToken().type() == TokenType::DIV)
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
		if (skipToken(TokenType::LEFT_SQUARE_BRA))
		{
			auto size = expNode2();		// 最后统一改成expNode()
			skipToken(TokenType::RIGHT_SQUARE_BAR);
			return AST(var, { AST(size) });
		}
		else if (skipToken(TokenType::POINT))
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