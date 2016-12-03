#include "stdafx.h"
#include "CppUnitTest.h"

#include <map>
#include <vector>
#include <algorithm>
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestAgumon
{		


	TEST_CLASS(UnitTest1)
	{

	public:
		enum class TokenType
		{
			INTEGER,

			INT,
			ASSIGN,
			SEMICOLON,
			VARIABLE,
		};

		class Token
		{
		public:
			Token(TokenType type, int value)				: type_(type), value_(std::to_string(value)) { }
			Token(TokenType type, std::string value = "")	: type_(type), value_(value) { }

		public:
			inline TokenType type(){ return type_; }
			inline std::string value() { return value_; }

		private:
			TokenType type_;
			std::string value_;
		};

		class Scanner
		{
		public:
			Scanner(std::string text) : text_(text), index_(0) { }

		public:
			inline Token getToken()
			{
				
				std::vector<char> skipList{ ' ' };
				while (std::find(skipList.begin(), skipList.end(), peekChar()) != skipList.end())
				{
					getChar();
				}


				auto firstChar = peekChar();

				if (isdigit(firstChar))
				{
					return getNumberToken();
				}
				else if (firstChar == '=')
				{
					getChar();
					return Token(TokenType::ASSIGN);
				}
				else if (firstChar == ';')
				{
					getChar();
					return Token(TokenType::SEMICOLON);
				}
				else if (isalpha(firstChar))
				{
					return getIdentifierToken();
				}
				else
				{
					; // error
				}
			}

			inline Token getIdentifierToken()
			{
				std::string value;
				while (isalpha(peekChar()) && !isEndOfExp())
				{
					value.push_back(getChar());
				}

				if (value == "int")
				{
					return Token(TokenType::INT);
				}
				else
				{
					return Token(TokenType::VARIABLE, value);
				}
			}

			inline Token getNumberToken()
			{
				std::string value;
				while (isdigit(peekChar()) && !isEndOfExp())
				{
					value.push_back(getChar());
				}
				return Token(TokenType::INTEGER, toInt(value));
			}

			inline int toInt(std::string str)
			{
				std::stringstream stream(str);
				int value;
				stream >> value;
				return value;
			}
			inline char Scanner::getChar()
			{ 
				return isEndOfExp() ? text_[text_.size() - 1] : text_[index_++]; 
			}
			inline char peekChar() 
			{ 
				return isEndOfExp() ? text_[text_.size() - 1] : text_[index_];
			}
			inline std::string text() 
			{ 
				return text_; 
			}
			inline bool isEndOfExp()
			{
				return index_ >= text_.size() ? true : false;
			}

		private:
			std::string text_;
			size_t index_;
		};


		class Dictionary
		{
		public:
			Dictionary()
			{
				map_.insert(std::pair<std::string, Token>("int", Token(TokenType::INT)));
				map_.insert(std::pair<std::string, Token>(";", Token(TokenType::SEMICOLON)));
			}
		public:
			inline bool find(std::string key)
			{
				return map_.find(key) != map_.end();
			}
			inline bool find(char key)
			{
				std::string keyStr;
				keyStr.push_back(key);
				return find(keyStr);
			}
		private:
			std::map<std::string, Token> map_;
		};

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

			scanner = Scanner("int");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::INT, L"get token int type");

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
			Node(Token token, std::shared_ptr<Node> leftNode, std::shared_ptr<Node> rightNode)
				:token_(token), leftNode_(leftNode), rightNode_(rightNode)
			{

			}

		public:
			Token token_;
			std::shared_ptr<Node> leftNode_;
			std::shared_ptr<Node> rightNode_;

		};

		class Parser
		{
		public:
			Parser(std::string text) : scanner_(text)
			{

			}
		public:
			inline Node Tree()
			{
				scanner_.getToken();		// skip int
				auto lhs = scanner_.getToken();	// variable
				auto leftNode = new Node(lhs, nullptr, nullptr);

				scanner_.getToken();		// skip =

				auto rhs = scanner_.getToken();	// integer
				auto rightNode = new Node(rhs, nullptr, nullptr);
				return Node(Token(TokenType::ASSIGN), std::shared_ptr<Node>(leftNode), std::shared_ptr<Node>(rightNode));
			}

		private:
			Scanner scanner_;
		};

		TEST_METHOD(TestParser)
		{
			Parser parser = Parser(std::string("int i = 0;"));
			Node node = parser.Tree();
			Assert::IsTrue(node.token_.type() == TokenType::ASSIGN, L"syntax tree value -> =");
			Assert::IsTrue(node.leftNode_->token_.type() == TokenType::VARIABLE, L"syntax tree left -> variable:i");
			Assert::IsTrue(node.rightNode_->token_.type() == TokenType::INTEGER, L"syntax tree right -> integer:0");

			parser = Parser(std::string("int var = 5;"));
			node = parser.Tree();
			Assert::IsTrue(node.token_.type() == TokenType::ASSIGN, L"syntax tree value -> =");
			Assert::IsTrue(node.leftNode_->token_.type() == TokenType::VARIABLE, L"syntax tree left -> variable:var");
			Assert::IsTrue(node.rightNode_->token_.type() == TokenType::INTEGER, L"syntax tree right -> integer:5");
			Assert::IsTrue(node.leftNode_->token_.value() == std::string("var"), L"syntax tree left value -> variable:var");
			Assert::IsTrue(node.rightNode_->token_.value() == std::string("5"),  L"syntax tree right value -> integer:5");

		}

		TEST_METHOD(TestNode)
		{
			Node node = Node(Token(TokenType::ASSIGN), nullptr, nullptr);
		}


	};




}