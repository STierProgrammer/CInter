#include "interpreter.h"

std::shared_ptr<RuntimeValue> evaluate(const std::shared_ptr<Expression>& astNode, Environment& env)
{
	switch (astNode->kind)
	{
		case NodeType::NumericLiteral:
		{
			auto numericLiteral = std::dynamic_pointer_cast<NumericLiteral>(astNode);
			return std::make_shared<NumberValue>(numericLiteral->value);
		}

		case NodeType::BinaryExpression:
		{
			auto binaryExpression = std::dynamic_pointer_cast<BinaryExpression>(astNode);
			return evaluateBinaryExpression(*binaryExpression, env);
		}
		
		case NodeType::Identifier:
		{

		}

		
		case NodeType::Program:
		{

		}

		case NodeType::ObjectLiteral:
		{

		}

		case NodeType::VariableDeclaration:
		{

		}

		case NodeType::CallExpression:
		{

		}

		case NodeType::AssignmentExpression:
		{

		}

		default:
			std::cerr << "This AST Node has not yet been setup for interpretation. ";
			exit(0);
	}

	return nullptr;
}