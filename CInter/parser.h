#pragma once

#include "lexer.h"
#include "ast.h"

class Parser {
	private:
		std::vector<Token> tokens = {};

		bool not_EOF() const;

		Token at() const;
		Token eat();
		Token expect(TokenType type, const std::string& err);

		std::unique_ptr<Statement> parseStatement();
		std::unique_ptr<Statement> parseVariableDeclaration();
		std::unique_ptr<Expression> parseExpression();
		std::unique_ptr<Expression> parseAssignmentExpression();
		std::unique_ptr<Expression> parseObjectExpression();
		std::unique_ptr<Expression> parseAddtiveExpression();
	public:
		Program produceAST(std::string& sourceCode);
};