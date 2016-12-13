#include "stdafx.h"
#include "CppUnitTest.h"

#include "Scanner.h"
#include "Dictionary.h"
#include "Node.h"
#include "Parser.h"

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
			tokenType = TokenType::INT_SIGN;
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

			token = Token(TokenType::INT_SIGN);
			Assert::IsTrue(token.type() == TokenType::INT_SIGN, L"get token int type");
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

			Assert::IsTrue(dictionary.token("int").type() == TokenType::INT_SIGN, L"get key token:int");
			Assert::IsTrue(dictionary.token(";").type() == TokenType::SEMICOLON, L"get key token:semicolon");
			Assert::IsTrue(dictionary.token("aaa").type() == TokenType::INVAILD, L"error token");
			Assert::IsTrue(dictionary.token("||").type() == TokenType::OR, L"get key token:or");

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
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT_SIGN,			L"get token int type");

			scanner = Scanner("double");
			Assert::IsTrue(scanner.getToken().type() == TokenType::DOUBLE_SIGN,		L"get token double type");

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

			scanner = Scanner("(");
			Assert::IsTrue(scanner.getToken().type() == TokenType::LEFT_PAR,	L"get token left parenthesis sign");

			scanner = Scanner(")");
			Assert::IsTrue(scanner.getToken().type() == TokenType::RIGHT_PAR,	L"get token right parenthesis sign");

			scanner = Scanner("bool");
			Assert::IsTrue(scanner.getToken().type() == TokenType::BOOL_SIGN,	L"get token bool sign");

			scanner = Scanner("true");
			Assert::IsTrue(scanner.getToken().type() == TokenType::TRUE,		L"get token true");

			scanner = Scanner("false");
			Assert::IsTrue(scanner.getToken().type() == TokenType::FALSE,		L"get token false");

			scanner = Scanner("||");
			Assert::IsTrue(scanner.getToken().type() == TokenType::OR,			L"get token or");

			// skip token
			scanner = Scanner(" int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT_SIGN,			L"skip token space ");

			// statement 
			scanner = Scanner("int i = 0;");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT_SIGN,			L"assign[0] : int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::VARIABLE,	L"assign[1] : i");
			Assert::IsTrue(scanner.getToken().type() == TokenType::ASSIGN,		L"assign[2] : =");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INTEGER,		L"assign[3] : 0");
			Assert::IsTrue(scanner.getToken().type() == TokenType::SEMICOLON,	L"assign[4] : ;");
		}

		TEST_METHOD(TestScanner_PeekChar)
		{
			Scanner scanner = Scanner("2+1");
			Assert::AreEqual(scanner.peekChar(), '2', L"Peek first char");
			scanner.getChar();
			Assert::AreEqual(scanner.peekChar(), '+', L"Peek second char");
			scanner.getChar();
			Assert::AreEqual(scanner.peekChar(), '1', L"peek end of expression");
		}

		TEST_METHOD(TestScanner_PeekChar_Multiple)
		{
			Scanner scanner = Scanner("2+");
			Assert::IsTrue(scanner.peekChar(2) == "2+", L"peek two char");

			scanner = Scanner("");
			Assert::IsTrue(scanner.peekChar(2) == "", L"end of expression");

			scanner = Scanner("2");
			Assert::IsTrue(scanner.peekChar(2) == "", L"out of range:2");
			Assert::IsTrue(scanner.peekChar(1) == "2", L"out of range:1");
		}

		TEST_METHOD(TestScanner_PeekToken)
		{
			auto scanner = Scanner("int i = 0;");
			Assert::IsTrue(scanner.peekToken().type() == TokenType::INT_SIGN,		L"int");
			scanner.getToken();
			Assert::IsTrue(scanner.peekToken().type() == TokenType::VARIABLE,	L"i");
			scanner.getToken();
			Assert::IsTrue(scanner.peekToken().type() == TokenType::ASSIGN,		L"=");
		}

		TEST_METHOD(TestNode)
		{
			std::shared_ptr<Node> integerNode = std::make_shared<NumberNode>(NumberNode(Token(TokenType::INTEGER, 1), {}));
			std::shared_ptr<Node> decimalNode = std::make_shared<NumberNode>(NumberNode(Token(TokenType::DECIMAL, 1), {}));
			std::shared_ptr<Node> intNode = std::make_shared<TypeNode>(TypeNode(Token(TokenType::INT_SIGN), {}));
			std::shared_ptr<Node> doubleNode = std::make_shared<TypeNode>(TypeNode(Token(TokenType::DOUBLE_SIGN), {}));
			std::shared_ptr<Node> varNode = std::make_shared<VarNode>(VarNode(Token(TokenType::VARIABLE), {}));
			std::shared_ptr<Node> boolNode = std::make_shared<BoolNode>(BoolNode(Token(TokenType::BOOL_SIGN), {}));
		}

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
			Assert::IsTrue(node->nodeList_[0]->token_.type() == TokenType::VARIABLE,	L"type : var");
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

		TEST_METHOD(TestParser_AssignStatForParenthesis)
		{
			auto parser = Parser(std::string("int var = (1 + 2) * 3;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::MUL, L"rhs.value : *");
		}

		TEST_METHOD(TestParser_AssignStatForPositiveNumber)
		{
			auto parser = Parser(std::string("int var = +1;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::INTEGER, L"rhs.value : integer:1");
		}

		TEST_METHOD(TestParser_AssignStatForNegativeNumber)
		{
			auto parser = Parser(std::string("int var = -1;"));
			auto node = parser.node();
			Assert::IsTrue(node->token_.type() == TokenType::ASSIGN, L"value : =");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::MINUS, L"rhs.type : -");
		}

		TEST_METHOD(TestParser_AssignStatForPositiveNumberAndParenthesis)
		{
			auto parser = Parser(std::string("int var = (+1);"));
			auto node = parser.node();
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::INTEGER, L"rhs.type : integer:1");

			parser = Parser(std::string("int var = -(+1);"));
			node = parser.node();
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::MINUS, L"rhs.type : -");
		}

		TEST_METHOD(TestParser_AssignStatForNegativeNumberAndParenthesis)
		{
			auto parser = Parser(std::string("int var = -(-1);"));
			auto node = parser.node();
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::MINUS, L"rhs.type : -");
		}

		TEST_METHOD(TestParser_AssignStatForBoolean)
		{
			auto parser = Parser(std::string("bool b = true;"));
			auto node = parser.node();
			Assert::IsTrue(node->nodeList_[0]->token_.type() == TokenType::VARIABLE, L"rhs.type : true");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::TRUE, L"rhs.type : true");
		}

		TEST_METHOD(TestParser_AssignStatForOr)
		{
			auto parser = Parser(std::string("bool o = true || false;"));
			auto node = parser.node();
			Assert::IsTrue(node->nodeList_[0]->token_.type() == TokenType::VARIABLE, L"lhs.type : var:o");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::OR, L"rhs.type : or");

			node = node->nodeList_[1];
			Assert::IsTrue(node->nodeList_[0]->token_.type() == TokenType::TRUE , L"lhs.type : true");
			Assert::IsTrue(node->nodeList_[1]->token_.type() == TokenType::FALSE, L"lhs.type : false");
		}

		TEST_METHOD(TestParser_AssignStatForSyntaxErrorMissIden)
		{
			auto parser = Parser(std::string("int = 0;"));
			auto node = parser.node();
			Assert::IsTrue(parser.isError(), L"in the errorlist");
			Assert::IsTrue(parser.error("SyntaxError: missing identifier"), L"msg matches");
		}

		TEST_METHOD(TestParser_AssignStatForSyntaxErrorTypeMissmatch)
		{
			auto parser = Parser(std::string("int i = true;"));
			auto node = parser.node();
			Assert::IsTrue(parser.isError(), L"int : bool");

			parser = Parser(std::string("double i = true;"));
			node = parser.node();
			Assert::IsTrue(parser.isError(), L"double : bool");

			parser = Parser(std::string("int i = 1 + 2"));
			node = parser.node();
			Assert::IsFalse(parser.isError(), L"int : add)");

		}
	};
}