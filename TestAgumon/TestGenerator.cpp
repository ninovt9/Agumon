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
	TEST_CLASS(Test_Generator)
	{
	public:

		class Generator
		{
		public:
			Generator() : ast_(AST(Token(TokenType::INVAILD))) { ; }
			Generator(AST ast) : ast_(ast) { analyze(); }

		public:
			inline void analyze()
			{
				analyzeAssignNode(ast_);
			}

			inline void analyzeAssignNode(AST ast)
			{
				auto name = ast.childrenList()[1].value();
				auto token = ast.childrenList()[2].token();
				addVar(name, token);

				walk(ast.childrenList()[2]);

				statList_.push_back(std::string("mov ") + name + ", eax");
			}

			inline void walk(AST ast, std::string reg = "eax")
			{
				if (ast.type() == TokenType::INTEGER)
				{
					mov(ast.value(), reg);
				}
				else if (ast.type() == TokenType::PLUS)
				{
					walk(ast.childrenList()[0], "eax");
					walk(ast.childrenList()[1], "ecx");
					add("eax", "ecx");
				}
				else if (ast.type() == TokenType::MINUS)
				{
					walk(ast.childrenList()[0], "eax");
					walk(ast.childrenList()[1], "ecx");
					sub("eax", "ecx");
				}
				else if (ast.type() == TokenType::MUL)
				{
					walk(ast.childrenList()[0], "eax");
					walk(ast.childrenList()[1], "ecx");
					mul("eax", "ecx");
				}
				else if (ast.type() == TokenType::DIV)
				{
					walk(ast.childrenList()[0], "eax");
					walk(ast.childrenList()[1], "ecx");
					div("eax", "ecx");
				}
			}

			inline void mov(std::string value, std::string reg)
			{
				statList_.push_back(std::string("mov " + reg + ", " + value));
			}

			inline void add(std::string lhs, std::string rhs)
			{
				statList_.push_back(std::string("add " + lhs + ", " + rhs));
			}

			inline void sub(std::string lhs, std::string rhs)
			{
				statList_.push_back(std::string("sub " + lhs + ", " + rhs));
			}

			inline void mul(std::string lhs, std::string rhs)
			{
				statList_.push_back(std::string("imul " + lhs + ", " + rhs));
			}

			inline void div(std::string lhs, std::string rhs)
			{
				statList_.push_back(std::string("cdq"));
				statList_.push_back(std::string("idiv " + rhs));
			}

			inline void addVar(std::string name, Token token)
			{
				varList_.insert({ name, token });
			}

			inline bool findVar(std::string varName)
			{
				return varList_.find(varName) != varList_.end();
			}

		public:


			inline void pushLine(std::ofstream& outFile, std::vector<std::string> lineList)
			{
				for (auto line : lineList)
				{
					outFile << line << "\n";
				}
			}

			inline void toFile(std::ofstream &filePath)
			{
				pushOptionToFile(filePath);
				pushLine(filePath, LINEBREAK);
				pushIncludeToFile(filePath);
				pushLine(filePath, LINEBREAK);
				pushDataToFile(filePath);
				pushLine(filePath, LINEBREAK);
				pushCodeToFile(filePath);
			}

			inline void pushOptionToFile(std::ofstream &outFile)
			{
				pushLine(outFile, OPTION);
			}

			inline void pushIncludeToFile(std::ofstream &outFile)
			{
				pushLine(outFile, INCLUDE);
			}

			inline void pushDataToFile(std::ofstream &outFile)
			{
				pushLine(outFile, DATA);

				for (auto var : varList_)
				{
					std::string name = var.first;
					std::string type = "dd";
					std::string value = var.second.value();
					pushLine(outFile, { name + " " + type + " " + value });
				}
			}

			inline void pushCodeToFile(std::ofstream &outFile)
			{
				pushLine(outFile, CODE_START);
				pushLine(outFile, statList_);
				pushLine(outFile, CODE_END);
			}

		private:
			std::vector<std::string> LINEBREAK{ "\n" };
			std::vector<std::string> OPTION{ ".386", ".model flat, stdcall", "option casemap : none" };
			std::vector<std::string> INCLUDE{ "include windows.inc", "include kernel32.inc", "includelib kernel32.lib",
				"include	msvcrt.inc", "includelib msvcrt.lib" };
			std::vector<std::string> DATA{ ".data" };
			std::vector<std::string> CODE_START{ ".code", "start:" };
			std::vector<std::string> CODE_END{ "end start" };

		private:
			AST										ast_;
			std::map<std::string, Token>			varList_;
			std::vector<std::string>				statList_;
		};

	private:
		const std::string TEST_PATH = "..\\TestAgumon\\";
		const std::string ASM_FILE = "test.asm";

	private:
		inline void checkLine(std::ifstream &inFile, std::string correct, const wchar_t *message)
		{
			std::string line;
			std::getline(inFile, line);
			Assert::IsTrue(line == correct, message);
		}

	public:

		TEST_METHOD(TestGenerator_Analyze)
		{
			auto node = Parser("int i = 0;").node();
			Generator generator = Generator(node);
			Assert::IsTrue(generator.findVar("i"), L"find var : i");
			Assert::IsFalse(generator.findVar("d"), L"find var : d");
		}


		TEST_METHOD(TestGenerator_PushOptionToFile)
		{
			auto generator = Generator();
			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);

			Assert::IsFalse(outFile.fail(), L"open file");

			generator.pushOptionToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			std::string line;
			std::getline(inFile, line);
			Assert::IsTrue(line == ".386", L"line 1 : '.386'");
			inFile.close();
		}

		TEST_METHOD(TestGenerator_PushIncludeToFile)
		{
			auto generator = Generator();

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushIncludeToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			std::string line;
			std::getline(inFile, line);
			Assert::IsTrue(line == "include windows.inc", L"include windows.inc");
			inFile.close();
		}

		TEST_METHOD(TestGenerator_PushDataToFile)
		{
			auto generator = Generator(Parser("int i = 0").node());

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushDataToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			std::string line;
			std::getline(inFile, line);
			Assert::IsTrue(line == ".data", L".data");

			std::getline(inFile, line);
			Assert::IsTrue(line == "i dd 0");
			inFile.close();
		}

		TEST_METHOD(TestGenerator_PushCodeToFile_Assign)
		{
			auto generator = Generator(Parser("int i = 2").node());

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushCodeToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			checkLine(inFile, ".code", L".data");
			checkLine(inFile, "start:", L"start:");
			checkLine(inFile, "mov eax, 2", L"mov eax, 2");
			checkLine(inFile, "mov i, eax", L"mov i, eax");
			checkLine(inFile, "end start", L"end start");
		}

		TEST_METHOD(TestGenerator_PushCodeToFile_Add)
		{
			auto generator = Generator(Parser("int i = 1 + 1").node());

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushCodeToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			checkLine(inFile, ".code", L".data");
			checkLine(inFile, "start:", L"start:");
			checkLine(inFile, "mov eax, 1", L"mov eax, 1");
			checkLine(inFile, "mov ecx, 1", L"mov ecx, 1");
			checkLine(inFile, "add eax, ecx", L"add eax, ecx");
			checkLine(inFile, "mov i, eax", L"mov i, eax");
			checkLine(inFile, "end start", L"end start");
		 }

		TEST_METHOD(TestGenerator_PushCodeToFile_Div)
		{
			auto generator = Generator(Parser("int i = 10 / 2").node());

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushCodeToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			checkLine(inFile, ".code",			L".code");
			checkLine(inFile, "start:",			L"start:");
			checkLine(inFile, "mov eax, 10",	L"mov eax, 10");
			checkLine(inFile, "mov ecx, 2",		L"mov ecx, 2");
			checkLine(inFile, "cdq",			L"cdq");
			checkLine(inFile, "idiv ecx",		L"idiv ecx");
		}

		TEST_METHOD(TestGenerator_PushCodeToFile_mix)
		{
			auto generator = Generator(Parser("int i = (11 - 5) * 2;").node());

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushCodeToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			checkLine(inFile, ".code", L".data");
			checkLine(inFile, "start:", L"start");
			checkLine(inFile, "mov eax, 11", L"mov eax, 11");
			checkLine(inFile, "mov ecx, 5", L"mov ecx, 5");
			checkLine(inFile, "sub eax, ecx", L"sub eax, ecx");
			checkLine(inFile, "mov ecx, 2", L"mov ecx, 2");
			checkLine(inFile, "imul eax, ecx", L"imul eax, ecx");
			checkLine(inFile, "mov i, eax", L"mov i, eax");
			checkLine(inFile, "end start", L"end start");
		}

		TEST_METHOD(TestGenerator_demo)
		{
			auto node = Parser("int i = 2").node();
			auto generator = Generator(node);

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.toFile(outFile);
			outFile.close();
		}

	};
}