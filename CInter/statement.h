#pragma once

#include "values.h"
#include "ast.h"
#include <memory>

std::unique_ptr<RuntimeValue> evaluateProgram(std::unique_ptr<Program> program);
std::unique_ptr<RuntimeValue> evaluateVariableDeclaration();