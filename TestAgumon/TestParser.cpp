#include "stdafx.h"
#include "CppUnitTest.h"

#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Agumon;

namespace TestAgumon
{
	TEST_CLASS(Test_Parser)
	{
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




		/* exp */

		TEST_METHOD(TestParser_Exp_Integer)
		{
			auto node = Parser("1;").node();
			Assert::IsTrue(node.type() == TokenType::INTEGER, L"root type : integer");
		}

		TEST_METHOD(TestParser_Exp_Square)
		{
			auto node = Parser("arr[5];").node();
			Assert::IsTrue(node.type() == TokenType::ARRAY, L"root type : array");
			Assert::IsTrue(node.childrenList()[0].type() == TokenType::VARIABLE, L"lhs type : varible");
			Assert::IsTrue(node.childrenList()[1].type() == TokenType::INTEGER, L"rhs type : size(integer)");
		}

		TEST_METHOD(TestParser_Exp_Par)
		{
			auto node = Parser("func();").node();
			Assert::IsTrue(node.type() == TokenType::FUNCTION, L"root type : function");
			Assert::IsTrue(node.childrenList()[0].type() == TokenType::VARIABLE, L"lhs type : varible");

			node = Parser("func(1);").node();
			Assert::IsTrue(node.childrenList()[1].type() == TokenType::INTEGER, L"parameter type : integer");

			/*node = Parser("func(1, 2);").node();
			Assert::IsTrue(node.childrenList()[2].type() == TokenType::INTEGER, L"parameter_2 type : integer");*/
		}



		// ×îµÍÒ»¼¶
		TEST_METHOD(TestParser_Exp)
		{
			auto node = Parser("1, 2").node();
			Assert::IsTrue(node.type() == TokenType::COMMA, L"root type : comma");
			Assert::IsTrue(node.childrenList()[0].type() == TokenType::INTEGER, L"first element type : integer");
			Assert::IsTrue(node.childrenList()[1].type() == TokenType::INTEGER, L"second element type : integer");

			node = Parser("1, 2, 3").node();
			Assert::IsTrue(node.childrenList()[2].type() == TokenType::INTEGER, L"third element type : integer");

			
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

		TEST_METHOD(TestParser_AssignStatAndExp)
		{
			auto node = Parser("int i = 1 + 1;").node();
			Assert::IsTrue(node.type() == TokenType::ASSIGN, L"root type : assign sign");

			node = node.childrenList()[2];
			Assert::IsTrue(node.type() == TokenType::PLUS, L"rhs type : plus");
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

		TEST_METHOD(TestParser_AssignStatAndVaribale)
		{
			auto node = Parser("int i = j;").node();
			Assert::IsTrue(node.childrenList()[2].type() == TokenType::VARIABLE, L"rhs type : varible");

			node = Parser("int i = j + k;").node();
			Assert::IsTrue(node.childrenList()[2].type() == TokenType::PLUS, L"rhs type : plus");

		}


	};

}