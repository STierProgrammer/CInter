#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>

enum TokenType {
	Number,
	Identifier,
	Let,
	Const,
	Exostatic,
	Static,
	BinaryOperaotr,
	Equals, 
	Comma,
	Colon,
	Semicolon,
	OpenParen,
	CloseParen,
	OpenBrace,
	CloseBrace,
	OpenBracket,
	CloseBracket,
	Dot,
	_EOF,
};

const std::unordered_map<std::string, TokenType> KEYWORDS {
	{ "let", TokenType::Let },
	{ "const", TokenType::Const },
	{ "exostatic", TokenType::Exostatic },
	{ "static", TokenType::Static }
};

struct Token {
	std::string value;
	TokenType type;
};

Token createToken(const std::string& value, TokenType type);

bool isAlphabetic(const std::string& source);
bool isSkippable(char ch);
bool isInteger(char ch);

std::vector<Token> Tokenize(const std::string& sourceCode);
