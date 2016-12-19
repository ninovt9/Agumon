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
		Parser(std::string text)
		{
			tokenList_ = Scanner(text).getTokenList();
			iter_ = tokenList_.begin();
		}

	public:
		AST node()
		{
			if (isTypeSign())
			{
				return assignNode();
			}
			else
			{
				return expNode3();
			}
		}

		inline bool isTypeSign()
		{
			return peekToken().type() == TokenType::INT_SIGN ||
				peekToken().type() == TokenType::DOUBLE_SIGN ||
				peekToken().type() == TokenType::BOOL_SIGN;
		}

		inline AST assignNode()
		{
			auto type = termNode();
			auto var = termNode();
			auto assign = termNode();
			auto rhs = termNode();

			return AST(assign.token(), std::vector<AST>{type, var, rhs});
		}

		inline AST expNode3()
		{
			auto lhs = expNode2();

			if (peekToken().type() == TokenType::GEATER_THAN || peekToken().type() == TokenType::GEATER_THAN_OR_EQUAL
				|| peekToken().type() == TokenType::LESS_THAN || peekToken().type() == TokenType::LESS_THAN_OR_EQUAL)
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

		inline AST expNode2()
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


		inline AST expNode1()
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

		inline AST termNode()
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

	public:
		inline Token getToken() { return *(iter_++); }
		inline Token peekToken() { return !isEndOfList() ? *iter_ : Token(TokenType::INVAILD); }
		inline bool isEndOfList() { return iter_ == tokenList_.end(); }

		inline bool skipToken(TokenType type)
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

	private:
		std::vector<Token> tokenList_;
		std::vector<Token>::iterator iter_;
	};
}

