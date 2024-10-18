#pragma once

#include "ast.h"
#include "values.h"
#include "expressions.h"

std::shared_ptr<RuntimeValue> evaluate(std::shared_ptr<Statement>& astNode, Environment& env);