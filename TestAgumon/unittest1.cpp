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
		//TEST_METHOD(TestAST_Init)
		//{
		//	AST ast = AST(Token(TokenType::INVAILD));
		//	Assert::IsTrue(ast.type() == TokenType::INVAILD, L"root type : invaild");

		//	ast = AST(Token(TokenType::INTEGER, 1));
		//	Assert::IsTrue(ast.type() == TokenType::INTEGER, L"root type : integer");
		//}

		//TEST_METHOD(TestAST_AddChildren)
		//{
		//	AST ast = AST(Token(TokenType::DECIMAL, 0.0));
		//	ast.addChildren(Token(TokenType::ASSIGN));
		//	auto childrenList = ast.childrenList();
		//	Assert::AreEqual(childrenList.size(), size_t(1), L"childList.size : 1");
		//	Assert::IsTrue(childrenList[0].type() == TokenType::ASSIGN, L"childList.node[0] : assign");

		//	ast.addChildren(AST(Token(TokenType::INVAILD)));
		//	childrenList = ast.childrenList();
		//	Assert::IsTrue(childrenList[1].type() == TokenType::INVAILD, L"childList.node[1] : invaild");
		//}


		//TEST_METHOD(TestParser_AssignStat)
		//{
		//	Parser parser = Parser("int i = 2;");
		//	auto node = parser.node();
		//	Assert::IsTrue(node.type() == TokenType::ASSIGN, L"root type : assign");
		//	Assert::IsTrue(node.childrenList()[0].type() == TokenType::INT_SIGN, L"childList[0] type : int_sign");
		//	Assert::IsTrue(node.childrenList()[1].type() == TokenType::VARIABLE, L"childList[1] type : variable");
		//	Assert::IsTrue(node.childrenList()[2].type() == TokenType::INTEGER, L"childList[2] type : integer");
		//}

		//TEST_METHOD(TestParser_MinusExp)
		//{
		//	Parser parser = Parser("1-2;");
		//	auto node = parser.node();
		//	Assert::IsTrue(node.type() == TokenType::MINUS, L"root type : minus");
		//	Assert::IsTrue(node.childrenList()[0].type() == TokenType::INTEGER, L"childList[0] type : integer");
		//	Assert::IsTrue(node.childrenList()[1].type() == TokenType::INTEGER, L"childList[1] type : integer");
		//}

		//TEST_METHOD(TestParser_PlusExp)
		//{
		//	Parser parser = Parser("1+2;");
		//	auto node = parser.node();
		//	Assert::IsTrue(node.type() == TokenType::PLUS, L"root type : plus");
		//}

		//TEST_METHOD(TestParser_MixedExp)
		//{
		//	Parser parser = Parser("1*2+2;");
		//	auto node = parser.node();
		//	Assert::IsTrue(node.type() == TokenType::PLUS, L"root type : plus");

		//	node = node.childrenList()[0];	// 1*2
		//	Assert::IsTrue(node.type() == TokenType::MUL, L"root type : mul");

		//	node = Parser("1-2*3+5;").node();
		//	Assert::IsTrue(node.type() == TokenType::MINUS, L"root type : minus");
		//}

		//TEST_METHOD(TestParser_ParenthesisExp)
		//{
		//	Parser parser = Parser("(1+2);");
		//	auto node = parser.node();
		//	Assert::IsTrue(node.type() == TokenType::PLUS, L"root type : plus");

		//	node = Parser("(2+1)-1;").node();
		//	Assert::IsTrue(node.type() == TokenType::MINUS, L"root type : minus");
		//}

		//TEST_METHOD(TestParser_GreaterThan)
		//{
		//	auto node = Parser("2 > 1;").node();
		//	Assert::IsTrue(node.type() == TokenType::GEATER_THAN, L"root type : greater than");
		//}

		//TEST_METHOD(TestParser_GreaterThanOrEqual)
		//{
		//	auto node = Parser("2 >= 1;").node();
		//	Assert::IsTrue(node.type() == TokenType::GEATER_THAN_OR_EQUAL, L"root type : greater than or equal");
		//}

		//TEST_METHOD(TestParser_LessThan)
		//{
		//	auto node = Parser("2 < 1;").node();
		//	Assert::IsTrue(node.type() == TokenType::LESS_THAN, L"root type : less than");
		//}

		//TEST_METHOD(TestParser_LessThanOrEqual)
		//{
		//	auto node = Parser("2 <= 1;").node();
		//	Assert::IsTrue(node.type() == TokenType::LESS_THAN_OR_EQUAL, L"root type : less than or equal");
		//}

		//TEST_METHOD(TestParser_Array)
		//{
		//	// int arr[5], 暂时没想到好办法，先定义成变量，size放到子节点

		//	auto node = Parser("arr[5];").node();
		//	Assert::IsTrue(node.type() == TokenType::VARIABLE, L"root type : array -> variable");
		//	Assert::IsTrue(node.childrenList()[0].type() == TokenType::INTEGER, L"child[0] type : integer(size)");
		//}

		//TEST_METHOD(TestParser_Point)
		//{
		//	auto node = Parser("class.member;").node();
		//	Assert::IsTrue(node.type() == TokenType::POINT, L"root type : point");
		//	Assert::IsTrue(node.childrenList()[1].type() == TokenType::VARIABLE, L"child[1] type : var(member)");
		//}

		//TEST_METHOD(TestParser_Pointer)
		//{
		//	auto node = Parser("pointer->member;").node();
		//	Assert::IsTrue(node.type() == TokenType::POINT_TO_STRUCT, L"root type : point to struct");
		//}

		//TEST_METHOD(TestParser_AssignStatAndExp)
		//{
		//	auto node = Parser("int i = 1 + 1;").node();
		//	Assert::IsTrue(node.type() == TokenType::ASSIGN, L"root type : assign sign");
		//	
		//	node = node.childrenList()[2];
		//	Assert::IsTrue(node.type() == TokenType::PLUS, L"rhs type : plus");
		//}
	};
}