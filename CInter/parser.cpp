#include "parser.h"

bool Parser::not_EOF() const
{
    return !tokens.empty() && tokens.front().type != TokenType::_EOF;
}

Token Parser::at() const
{
    return tokens.empty() ? Token {} : tokens.front();
}

Token Parser::eat()
{
    Token previous = std::move(tokens.front());

    tokens.erase(tokens.begin());

    return previous;
}

Token Parser::expect(TokenType type, const std::string& err)
{
    Token previous = eat();

    if (previous.type != type) {
        std::cerr << "Parser Error:\n" << err << " - Expecting: " << static_cast<int>(type) << std::endl;
        exit(1);
    }

    return previous;
}


std::unique_ptr<Statement> Parser::parseStatement()
{
    switch (this->at().type) {
        case TokenType::Const:
        case TokenType::Let:
           return this->parseVariableDeclaration();

        default: {
            std::unique_ptr<Expression> expression = this->parseExpression();

            if (this->at().type == TokenType::Semicolon)
                this->eat();

            return expression;
        }
    }
}

std::unique_ptr<Statement> Parser::parseVariableDeclaration()
{
    const bool isConstant = this->eat().type == TokenType::Const;
    const std::string identifier = this->expect(TokenType::Identifier, std::string("Expected identifier name following let | const keywords.")).value;

    if (this->at().type == TokenType::Semicolon) {
        this->eat();

        if (isConstant)
            throw "Must assign value to constant expression. No value provided.";

        auto varDeclaration = std::make_unique<VariableDeclaration>();

        varDeclaration->identifier = identifier;
        varDeclaration->constant = false;

        return varDeclaration;
    }

    this->expect(TokenType::Equals, std::string("Expected equals token following identifier in var declaration."));

    auto declaration = std::make_unique<VariableDeclaration>();

    declaration->identifier = identifier;
    declaration->constant = isConstant;
    declaration->kind = NodeType::VariableDeclaration;
    declaration->value = this->parseExpression();

    return declaration;
}

std::unique_ptr<Expression> Parser::parseExpression()
{
    return this->parseAssignmentExpression();
}

std::unique_ptr<Expression> Parser::parseAssignmentExpression()
{
    std::unique_ptr<Expression> left = this->parseObjectExpression();

    if (this->at().type == TokenType::Equals) {
        this->eat();

        std::unique_ptr<Expression> value = this->parseAssignmentExpression();

        auto assignment = std::make_unique<AssignmentExpression>();

        assignment->value = std::move(value);
        assignment->assignee = std::move(left);
        assignment->kind = NodeType::AssignmentExpression;

        return assignment;
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseObjectExpression()
{
    if (this->at().type != TokenType::OpenBrace) {
        return this->parseAddtiveExpression();
    }

    this->eat();

    std::vector<std::unique_ptr<Property>> properties;

    while (this->not_EOF() && this->at().type != TokenType::CloseBrace) {
        const std::string key = this->expect(TokenType::Identifier, std::string("Object literal key expected")).value;

        if (this->at().type == TokenType::Comma) {
            this->eat();

            auto prop = std::make_unique<Property>();

            prop->key = key;
            prop->kind = NodeType::Property;

            properties.push_back(std::move(prop)); 

            continue;
        }
    
        else if (this->at().type == TokenType::CloseBrace) {
            auto prop = std::make_unique<Property>();

            prop->key = key;
            prop->kind = NodeType::Property;

            properties.push_back(std::move(prop));

            continue;
        }

        this->expect(TokenType::Colon, std::string("Missing colon following identifier in ObjectExpression"));

        std::unique_ptr<Expression> value = this->parseExpression();

        auto prop = std::make_unique<Property>();

        prop->key = key;
        prop->kind = NodeType::Property;
        prop->value = std::move(value);
           
        properties.push_back(std::move(prop));
        
        if (this->at().type != TokenType::CloseBrace) {
            this->expect(TokenType::Comma, std::string("Expected comma or closing bracket following property"));
            
        }
    }

            this->expect(TokenType::CloseBrace, std::string("Object literal missing closing brace."));
        
        auto object = std::make_unique<ObjectLiteral>();
        
        object->kind = NodeType::ObjectLiteral;
        object->properties = std::move(properties);

        return object;
}

std::unique_ptr<Expression> Parser::parseAddtiveExpression()
{
    return std::unique_ptr<Expression>();
}

Program Parser::produceAST(std::string& sourceCode)
{
    Program program;
   
    return std::move(program);
}
