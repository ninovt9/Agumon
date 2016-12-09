#pragma once


#include "Scanner.h"

#include <memory>

namespace Agumon
{
	class Node
	{
	public:
		using NodePtr = std::shared_ptr<Node>;

	public:
		Node() = default;
		Node(Token token, std::vector<NodePtr> nodeList) :token_(token), nodeList_(nodeList) { ; }

	public:
		virtual TokenType						checkType() = 0;
		virtual void							walk(std::map<std::string, Token>&) = 0;

	public:
		Token									token_;
		std::vector<NodePtr>					nodeList_;
	};


	class NumberNode : public Node
	{
	public:
		NumberNode() = default;
		NumberNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

	public:
		TokenType								checkType();
		inline void								walk(std::map<std::string, Token>& symbolTable) { ; }
	};


	class TypeNode : public Node
	{
	public:
		TypeNode() = default;
		TypeNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

	public:
		TokenType								checkType();
		inline void							    walk(std::map<std::string, Token>& symbolTable) { ; }
	};


	class VarNode : public Node
	{
	public:
		VarNode() = default;
		VarNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

	public:
		TokenType								checkType();
		inline void								walk(std::map<std::string, Token>& symbolTable) { ; }
	};


	class AssignNode : public Node
	{
	public:
		AssignNode() = default;
		AssignNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }
	public:
		TokenType								checkType();
		void									walk(std::map<std::string, Token>& symbolTable);
	};




	class AddNode : public Node
	{
	public:
		AddNode() = default;
		AddNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }
	public:
		TokenType								checkType();
		inline void								walk(std::map<std::string, Token>& symbolTable) { ; }
	};



	inline TokenType NumberNode::checkType()
	{
		return token_.type() == TokenType::INTEGER ? TokenType::INT_SIGN : TokenType::DOUBLE_SIGN;
	}

	inline TokenType TypeNode::checkType()
	{
		return token_.type();
	}

	inline TokenType VarNode::checkType()
	{
		return token_.type();
	}

	inline TokenType AssignNode::checkType()
	{
		return token_.type();
	}

	inline void AssignNode::walk(std::map<std::string, Token>& symbolTable)
	{
		symbolTable.insert({ nodeList_[0]->token_.value(), nodeList_[1]->token_ });
	}

	inline TokenType AddNode::checkType()
	{
		return token_.type();
	}


}

