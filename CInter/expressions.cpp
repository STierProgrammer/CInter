#include "expressions.h"

std::shared_ptr<NumberValue> evaluateNumericBinaryExpression(const NumberValue& lhs, const NumberValue& rhs, const std::string& _operator) {
	double result;

	if (_operator == "+")
		result = lhs.value + rhs.value;

	else if (_operator == "-")
		result = lhs.value - rhs.value;

	else if (_operator == "*")
		result = lhs.value * rhs.value;

	else if (_operator == "/")
		result = lhs.value / rhs.value;

	else
		result = std::fmod(lhs.value, rhs.value);

	auto numberValue = std::make_unique<NumberValue>();
	
	numberValue->value = result;
	numberValue->getType();

	return numberValue;
}

std::shared_ptr<RuntimeValue> evaluateBinaryExpression(BinaryExpression& binop, Environment& env) {

}

std::shared_ptr<_Identifier> evaluateIdentifier(_Identifier& ident, Environment& env)
{
	
}

std::shared_ptr<RuntimeValue> evaluateAssignment(AssignmentExpression& node, Environment& env)
{
	
}

std::shared_ptr<RuntimeValue> evaluateObjectExpression(ObjectLiteral& obj, Environment& env)
{
	
}

std::shared_ptr<RuntimeValue> evaluateCallExpression(CallExpression& expression, Environment& env)
{
	
}
