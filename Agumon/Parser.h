#pragma once

#include "Scanner.h"

namespace Agumon
{
	class AST
	{
	public:
		AST(Token token) : token_(token) { ; }
		AST(Token token, std::vector<AST> childrenList) : token_(token), childrenList_(childrenList) { ; }
		AST(Token token, std::vector<Token> tokenList) : token_(token)
		{
			for (auto t : tokenList)
			{
				childrenList_.push_back(AST(t));
			}
		}

	public:
		inline Token					token() { return token_; }
		inline TokenType				type() { return token_.type(); }
		inline std::string				value() { return token_.value(); }
		inline std::vector<AST>			childrenList() { return childrenList_; }
		inline void						addChildren(Token token) { childrenList_.push_back(AST(token)); }
		inline void						addChildren(AST ast) { childrenList_.push_back(ast); }

	private:
		Token							token_;
		std::vector<AST>				childrenList_;
	};

	class Parser
	{
	public:
		Parser(std::string text);

	public:
		AST				node();
		AST				assignNode();
		AST				expNode();
		AST				expNode3();
		AST				expNode2();
		AST				expNode1();
		AST				termNode();

	public:
		bool			isTypeSign();
		bool			isOperator();
		bool			isEndOfList();

	public:
		Token			getToken();
		Token			peekToken();
		bool			skipToken(TokenType type);

	private:
		std::vector<Token>					tokenList_;
		std::vector<Token>::iterator		iter_;
	};

	inline bool Parser::isTypeSign()
	{
		return peekToken().type() == TokenType::INT_SIGN ||
			peekToken().type() == TokenType::DOUBLE_SIGN ||
			peekToken().type() == TokenType::BOOL_SIGN;
	}

	inline bool Parser::isOperator()
	{
		return (peekToken().type() == TokenType::GEATER_THAN || peekToken().type() == TokenType::GEATER_THAN_OR_EQUAL
			|| peekToken().type() == TokenType::LESS_THAN || peekToken().type() == TokenType::LESS_THAN_OR_EQUAL);
	}

	inline bool Parser::isEndOfList() 
	{
		return iter_ == tokenList_.end(); 
	}

	inline AST Parser::expNode()
	{
		return expNode3();
	}

	inline Token Parser::getToken()
	{
		return *(iter_++);
	}

	inline Token Parser::peekToken() 
	{ 
		return !isEndOfList() ? *iter_ : Token(TokenType::INVAILD); 
	}

	inline bool Parser::skipToken(TokenType type)
	{
		if (peekToken().type() == type)
		{
			getToken();
			return true;
		}
		else
		{
			return false;
		}
	}
}

