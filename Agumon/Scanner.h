#pragma once


#include "Convert.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <functional>


namespace Agumon
{
	enum class TokenType
	{
		INTEGER,
		DECIMAL,
		TRUE,
		FALSE,
		

		INT_SIGN,
		DOUBLE_SIGN,
		BOOL_SIGN,
		

		VARIABLE,

		ASSIGN,
		SEMICOLON,

		PLUS,
		MINUS,
		MUL,
		DIV,
		LEFT_PAR,
		RIGHT_PAR,
		OR,

		INVAILD,
		
	};

	class Token
	{
	public:
		Token(TokenType type, int value) : type_(type), value_(std::to_string(value)) { }
		Token(TokenType type, double value) : type_(type), value_(std::to_string(value)) { }
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
			map_.insert(std::pair<std::string, Token>("int", Token(TokenType::INT_SIGN)));
			map_.insert(std::pair<std::string, Token>("double", Token(TokenType::DOUBLE_SIGN)));
			map_.insert(std::pair<std::string, Token>("bool", Token(TokenType::BOOL_SIGN)));
			map_.insert(std::pair<std::string, Token>("true", Token(TokenType::TRUE)));
			map_.insert(std::pair<std::string, Token>("false", Token(TokenType::FALSE)));
			map_.insert(std::pair<std::string, Token>(";", Token(TokenType::SEMICOLON)));
			map_.insert(std::pair<std::string, Token>("=", Token(TokenType::ASSIGN)));
			map_.insert(std::pair<std::string, Token>("+", Token(TokenType::PLUS)));
			map_.insert(std::pair<std::string, Token>("-", Token(TokenType::MINUS)));
			map_.insert(std::pair<std::string, Token>("*", Token(TokenType::MUL)));
			map_.insert(std::pair<std::string, Token>("/", Token(TokenType::DIV)));
			map_.insert(std::pair<std::string, Token>("(", Token(TokenType::LEFT_PAR)));
			map_.insert(std::pair<std::string, Token>(")", Token(TokenType::RIGHT_PAR)));
			map_.insert(std::pair<std::string, Token>("||", Token(TokenType::OR)));
		}
	public:
		Token						token(std::string key);
		Token						token(char key);
		bool						find(std::string key);
		bool						find(char key);

	private:
		std::map<std::string, Token> map_;
	};

	inline Token Dictionary::token(std::string key)
	{
		return find(key) ? map_.find(key)->second : Token(TokenType::INVAILD);
	}
	inline Token Dictionary::token(char key)
	{
		return find(key) ? map_.find(Convert::toString(key))->second : Token(TokenType::INVAILD);
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
		char						getChar();
		char						peekChar();
		std::string					peekChar(int size);
		Token						getToken();
		bool						isEndOfExp();
		bool						isOutOfRange(int size);

	public:
		inline Token peekToken()
		{
			auto scanner = Scanner(std::string(text_.begin() + index_, text_.end()));
			return scanner.getToken();
		}

	public:
		Token						getSignToken();
		Token						getIdentifierToken();
		Token						getNumberToken();
		std::string					getIntegerValue();

	private:
		std::string					text_;
		size_t						index_;
		Dictionary					dict_;

	};
	inline char Scanner::getChar()
	{
		return isEndOfExp() ? text_[text_.size() - 1] : text_[index_++];
	}

	inline char Scanner::peekChar()
	{
		return isEndOfExp() ? text_[text_.size() - 1] : text_[index_];
	}

	inline std::string Scanner::peekChar(int size)
	{
		return isEndOfExp() || isOutOfRange(size) ? std::string("") :
			std::string(text_.begin() + index_, text_.begin() + index_ + size);
	}

	inline bool Scanner::isEndOfExp()
	{
		return index_ >= text_.size() ? true : false;
	}

	inline bool	Scanner::isOutOfRange(int size)
	{
		return index_ + size > text_.size();
	}

	inline Token Scanner::getSignToken()
	{
		if (dict_.find(peekChar()))
		{
			return dict_.token(getChar());
		}
		else
		{
			std::string buffer;
			buffer.push_back(getChar());
			buffer.push_back(getChar());
			return dict_.token(buffer);
		}
		
	}

	inline Token Scanner::getIdentifierToken()
	{
		std::string value;
		while (isalpha(peekChar()) && !isEndOfExp()) 
			{ value.push_back(getChar()); }
		return dict_.find(value) ? dict_.token(value) : Token(TokenType::VARIABLE, value);
	}

	inline std::string Scanner::getIntegerValue()
	{
		std::string value;
		while (isdigit(peekChar()) && !isEndOfExp()) { value.push_back(getChar()); }
		return value;
	}

	inline Token Scanner::getNumberToken()
	{
		std::string value = getIntegerValue();

		if (peekChar() == '.')
		{
			getChar();	// skip 
			value += "." + getIntegerValue();
			return Token(TokenType::DECIMAL, Convert::toDouble(value));
		}
		else
		{
			return Token(TokenType::INTEGER, Convert::toInt(value));
		}
	}



}
