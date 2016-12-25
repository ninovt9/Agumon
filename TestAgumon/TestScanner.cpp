#include "stdafx.h"
#include "CppUnitTest.h"

#include "Scanner.h"
#include "Dictionary.h"

#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Agumon;

namespace TestAgumon
{
	TEST_CLASS(Test_Scanner)
	{
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

		TEST_METHOD(TestScanner_Main)
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
			Assert::IsTrue(token.value() == "50", L"get token integer:50 value");
			Assert::IsTrue(token.type() == TokenType::INTEGER, L"get token integer:50 type");

			scanner = Scanner("1.5");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::DECIMAL, L"get token decimal:1.5 type");
			Assert::IsTrue(token.value() == "1.500000", L"get token decimal:1.5 value");

			scanner = Scanner("var");
			token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::VARIABLE, L"get token variable type");
			Assert::IsTrue(token.value() == "var", L"get token variable value");

			scanner = Scanner("int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT_SIGN, L"get token int type");

			scanner = Scanner("double");
			Assert::IsTrue(scanner.getToken().type() == TokenType::DOUBLE_SIGN, L"get token double type");

			scanner = Scanner("=");
			Assert::IsTrue(scanner.getToken().type() == TokenType::ASSIGN, L"get token assign sign");

			scanner = Scanner(";");
			Assert::IsTrue(scanner.getToken().type() == TokenType::SEMICOLON, L"get token semicolon sign");

			scanner = Scanner("(");
			Assert::IsTrue(scanner.getToken().type() == TokenType::LEFT_PAR, L"get token left parenthesis sign");

			scanner = Scanner(")");
			Assert::IsTrue(scanner.getToken().type() == TokenType::RIGHT_PAR, L"get token right parenthesis sign");

			scanner = Scanner("bool");
			Assert::IsTrue(scanner.getToken().type() == TokenType::BOOL_SIGN, L"get token bool sign");

			scanner = Scanner("true");
			Assert::IsTrue(scanner.getToken().type() == TokenType::TRUE, L"get token true");

			scanner = Scanner("false");
			Assert::IsTrue(scanner.getToken().type() == TokenType::FALSE, L"get token false");

			scanner = Scanner("||");
			Assert::IsTrue(scanner.getToken().type() == TokenType::OR, L"get token or");

			scanner = Scanner(">");
			Assert::IsTrue(scanner.getToken().type() == TokenType::GEATER_THAN, L"get token geater than");

			scanner = Scanner(">=");
			Assert::IsTrue(scanner.getToken().type() == TokenType::GEATER_THAN_OR_EQUAL, L"get token geater than or equal");

			scanner = Scanner("<");
			Assert::IsTrue(scanner.getToken().type() == TokenType::LESS_THAN, L"get token less than");

			scanner = Scanner("<=");
			Assert::IsTrue(scanner.getToken().type() == TokenType::LESS_THAN_OR_EQUAL, L"get token less than or equal");

			scanner = Scanner("[");
			Assert::IsTrue(scanner.getToken().type() == TokenType::LEFT_SQUARE_BRA, L"get token left square bracket");

			scanner = Scanner("]");
			Assert::IsTrue(scanner.getToken().type() == TokenType::RIGHT_SQUARE_BAR, L"get token right square bracket");

			scanner = Scanner(".");
			Assert::IsTrue(scanner.getToken().type() == TokenType::POINT, L"get token point");

			scanner = Scanner("->");
			Assert::IsTrue(scanner.getToken().type() == TokenType::POINT_TO_STRUCT, L"get token point to struct");

			scanner = Scanner("print");
			Assert::IsTrue(scanner.getToken().type() == TokenType::PRINT, L"get token print");

			// skip token
			scanner = Scanner(" int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT_SIGN, L"skip token space ");

			// statement 
			scanner = Scanner("int i = 0;");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INT_SIGN, L"assign[0] : int");
			Assert::IsTrue(scanner.getToken().type() == TokenType::VARIABLE, L"assign[1] : i");
			Assert::IsTrue(scanner.getToken().type() == TokenType::ASSIGN, L"assign[2] : =");
			Assert::IsTrue(scanner.getToken().type() == TokenType::INTEGER, L"assign[3] : 0");
			Assert::IsTrue(scanner.getToken().type() == TokenType::SEMICOLON, L"assign[4] : ;");
		}

		TEST_METHOD(TestScanner_GetToken_Plus)
		{
			auto scanner = Scanner("+");
			auto token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::PLUS, L"get token plus sign type");
			Assert::IsTrue(token.value() == std::string("+"), L"get token plus sign value");
		}

		TEST_METHOD(TestScanner_GetToken_Minus)
		{
			auto scanner = Scanner("-");
			auto token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::MINUS, L"get token minus sign type");
			Assert::IsTrue(token.value() == std::string("-"), L"get token minus sign value");
		
		}

		TEST_METHOD(TestScanner_GetToken_Mul)
		{
			auto scanner = Scanner("*");
			auto token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::MUL, L"get token mul sign type");
			Assert::IsTrue(token.value() == std::string("*"), L"get token mul sign value");
		}

		TEST_METHOD(TestScanner_GetToken_Div)
		{
			auto scanner = Scanner("/");
			auto token = scanner.getToken();
			Assert::IsTrue(token.type() == TokenType::DIV, L"get token div sign type");
			Assert::IsTrue(token.value() == std::string("/"), L"get token div sign value");
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
			Assert::IsTrue(scanner.peekToken().type() == TokenType::INT_SIGN, L"int");
			scanner.getToken();
			Assert::IsTrue(scanner.peekToken().type() == TokenType::VARIABLE, L"i");
			scanner.getToken();
			Assert::IsTrue(scanner.peekToken().type() == TokenType::ASSIGN, L"=");
		}

		TEST_METHOD(TestScanner_GetTokenList)
		{
			auto scanner = Scanner("int i = 0;");

			auto tokenList = scanner.getTokenList();
			Assert::IsTrue(tokenList[0].type() == TokenType::INT_SIGN, L"assign[0] : int");
			Assert::IsTrue(tokenList[1].type() == TokenType::VARIABLE, L"assign[1] : i");
			Assert::IsTrue(tokenList[2].type() == TokenType::ASSIGN, L"assign[2] : =");
			Assert::IsTrue(tokenList[3].type() == TokenType::INTEGER, L"assign[3] : 0");
			Assert::IsTrue(tokenList[4].type() == TokenType::SEMICOLON, L"assign[4] : ;");

		}
	};
}