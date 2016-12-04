#include "stdafx.h"
#include "CppUnitTest.h"

#include "Scanner.h"
#include "Dictionary.h"

#include <map>
#include <vector>
#include <algorithm>
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Agumon;

namespace TestAgumon
{		
	TEST_CLASS(UnitTest1)
	{

	public:


	public:
		
		TEST_METHOD(TestTokenType)
		{
			TokenType tokenType = TokenType::INTEGER;
			tokenType = TokenType::INT;
			tokenType = TokenType::ASSIGN;
			tokenType = TokenType::SEMICOLON;
		}

		TEST_METHOD(TestToken)
		{
			Token token = Token(TokenType::INTEGER, 1);
			Assert::IsTrue(token.type() == TokenType::INTEGER, L"get token integer:1 type ");
			Assert::IsTrue(token.value() == "1", L"get token integer:1 value");

			token = Token(TokenType::DECIMAL, 1.5);
			Assert::IsTrue(token.type() == TokenType::DECIMAL, L"get token decimal:1.5 type ");
			Assert::IsTrue(token.value() == "1.500000", L"get token decimal:1.5 value");

			token = Token(TokenType::INT);
			Assert::IsTrue(token.type() == TokenType::INT, L"get token int type");
			Assert::IsTrue(token.value() == "", L"get token int value");

			token = Token(TokenType::ASSIGN);
			Assert::IsTrue(token.type() == TokenType::ASSIGN, L"get token assign type");
			Assert::IsTrue(token.value() == "", L"get token assign value");

		}

		TEST_METHOD(TestDictionary)
		{
			Dictionary dictionary;
			Assert::IsTrue(dictionary.find("int"), L"find token:int in dictionary");
			Assert::IsFalse(dictionary.find("aaa"), L"find invalid:aaa in dictionary");
			Assert::IsTrue(dictionary.find(';'), L"find token:semicolon of char in dictionary");

			Assert::IsTrue(dictionary.token("int").type() == TokenType::INT, L"get key token:int");
			Assert::IsTrue(dictionary.token(";").type() == TokenType::SEMICOLON, L"get key token:semicolon");
			Assert::IsTrue(dictionary.token("aaa").type() == TokenType::INVAILD, L"error token");

		}

		TEST_METHOD(TestScanner)
		{
			Scanner scanner = Scanner(std::string("1"));
			Assert::IsTrue(scanner.text() == "1", L"scanner text");

			// get char
			scanner = Scanner("1+2");
			Assert::AreEqual(scanner.getChar(), '1', L"Get first char");
			Assert::AreEqual(scanner.getChar(), '+', L"Get second char");
			Assert::AreEqual(scanner.getChar(), '2', L"Get last char");
			Assert::AreEqual(scanner.getChar(), '2', L"End of expression");

			// peek char
			scanner = Scanner("2+1");
			Assert::AreEqual(scanner.peekChar(), '2', L"Peek first char");
			scanner.getChar();
			Assert::AreEqual(scanner.peekChar(), '+', L"Peek second char");
			scanner.getChar();
			Assert::AreEqual(scanner.peekChar(), '1', L"peek end of expression");

			// get token
			scanner = Scanner("1");
			auto token = scanner.getToken();
			Assert::IsTrue(token.value() == "1", L"get token integer:1 value");
			Assert::IsTrue(token.type() == TokenType::INTEGER, L"get token integer:1 type");

			scanner = Scanner("50");
			token = scanner.getToken();
			Assert::IsTrue(token.value() == "50", L"get token integer:50 value");
			Assert::IsTrue(token.type() == TokenType::INTEGER, L"get token integer:50 type");

			scanner = Scanner("1.5");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::DECIMAL, L"get token decimal:1.5 type");
			Assert::IsTrue(token.value() == "1.500000", L"get token decimal:1.5 value");

			scanner = Scanner("int");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::INT, L"get token int type");

			scanner = Scanner("double");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::DOUBLE, L"get token double type");

			scanner = Scanner("=");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::ASSIGN, L"get token assign type");

			scanner = Scanner(";");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::SEMICOLON, L"get token semicolon type");

			scanner = Scanner("var");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::VARIABLE, L"get token variable type");
			Assert::IsTrue(token.value() == "var", L"get token variable value");



			// skip token
			scanner = Scanner(" int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT, L"skip token:space");

			scanner = Scanner("  int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT, L"skip token:space_2");

			// assignment statement 
			scanner = Scanner("int i = 0;");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT, L"assign[0] : int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::VARIABLE, L"assign[1] : i");
			Assert::IsTrue(scanner.getToken().type() == TokenType::ASSIGN, L"assign[2] : =");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INTEGER, L"assign[3] : 0");
			Assert::IsTrue(scanner.getToken().type() == TokenType::SEMICOLON, L"assign[4] : ;");


		}



		class Node
		{
		public:
			using NodePtr = std::shared_ptr<Node>;

		public:
			Node() = default;
			Node(Token token, std::vector<NodePtr> nodeList) :token_(token), nodeList_(nodeList) { ; }

		public:
			inline bool checkType()
			{
				if (nodeList_[0]->token_.type() == TokenType::INT && nodeList_[2]->token_.type() == TokenType::INTEGER)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

		public:
			Token token_;
			std::vector<NodePtr> nodeList_;
		};


		TEST_METHOD(TestNode)
		{
			Node node = Node(Token(TokenType::ASSIGN), { nullptr, nullptr, nullptr });
			node = Node(Token(TokenType::ASSIGN), { nullptr, nullptr });
		}

		class Parser
		{
		public:
			Parser(std::string text) : scanner_(text)
			{

			}

		public:
			inline Node Tree()
			{
				auto type = Node(scanner_.getToken(), {});
				auto var = Node(scanner_.getToken(), {});
				auto assign = scanner_.getToken();
				auto rhs = Node(scanner_.getToken(), {});

				scanner_.getChar();		// skip semicolon

				return Node(assign, { std::make_shared<Node>(type), std::make_shared<Node>(var), std::make_shared<Node>(rhs) });
			}

		private:
			Scanner scanner_;
		};


		TEST_METHOD(TestParser)
		{
			Parser parser = Parser(std::string("int i = 0;"));
			Node node = parser.Tree();
			Assert::IsTrue(node.token_.type() == TokenType::ASSIGN,						L"syntax tree assign -> =");
			Assert::IsTrue(node.nodeList_[0]->token_.type() == TokenType::INT,			L"syntax tree type -> int");
			Assert::IsTrue(node.nodeList_[1]->token_.type() == TokenType::VARIABLE,		L"syntax tree var -> variable:i");
			Assert::IsTrue(node.nodeList_[2]->token_.type() == TokenType::INTEGER,		L"syntax tree value -> integer:0");

			parser = Parser(std::string("int var = 5;"));
			node = parser.Tree();
			Assert::IsTrue(node.token_.type() == TokenType::ASSIGN,						L"syntax tree value -> =");
			Assert::IsTrue(node.nodeList_[1]->token_.value() == "var",					L"syntax tree var -> variable:var");
			Assert::IsTrue(node.nodeList_[2]->token_.value() == "5",					L"syntax tree value -> integer:5");

			// type check
			parser = Parser(std::string("int var = int;"));
			node = parser.Tree();
			Assert::IsFalse(node.checkType(), L"type checking error");

			parser = Parser(std::string("int var = 5;"));
			node = parser.Tree();
			Assert::IsTrue(node.checkType(), L"type checking successful");
			

		}





	};




}