#include "lexer.h"

Token createToken(const std::string& value, TokenType type)
{
	return { value, type };
}

bool isAlphabetic(const std::string& source)
{
	for (char ch : source) {
		if (!std::isalpha( static_cast<unsigned char>(ch) ) ) {
			return false;
		}
	}

	return true;
}

bool isSkippable(char ch)
{
	return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

bool isInteger(char ch)
{
	return std::isdigit(static_cast<unsigned char>(ch));
}

std::vector<Token> Tokenize(const std::string& sourceCode)
{
	std::vector<Token> tokens;
	tokens.reserve(sourceCode.size() / 2);

	auto it = sourceCode.begin();
	auto end = sourceCode.end();

	while (it != end) {
		char ch = *it;

		switch (ch) {
			case '(':
				tokens.push_back({ "(", TokenType::OpenParen});
				++it;
				break;
			case ')':
				tokens.push_back({ ")", TokenType::CloseParen });
				++it;
				break;
			case '=':
				tokens.push_back({ "=", TokenType::Equals });
				++it;
				break;
			case ';':
				tokens.push_back({ ";", TokenType::Semicolon });
				++it;
				break;
			case ':':
				tokens.push_back({ ":", TokenType::Colon });
				++it;
				break;
			case ',':
				tokens.push_back({ ",", TokenType::Comma });
				++it;
				break;
			case '[':
				tokens.push_back({ "[", TokenType::OpenBracket });
				++it;
				break;
			case ']':
				tokens.push_back({ "]", TokenType::CloseBracket });
				++it;
				break;
			case '{':
				tokens.push_back({ "{", TokenType::OpenBrace });
				++it;
				break;
			case '}':
				tokens.push_back({ "}", TokenType::CloseBrace });
				++it;
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
				tokens.push_back({ std::string(1, ch), TokenType::BinaryOperaotr});
				++it;
				break;

			default:
				if (isInteger(ch)) {
					std::string number;

					while (it != end && isInteger(*it)) {
						number += *it;
						++it;
					}

					tokens.push_back({ number, TokenType::Number });
				}

				else if (isAlphabetic(std::string(1, ch))) {
					std::string identifier;

					while (it != end && isAlphabetic(std::string(1, *it))) {
						identifier += *it;
						++it;
					}

					auto keywordIT = KEYWORDS.find(identifier);

					if (keywordIT != KEYWORDS.end())
						tokens.push_back({ identifier, keywordIT->second });

					else
						tokens.push_back({ identifier, TokenType::Identifier });
				}

				else if (isSkippable(ch))
					++it;

				else {
					std::cerr << "Unrecognized character found in source: "
						<< static_cast<int>(ch)
						<< " ("
						<< ch
						<< ")"
						<< std::endl;

					exit(1);
				}

				break;
		}
	}

	tokens.push_back({ "EndOfFile", TokenType::_EOF });
	return tokens;
}
