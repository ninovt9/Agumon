#include "stdafx.h"
#include "CppUnitTest.h"

#include "Scanner.h"
#include "Dictionary.h"
#include "Parser.h"

#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Agumon;

namespace TestAgumon
{		
	TEST_CLASS(UnitTest1)
	{
	//private:
	//	std::string testFilePath = "D:\\code\\Agumon\\TestAgumon";

	//public:

	//	TEST_METHOD_INITIALIZE(init)
	//	{
	//		auto asmFile = testFilePath + "test.asm";
	//	}

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

			scanner = Scanner("+");
			Assert::IsTrue(scanner.getToken().type() == TokenType::PLUS, L"get token plus sign");

			scanner = Scanner("-");
			Assert::IsTrue(scanner.getToken().type() == TokenType::MINUS, L"get token minus sign");

			scanner = Scanner("*");
			Assert::IsTrue(scanner.getToken().type() == TokenType::MUL, L"get token mul sign");

			scanner = Scanner("/");
			Assert::IsTrue(scanner.getToken().type() == TokenType::DIV, L"get token div sign");

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


		TEST_METHOD(TestAST_Init)
		{
			AST ast = AST(Token(TokenType::INVAILD));
			Assert::IsTrue(ast.type() == TokenType::INVAILD, L"root type : invaild");

			ast = AST(Token(TokenType::INTEGER, 1));
			Assert::IsTrue(ast.type() == TokenType::INTEGER, L"root type : integer");
		}

		TEST_METHOD(TestAST_AddChildren)
		{
			AST ast = AST(Token(TokenType::DECIMAL, 0.0));
			ast.addChildren(Token(TokenType::ASSIGN));
			auto childrenList = ast.childrenList();
			Assert::AreEqual(childrenList.size(), size_t(1), L"childList.size : 1");
			Assert::IsTrue(childrenList[0].type() == TokenType::ASSIGN, L"childList.node[0] : assign");

			ast.addChildren(AST(Token(TokenType::INVAILD)));
			childrenList = ast.childrenList();
			Assert::IsTrue(childrenList[1].type() == TokenType::INVAILD, L"childList.node[1] : invaild");
		}


		TEST_METHOD(TestParser_AssignStat)
		{
			Parser parser = Parser("int i = 2;");
			auto node = parser.node();
			Assert::IsTrue(node.type() == TokenType::ASSIGN, L"root type : assign");
			Assert::IsTrue(node.childrenList()[0].type() == TokenType::INT_SIGN, L"childList[0] type : int_sign");
			Assert::IsTrue(node.childrenList()[1].type() == TokenType::VARIABLE, L"childList[1] type : variable");
			Assert::IsTrue(node.childrenList()[2].type() == TokenType::INTEGER, L"childList[2] type : integer");
		}

		TEST_METHOD(TestParser_MinusExp)
		{
			Parser parser = Parser("1-2;");
			auto node = parser.node();
			Assert::IsTrue(node.type() == TokenType::MINUS, L"root type : minus");
			Assert::IsTrue(node.childrenList()[0].type() == TokenType::INTEGER, L"childList[0] type : integer");
			Assert::IsTrue(node.childrenList()[1].type() == TokenType::INTEGER, L"childList[1] type : integer");
		}

		TEST_METHOD(TestParser_PlusExp)
		{
			Parser parser = Parser("1+2;");
			auto node = parser.node();
			Assert::IsTrue(node.type() == TokenType::PLUS, L"root type : plus");
		}

		TEST_METHOD(TestParser_MixedExp)
		{
			Parser parser = Parser("1*2+2;");
			auto node = parser.node();
			Assert::IsTrue(node.type() == TokenType::PLUS, L"root type : plus");

			node = node.childrenList()[0];	// 1*2
			Assert::IsTrue(node.type() == TokenType::MUL, L"root type : mul");

			node = Parser("1-2*3+5;").node();
			Assert::IsTrue(node.type() == TokenType::MINUS, L"root type : minus");
		}

		TEST_METHOD(TestParser_ParenthesisExp)
		{
			Parser parser = Parser("(1+2);");
			auto node = parser.node();
			Assert::IsTrue(node.type() == TokenType::PLUS, L"root type : plus");

			node = Parser("(2+1)-1;").node();
			Assert::IsTrue(node.type() == TokenType::MINUS, L"root type : minus");
		}

		TEST_METHOD(TestParser_GreaterThan)
		{
			auto node = Parser("2 > 1;").node();
			Assert::IsTrue(node.type() == TokenType::GEATER_THAN, L"root type : greater than");
		}

		TEST_METHOD(TestParser_GreaterThanOrEqual)
		{
			auto node = Parser("2 >= 1;").node();
			Assert::IsTrue(node.type() == TokenType::GEATER_THAN_OR_EQUAL, L"root type : greater than or equal");
		}

		TEST_METHOD(TestParser_LessThan)
		{
			auto node = Parser("2 < 1;").node();
			Assert::IsTrue(node.type() == TokenType::LESS_THAN, L"root type : less than");
		}

		TEST_METHOD(TestParser_LessThanOrEqual)
		{
			auto node = Parser("2 <= 1;").node();
			Assert::IsTrue(node.type() == TokenType::LESS_THAN_OR_EQUAL, L"root type : less than or equal");
		}

		TEST_METHOD(TestParser_Array)
		{
			// int arr[5], 暂时没想到好办法，先定义成变量，size放到子节点

			auto node = Parser("arr[5];").node();
			Assert::IsTrue(node.type() == TokenType::VARIABLE, L"root type : array -> variable");
			Assert::IsTrue(node.childrenList()[0].type() == TokenType::INTEGER, L"child[0] type : integer(size)");
		}

		TEST_METHOD(TestParser_Point)
		{
			auto node = Parser("class.member;").node();
			Assert::IsTrue(node.type() == TokenType::POINT, L"root type : point");
			Assert::IsTrue(node.childrenList()[1].type() == TokenType::VARIABLE, L"child[1] type : var(member)");
		}

		TEST_METHOD(TestParser_Pointer)
		{
			auto node = Parser("pointer->member;").node();
			Assert::IsTrue(node.type() == TokenType::POINT_TO_STRUCT, L"root type : point to struct");
		}

		TEST_METHOD(TestParser_AssignStatAndExp)
		{
			auto node = Parser("int i = 1 + 1;").node();
			Assert::IsTrue(node.type() == TokenType::ASSIGN, L"root type : assign sign");
			
			node = node.childrenList()[2];
			Assert::IsTrue(node.type() == TokenType::PLUS, L"rhs type : plus");
		}

		//TEST_METHOD(TestParser_Print)
		//{
		//	auto node = Parser("print('Hello, World')");
		//	
		//}

		//TEST_METHOD(TestParser_FuncCall)
		//{
		//	auto node = Parser("func()").node();
		//	Assert::IsTrue(node.type() == TokenType::)
		//}


		//class Generator
		//{
		//public:
		//	Generator(AST ast) : ast_(ast) 
		//	{ 
		//		analyze();
		//	}

		//public:

		//	inline void analyze()
		//	{
		//		addVar(ast_.childrenList()[1].token());
		//	}

		//	inline void addVar(Token token)
		//	{
		//		varList_.insert({ token.value(), token});
		//	}

		//	inline bool findVar(std::string varName)
		//	{
		//		return varList_.find(varName) != varList_.end();
		//	}

		//public:
		//	std::vector<std::string> LINEBREAK{"\n"};
		//	std::vector<std::string> HEAD{ ".386", ".model flat, stdcall", "option casemap : none" };
		//	std::vector<std::string> INCLUDE{"include windows.inc", "include kernel32.inc",
		//						"includelib kernel32.lib", "include	msvcrt.inc", "includelib msvcrt.lib" };
		//	std::vector<std::string> DATA{ ".data" };

		//	inline void pushLine(std::ofstream& outFile, std::vector<std::string> lineList)
		//	{
		//		for (auto line : lineList)
		//		{
		//			outFile << line << "\n";
		//		}
		//	}

		//	inline void toFile(std::string filePath)
		//	{
		//		std::ofstream outFile(filePath);
		//		pushLine(outFile, HEAD);
		//		pushLine(outFile, LINEBREAK);
		//		pushLine(outFile, INCLUDE);

		//		for (auto var : varList_)
		//		{
		//			DATA.push_back(var.first + " " + "dd" + " " + var.second.value());
		//		}
		//		pushLine(outFile, LINEBREAK);
		//		pushLine(outFile, DATA);

		//		outFile.close();
		//	}

		//	


		//private:
		//	AST ast_;
		//	std::map<std::string, Token> varList_;
		//};

		//TEST_METHOD(TestGenerator)
		//{
		//	
		//}

		TEST_METHOD(TestGenerator)
		{
			//auto node = Parser("int i = 0;").node();
			//Generator generator = Generator(node);
			//Assert::IsTrue(generator.findVar("i"), L"find var : i");
			//Assert::IsFalse(generator.findVar("d"), L"find var : d");

			//std::string filePath = "d:\\";
			//generator.toFile(filePath + "result.asm");
		}
	};
}