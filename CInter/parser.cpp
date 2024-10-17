#include "parser.h"

bool Parser::not_EOF() const
{
    return !tokens.empty() && tokens.front().type != TokenType::_EOF;
}

Token Parser::at() const
{
    return tokens.empty() ? Token{} : tokens.front();
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

    default: 
        auto expression = this->parseExpression();
        
        if (this->at().type == TokenType::Semicolon)
            this->eat();
        
        return expression;

    }
}

std::unique_ptr<Statement> Parser::parseVariableDeclaration()
{
    bool isConstant = this->eat().type == TokenType::Const;
    std::string identifier = this->expect(TokenType::Identifier, "Expected identifier name following let | const keywords.").value;

    if (this->at().type == TokenType::Semicolon) {
        this->eat();
        
        if (isConstant)
            throw "Must assign value to constant expression. No value provided.";

        auto varDeclaration = std::make_unique<VariableDeclaration>();
        
        varDeclaration->identifier = identifier;
        varDeclaration->constant = false;
        
        return varDeclaration;
    }

    this->expect(TokenType::Equals, "Expected equals token following identifier in var declaration.");
    
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
    auto left = this->parseObjectExpression();

    if (this->at().type == TokenType::Equals) {
        this->eat();
        
        auto value = this->parseAssignmentExpression();

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
        return this->parseAdditiveExpression();
    }

    this->eat();
    
    std::vector<std::unique_ptr<Property>> properties;

    while (this->not_EOF() && this->at().type != TokenType::CloseBrace) {
        std::string key = this->expect(TokenType::Identifier, "Object literal key expected").value;

        if (this->at().type == TokenType::Comma) {
            this->eat();
            
            auto prop = std::make_unique<Property>();
            
            prop->key = key;
            prop->kind = NodeType::Property;
            
            properties.push_back(std::move(prop));
            
            continue;
        }

        this->expect(TokenType::Colon, "Missing colon following identifier in ObjectExpression");
        
        auto value = this->parseExpression();
        auto prop = std::make_unique<Property>();
        
        prop->key = key;
        prop->kind = NodeType::Property;
        prop->value = std::move(value);
        
        properties.push_back(std::move(prop));

        if (this->at().type != TokenType::CloseBrace) {
            this->expect(TokenType::Comma, "Expected comma or closing bracket following property");
        }
    }

    this->expect(TokenType::CloseBrace, "Object literal missing closing brace.");
   
    auto object = std::make_unique<ObjectLiteral>();
    
    object->kind = NodeType::ObjectLiteral;
    object->properties = std::move(properties);
    
    return object;
}

std::unique_ptr<Expression> Parser::parseAdditiveExpression()
{
    auto left = this->parseMultiplicativeExpression();

    while (this->at().value == "+" || this->at().value == "-") {
        std::string _operator = this->eat().value;
        
        auto right = this->parseMultiplicativeExpression();
        auto binaryExpr = std::make_unique<BinaryExpression>();
        
        binaryExpr->kind = NodeType::BinaryExpression;
        binaryExpr->left = std::move(left);
        binaryExpr->right = std::move(right);
        
        return binaryExpr;
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseMultiplicativeExpression()
{
    auto left = this->parseCallMemberExpression();

    while (this->at().value == "/" || this->at().value == "*" || this->at().value == "%") {
        std::string _operator = this->eat().value;
        
        auto right = this->parseCallMemberExpression();

        auto binaryExpr = std::make_unique<BinaryExpression>();
        binaryExpr->kind = NodeType::BinaryExpression;
        binaryExpr->left = std::move(left);
        binaryExpr->right = std::move(right);
        
        return binaryExpr;
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseCallMemberExpression()
{
    auto member = this->parseMemberExpression();

    if (this->at().type == TokenType::OpenParen) {
        return this->parseCallExpression(std::move(member));
    }

    return member;
}

std::unique_ptr<Expression> Parser::parseCallExpression(std::unique_ptr<Expression> caller)
{
    auto callExpression = std::make_unique<CallExpression>();
    
    callExpression->kind = NodeType::CallExpression;
    callExpression->caller = std::move(caller);
    callExpression->args = std::move(this->parseArgs()); 

    if (this->at().type == TokenType::OpenParen) {
        return this->parseCallExpression(std::move(callExpression));
    }

    return callExpression;
}


std::vector<std::unique_ptr<Expression>> Parser::parseArgs()  
{
    this->expect(TokenType::OpenParen, "Expected open parenthesis");

    std::vector<std::unique_ptr<Expression>> args;  
    
    if (this->at().type != TokenType::CloseParen) {
        args = this->parseArgsList();
    }

    this->expect(TokenType::CloseParen, "Missing closing parenthesis inside arguments");
    
    return args;
}


std::vector<std::unique_ptr<Expression>> Parser::parseArgsList()
{
    std::vector<std::unique_ptr<Expression>> args;

    args.push_back(this->parseAssignmentExpression());

    while (this->at().type == TokenType::Comma) {
        this->eat(); 

        args.push_back(this->parseAssignmentExpression());
    }

    return args;
}

std::unique_ptr<Expression> Parser::parseMemberExpression()
{
    auto object = this->parsePrimaryExpression();

    while (this->at().type == TokenType::Dot || this->at().type == TokenType::OpenBracket) {
        auto _operator = this->eat();
        auto property = std::make_unique<Expression>();
        bool computed;

        if (_operator.type == TokenType::Dot) {
            computed = false;
            property = this->parsePrimaryExpression();

            if (property->kind != NodeType::Identifier) {
                throw "Cannot use a dot operator without right hand side being an identifier";
            }
        }
        else {
            computed = true;
            property = this->parseExpression();

            this->expect(TokenType::CloseBracket, std::string("Missing closing bracket in computed value"));
        }

        auto memberExpression = std::make_unique<MemberExpression>();

        memberExpression->kind = NodeType::MemberExpression;
        memberExpression->object = std::move(object);
        memberExpression->property = std::move(property);
        memberExpression->computed = computed;
    
        object = std::move(memberExpression);
    }

    return object;
}

std::unique_ptr<Expression> Parser::parsePrimaryExpression()
{
    auto token = this->at().type;
    
    switch (token) {
        case TokenType::Identifier: 
            return std::make_unique<_Identifier>(this->eat().value);
        case TokenType::Number:
            return std::make_unique<NumericLiteral>(this->eat().value);
        case TokenType::OpenParen: {
            this->eat();

            auto value = this->parseExpression();

            this->expect(TokenType::CloseParen, "Unexpected token found isnide parenthesised expression. Expected closing parenthessis.");

            return value;
        }

        default: {
            std::cerr << "Unexpected token found during parsing! " << this->at().value << std::endl;
            std::exit(1);
        }
    }
}

std::unique_ptr<Program> Parser::produceAST(std::string& sourceCode)
{
    this->tokens = Tokenize(sourceCode);

    auto program = std::make_unique<Program>();
    
    program->body = {};
    program->kind = NodeType::Program;

    while (this->not_EOF()) {
        program->body.push_back(this->parseStatement());
    }

    return program;
}
