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
		inline void								walk(std::map<std::string, Token>& symbolTable) { ; }
	};


	class TypeNode : public Node
	{
	public:
		TypeNode() = default;
		TypeNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

	public:
		inline void							    walk(std::map<std::string, Token>& symbolTable) { ; }
	};


	class VarNode : public Node
	{
	public:
		VarNode() = default;
		VarNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

	public:
		inline void								walk(std::map<std::string, Token>& symbolTable) { ; }
	};


	class AssignNode : public Node
	{
	public:
		AssignNode() = default;
		AssignNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }
	public:
		void									walk(std::map<std::string, Token>& symbolTable);
	};


	class AddNode : public Node
	{
	public:
		AddNode() = default;
		AddNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }
	public:
		inline void								walk(std::map<std::string, Token>& symbolTable) { ; }
	};

	class BoolNode : public Node
	{
	public:
		BoolNode() = default;
		BoolNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }
	public:
		inline void								walk(std::map<std::string, Token>& symbolTable) { ; }

	};

	inline void AssignNode::walk(std::map<std::string, Token>& symbolTable)
	{
		symbolTable.insert({ nodeList_[0]->token_.value(), nodeList_[1]->token_ });
	}


}

