#pragma once


#include <vector>
#include <map>
#include <string>
#include <sstream>


namespace Agumon
{
	enum class TokenType
	{
		INTEGER,

		INT,
		VARIABLE,

		ASSIGN,
		SEMICOLON,

		INVAILD,
		
	};

	class Token
	{
	public:
		Token(TokenType type, int value) : type_(type), value_(std::to_string(value)) { }
		Token(TokenType type, std::string value = "") : type_(type), value_(value) { }

	public:
		inline TokenType type() { return type_; }
		inline std::string value() { return value_; }

	private:
		TokenType type_;
		std::string value_;
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
		Token						token(std::string key);
		bool						find(std::string key);
		bool						find(char key);

	private:
		std::map<std::string, Token> map_;
	};

	inline Token Dictionary::token(std::string key)
	{
		return find(key) ? map_.find(key)->second : Token(TokenType::INVAILD);
	}
	inline bool Dictionary::find(std::string key)
	{
		return map_.find(key) != map_.end();
	}
	inline bool Dictionary::find(char key)
	{
		std::string keyStr;
		keyStr.push_back(key);
		return find(keyStr);
	}

	class Scanner
	{
	public:
		Scanner(std::string text) : text_(text), index_(0) { }

	public:

		Token getToken();

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

	private:
		Dictionary dict_;

	};


}
