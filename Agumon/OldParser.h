#pragma once

#include "Scanner.h"
#include "Node.h"

#include <algorithm>

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
		inline bool								isError()
		{
			return errorList_.size() > 0 ? true : false;
		}
		inline bool error(std::string msg)
		{
			return std::find(errorList_.begin(), errorList_.end(), msg) != errorList_.end();
		}

	private:
		Scanner								scanner_;
		std::map<std::string, Token>		symbolTable_;
		std::vector<std::string>			errorList_;
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
