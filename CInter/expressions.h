#pragma once

#include "values.h"
#include "ast.h"
#include "interpreter.h"
#include <memory>
#include <cmath>  

std::shared_ptr<NumberValue> evaluateNumericBinaryExpression(const NumberValue& lhs, const NumberValue& rhs, const std::string& _operator);
std::shared_ptr<RuntimeValue> evaluateBinaryExpression(const BinaryExpression& binop, const Environment& env);
std::shared_ptr<_Identifier> evaluateIdentifier(_Identifier& ident, Environment& env);
std::shared_ptr<RuntimeValue> evaluateAssignment(AssignmentExpression& node, Environment& env);
std::shared_ptr<RuntimeValue> evaluateObjectExpression(ObjectLiteral& obj, Environment& env);
std::shared_ptr<RuntimeValue> evaluateCallExpression(CallExpression& expression, Environment& env);
