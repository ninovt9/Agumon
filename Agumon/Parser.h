#pragma once

#include "Scanner.h"
#include "Node.h"

namespace Agumon
{
	class Parser
	{
	public:
		Parser(std::string text) : scanner_(text) { ; }

	public:
		bool						isTypeToken();
		bool						isNumberToken();

	public:
		std::shared_ptr<Node>		node();
		std::shared_ptr<Node>		assignNode();
		std::shared_ptr<Node>		expNode2();
		std::shared_ptr<Node>		expNode1();
		std::shared_ptr<Node>		termNode();

	public:
		std::map<std::string, Token>		symbolTable();

	private:
		Scanner								scanner_;
		std::map<std::string, Token>		symbolTable_;
	};


	inline bool Parser::isTypeToken()
	{
		return scanner_.peekToken().type() == TokenType::INT_SIGN ||
			scanner_.peekToken().type() == TokenType::DOUBLE_SIGN ||
			scanner_.peekToken().type() == TokenType::BOOL_SIGN;
	}

	inline bool Parser::isNumberToken()
	{
		return scanner_.peekToken().type() == TokenType::INTEGER ||
			scanner_.peekToken().type() == TokenType::DECIMAL;
	}

	inline std::map<std::string, Token> Parser::symbolTable()
	{
		return symbolTable_;
	}
}
