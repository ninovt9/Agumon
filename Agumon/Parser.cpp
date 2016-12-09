#include "stdafx.h"
#include "Parser.h"


using namespace Agumon;

std::shared_ptr<Node> Parser::node()
{
	if (isTypeToken())
	{
		return assignNode();
	}
	else if (isNumberToken())
	{
		return expNode2();
	}
	else
	{
		return nullptr; // error
	}
}

std::shared_ptr<Node> Parser::assignNode()
{
	std::shared_ptr<Node> type = std::make_shared<TypeNode>(TypeNode(scanner_.getToken(), {}));
	std::shared_ptr<Node> var = std::make_shared<VarNode>(VarNode(scanner_.getToken(), {}));
	Token assign = scanner_.getToken();
	std::shared_ptr<Node> rhs = expNode2();

	return std::make_shared<AssignNode>(AssignNode(assign, { var, rhs }));
}


std::shared_ptr<Node> Parser::termNode()
{
	std::shared_ptr<Node> result = nullptr;

	if (scanner_.peekToken().type() == TokenType::LEFT_PAR)
	{
		scanner_.getToken();
		result = expNode2();
		scanner_.getToken();
	}
	else if (scanner_.peekToken().type() == TokenType::PLUS)
	{
		scanner_.getToken();
		result = termNode();
	}
	else if (scanner_.peekToken().type() == TokenType::MINUS)
	{
		Token minus = scanner_.getToken();
		std::shared_ptr<Node> lhs, rhs;
		if (scanner_.peekToken().type() == TokenType::INTEGER)
		{
			lhs = std::make_shared<NumberNode>(NumberNode(Token(TokenType::INTEGER, 0), {}));
		}
		else if (scanner_.peekToken().type() == TokenType::DECIMAL)
		{
			lhs = std::make_shared<NumberNode>(NumberNode(Token(TokenType::DECIMAL, 0.0), {}));
		}

		rhs = termNode();
		result = std::make_shared<AddNode>(AddNode(minus, { lhs,  rhs }));
	}
	else
	{
		result = std::make_shared<NumberNode>(NumberNode(scanner_.getToken(), {}));
	}

	return result;

}


inline std::shared_ptr<Node> Parser::expNode1()
{
	std::shared_ptr<Node> lhs = termNode();

	if (scanner_.peekToken().type() == TokenType::MUL)
	{
		Token mul = scanner_.getToken();
		std::shared_ptr<Node> rhs = std::make_shared<NumberNode>(NumberNode(scanner_.getToken(), {}));
		return std::make_shared<AddNode>(AddNode(mul, { lhs, rhs }));
	}
	else if (scanner_.peekToken().type() == TokenType::DIV)
	{
		Token div = scanner_.getToken();
		std::shared_ptr<Node> rhs = std::make_shared<NumberNode>(NumberNode(scanner_.getToken(), {}));
		return std::make_shared<AddNode>(AddNode(div, { lhs, rhs }));
	}
	else
	{
		return lhs;
	}
}

inline std::shared_ptr<Node> Parser::expNode2()
{
	std::shared_ptr<Node> lhs = expNode1();

	if (scanner_.peekToken().type() == TokenType::PLUS)
	{
		Token plus = scanner_.getToken();
		std::shared_ptr<Node> rhs = std::make_shared<NumberNode>(NumberNode(scanner_.getToken(), {}));
		return std::make_shared<AddNode>(AddNode(plus, { lhs, rhs }));
	}
	else if (scanner_.peekToken().type() == TokenType::MINUS)
	{
		Token minus = scanner_.getToken();
		std::shared_ptr<Node> rhs = std::make_shared<NumberNode>(NumberNode(scanner_.getToken(), {}));
		return std::make_shared<AddNode>(AddNode(minus, { lhs, rhs }));
	}
	else
	{
		return lhs;
	}


}