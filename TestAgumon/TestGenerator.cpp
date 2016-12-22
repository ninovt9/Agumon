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
	TEST_CLASS(TestGenerator)
	{


	public:

		class Generator
		{
		public:
			Generator() : ast_(AST(Token(TokenType::INVAILD)))
			{
				;
			}
			Generator(AST ast) : ast_(ast)
			{
				analyze();
			}

		public:

			inline void analyze()
			{
				// auto type = ast_.childrenList()[0].type();
				auto name = ast_.childrenList()[1].token().value();
				auto token = ast_.childrenList()[2].token();
				addVar(name, token);
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
			std::vector<std::string> LINEBREAK{ "\n" };
			std::vector<std::string> OPTION{ ".386", ".model flat, stdcall", "option casemap : none" };
			std::vector<std::string> INCLUDE{ "include windows.inc", "include kernel32.inc",
				"includelib kernel32.lib", "include	msvcrt.inc", "includelib msvcrt.lib" };
			std::vector<std::string> DATA{ ".data" };

			inline void pushLine(std::ofstream& outFile, std::vector<std::string> lineList)
			{
				for (auto line : lineList)
				{
					outFile << line << "\n";
				}
			}

			inline void toFile(std::string filePath)
			{
				//std::ofstream outFile(filePath);
				//pushLine(outFile, HEAD);
				//pushLine(outFile, LINEBREAK);
				//pushLine(outFile, INCLUDE);

				//for (auto var : varList_)
				//{
				//	DATA.push_back(var.first + " " + "dd" + " " + var.second.value());
				//}
				//pushLine(outFile, LINEBREAK);
				//pushLine(outFile, DATA);

				//outFile.close();
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

		private:
			AST ast_;
			std::map<std::string, Token> varList_;
		};

	private:
		const std::string TEST_PATH = "D:\\code\\Agumon\\TestAgumon\\";
		const std::string ASM_FILE = "test.asm";

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
			auto generator = Generator();

			auto outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushDataToFile(outFile);
			outFile.close();

			auto inFile = std::ifstream(TEST_PATH + ASM_FILE);
			std::string line;
			std::getline(inFile, line);
			Assert::IsTrue(line == ".data", L".data");
			inFile.close();


			auto node = Parser("int i = 0;").node();
			generator = Generator(node);

			outFile = std::ofstream(TEST_PATH + ASM_FILE, std::ios::out);
			generator.pushDataToFile(outFile);
			outFile.close();

			inFile = std::ifstream(TEST_PATH + ASM_FILE);
			std::getline(inFile, line);
			Assert::IsTrue(line == ".data", L".data");

			std::getline(inFile, line);
			Assert::IsTrue(line == "i dd 0");
			inFile.close();
		}




		//TEST_METHOD(TestGenerator_pushLineFor)
		//{
		//	auto node = Parser("int i = 1 + 1;").node();
		//	auto generator = Generator(node);
		//	generator.toFile(TEST_PATH + ASM_FILE);
		//}

	};
}