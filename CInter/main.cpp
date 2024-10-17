#include <iostream>
#include "lexer.h"
#include "parser.h"

int main() {
    std::string sourceCode = "let x = (5 + 10);";

    std::unique_ptr<Program> program = Parser::produceAST(sourceCode);

    std::cin.get();

    return 0;
}
