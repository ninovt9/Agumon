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
			Scanner scanner = Scanner(std::string(""));
			Token token = Token(TokenType::INVAILD);

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
			scanner = Scanner("50");
			token = scanner.getToken();
			Assert::IsTrue(token.value() == "50",					L"get token integer:50 value");
			Assert::IsTrue(token.type() == TokenType::INTEGER,		L"get token integer:50 type");

			scanner = Scanner("1.5");
			token = scanner.getToken();
			Assert::IsTrue(token.type()	== TokenType::DECIMAL,		L"get token decimal:1.5 type");
			Assert::IsTrue(token.value() == "1.500000",				L"get token decimal:1.5 value");

			scanner = Scanner("var");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::VARIABLE,		L"get token variable type");
			Assert::IsTrue(token.value() == "var",					L"get token variable value");

			scanner = Scanner("int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT,			L"get token int type");

			scanner = Scanner("double");
			Assert::IsTrue(scanner.getToken().type() == TokenType::DOUBLE,		L"get token double type");

			scanner = Scanner("=");
			Assert::IsTrue(scanner.getToken().type() == TokenType::ASSIGN,		L"get token assign sign");

			scanner = Scanner(";");
			Assert::IsTrue(scanner.getToken().type() == TokenType::SEMICOLON,	L"get token semicolon sign");

			scanner = Scanner("+");
			Assert::IsTrue(scanner.getToken().type() == TokenType::PLUS,		L"get token plus sign");

			scanner = Scanner("-");
			Assert::IsTrue(scanner.getToken().type() == TokenType::MINUS,		L"get token minus sign");

			scanner = Scanner("*");
			Assert::IsTrue(scanner.getToken().type() == TokenType::MUL,			L"get token mul sign");

			scanner = Scanner("/");
			Assert::IsTrue(scanner.getToken().type() == TokenType::DIV,			L"get token div sign");

			// skip token
			scanner = Scanner(" int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT,			L"skip token space ");

			// peek token
			scanner = Scanner("int");
			Assert::IsTrue(scanner.peekToken().type() == TokenType::INT);

			scanner = Scanner("=");
			Assert::IsTrue(scanner.peekToken().type() == TokenType::ASSIGN);

			//scanner = Scanner("int i = 0;");
			//Assert::IsTrue(scanner.peekToken().type() == TokenType::INT);
			//scanner.getToken();
			//Assert::IsTrue(scanner.peekToken().type() == TokenType::VARIABLE);

			// statement 
			scanner = Scanner("int i = 0;");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT,			L"assign[0] : int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::VARIABLE,	L"assign[1] : i");
			Assert::IsTrue(scanner.getToken().type() == TokenType::ASSIGN,		L"assign[2] : =");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INTEGER,		L"assign[3] : 0");
			Assert::IsTrue(scanner.getToken().type() == TokenType::SEMICOLON,	L"assign[4] : ;");
		}

		TEST_METHOD(TestScanner_PeekToken)
		{
			auto scanner = Scanner("int i = 0;");
			Assert::IsTrue(scanner.peekToken().type() == TokenType::INT,		L"int");
			scanner.getToken();
			Assert::IsTrue(scanner.peekToken().type() == TokenType::VARIABLE,	L"i");
			scanner.getToken();
			Assert::IsTrue(scanner.peekToken().type() == TokenType::ASSIGN,		L"=");
		}

		class Node
		{
		public:
			using NodePtr = std::shared_ptr<Node>;

		public:
			Node() = default;
			Node(Token token, std::vector<NodePtr> nodeList) :token_(token), nodeList_(nodeList) { ; }

		public:

			inline virtual TokenType checkType() = 0;
			inline virtual void walk(std::map<std::string, Token>&)
			{
				;
			}

		public:
			Token token_;
			std::vector<NodePtr> nodeList_;
		};

		class NumberNode : public Node
		{
		public:
			NumberNode() = default;
			NumberNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

		public:
			inline TokenType checkType()
			{
				return token_.type() == TokenType::INTEGER ? TokenType::INT : TokenType::DOUBLE;
			}
		};

		class TypeNode : public Node
		{
		public:
			TypeNode() = default;
			TypeNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

		public:
			inline TokenType checkType()
			{
				return token_.type();
			}
		};

		class VarNode : public Node
		{
		public:
			VarNode() = default;
			VarNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }

		public:
			inline TokenType checkType()
			{
				return token_.type();
			}
		};

		class AssignNode : public Node
		{
		public:
			AssignNode() = default;
			AssignNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }
		public:
			inline TokenType checkType()
			{
				return token_.type();
			}
			inline void walk(std::map<std::string, Token>& symbolTable)
			{
				symbolTable.insert({ nodeList_[0]->token_.value(), nodeList_[1]->token_ });
			}
		};

		class AddNode : public Node
		{
		public:
			AddNode() = default;
			AddNode(Token token, std::vector<NodePtr> nodeList) : Node(token, nodeList) { ; }
		public:
			inline TokenType checkType()
			{
				return token_.type();
			}
			inline void walk(std::map<std::string, Token>& symbolTable)
			{
				;
			}
		};


		TEST_METHOD(TestNode)
		{
			std::shared_ptr<Node> integerNode = std::make_shared<NumberNode>(NumberNode(Token(TokenType::INTEGER, 1), {}));
			Assert::IsTrue(integerNode->checkType() == TokenType::INT, L"check type for number:integer");

			std::shared_ptr<Node> decimalNode = std::make_shared<NumberNode>(NumberNode(Token(TokenType::DECIMAL, 1), {}));
			Assert::IsTrue(decimalNode->checkType() == TokenType::DOUBLE, L"check type for number:decimal");

			std::shared_ptr<Node> intNode = std::make_shared<TypeNode>(TypeNode(Token(TokenType::INT), {}));
			Assert::IsTrue(intNode->checkType() == TokenType::INT, L"check type for type:int");

			std::shared_ptr<Node> doubleNode = std::make_shared<TypeNode>(TypeNode(Token(TokenType::DOUBLE), {}));
			Assert::IsTrue(doubleNode->checkType() == TokenType::DOUBLE, L"check type for type:double");

			std::shared_ptr<Node> varNode = std::make_shared<VarNode>(VarNode(Token(TokenType::VARIABLE), {}));
			Assert::IsTrue(varNode->checkType() == TokenType::VARIABLE, L"check type for variable");
		}


		class Parser
		{
		public:
			Parser(std::string text) : scanner_(text)
			{

			}
		public:

			inline bool isTypeToken()
			{
				return scanner_.peekToken().type() == TokenType::INT ||
					scanner_.peekToken().type() == TokenType::DOUBLE;
			}

			inline bool isNumberToken()
			{
				return scanner_.peekToken().type() == TokenType::INTEGER || 
					scanner_.peekToken().type() == TokenType::DECIMAL;
			}

			inline std::shared_ptr<Node> node()
			{
				if(isTypeToken())
				{
					return assignNode();
				}
				else if(isNumberToken())
				{
					return expNode();
				}
				else
				{
					return nullptr; // error
				}
			}

			inline std::shared_ptr<Node> assignNode()
			{
				std::shared_ptr<Node> type = std::make_shared<TypeNode>(TypeNode(scanner_.getToken(), {}));
				std::shared_ptr<Node> var = std::make_shared<VarNode>(VarNode(scanner_.getToken(), {}));
				Token assign = scanner_.getToken();
				std::shared_ptr<Node> rhs = expNode();

				return std::make_shared<AssignNode>(AssignNode(assign, { var, rhs }));
			}

			inline std::shared_ptr<Node> exp1Node()
			{
				std::shared_ptr<Node> lhs = std::make_shared<NumberNode>(NumberNode(scanner_.getToken(), {}));

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

			inline std::shared_ptr<Node> expNode()
			{
				std::shared_ptr<Node> lhs = exp1Node();

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

			inline std::map<std::string, Token> symbolTable()
			{
				return symbolTable_;
			}

		private:
			Scanner scanner_;
			std::map<std::string, Token> symbolTable_;
		};

		TEST_METHOD(TestParser_AssignStatForInt)
		{
			auto parser = Parser(std::string("int i = 0;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"syntax tree assign -> =");
			Assert::IsTrue(node->nodeList_[0]->token_.type() == TokenType::VARIABLE, L"syntax tree var -> variable:i");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::INTEGER, L"syntax tree value -> integer:0");
		}

		TEST_METHOD(TestParser_DefinedVariable)
		{
			auto parser = Parser(std::string("int i = 0;"));
			auto node = parser.node();
			auto symbolTable = parser.symbolTable();
			node->walk(symbolTable);
			Assert::IsTrue(symbolTable.find("i") != symbolTable.end(),					L"var.name : i");
		}

		TEST_METHOD(TestParser_AddStat)
		{
			auto parser = Parser(std::string("1 + 1"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::PLUS,						L"value : +");
			Assert::IsTrue(node->nodeList_[0]->token_.type() == TokenType::INTEGER,		L"lhs : 1");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::INTEGER,		L"rhs : 1");
		}


		TEST_METHOD(TestParser_AssignStatForDouble)
		{
			auto parser = Parser(std::string("double var = 5.0;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN,					L"value : =");
			Assert::IsTrue(node->nodeList_[0]->token_.type() == TokenType::VARIABLE,	L"type : double");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::DECIMAL,		L"rhs : integer:0");
		}

		TEST_METHOD(TestParser_AssignStatForAdd)
		{
			auto parser = Parser(std::string("int var = 1 + 2;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN,					L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::PLUS,		L"rhs.value : +");
		}

		TEST_METHOD(TestParser_AssignStatForSub)
		{
			auto parser = Parser(std::string("int var = 1 - 2;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::MINUS,		L"rhs.value : -");
		}

		TEST_METHOD(TestParser_AssignStatForMul)
		{
			auto parser = Parser(std::string("int var = 1 * 2;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::MUL, L"rhs.value : *");
		}

		TEST_METHOD(TestParser_AssignStatForDiv)
		{
			auto parser = Parser(std::string("int var = 1 / 2;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::DIV, L"rhs.value : /");
		}

		TEST_METHOD(TestParser_AssignStatForAddAndSub)
		{
			auto parser = Parser(std::string("int var = 1 * 2 + 3;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::PLUS, L"rhs.value : +");
		}

		TEST_METHOD(TestParser_AssignStatForAddAndDiv)
		{
			auto parser = Parser(std::string("int var = 1 + 2 / 3;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::PLUS, L"rhs.value : +");
		}
	};




}