#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "environment.h"

enum class ValueType {
	Null,
	Number,
	Boolean,
	Object,
	nativeFunction
};

struct RuntimeValue {
	virtual ~RuntimeValue() = default;
	virtual ValueType getType() const = 0;
};

struct NullValue : public RuntimeValue {
	ValueType getType() const override {
		return ValueType::Null;
	}

	std::nullptr_t value = nullptr;
};

struct BooleanValue : public RuntimeValue {
	ValueType getType() const override {
		return ValueType::Boolean;
	}

	bool value;

	BooleanValue(bool val = true) : value(val) {}
};

struct NumberValue : public RuntimeValue {
	ValueType getType() const override {
		return ValueType::Number;
	}

	double value;

	NumberValue(double val = 0.0) : value(val) {}
};

struct ObjectValue : public RuntimeValue {
	ValueType getType() const override {
		return ValueType::Object;
	}

	std::map<std::string, std::unique_ptr<RuntimeValue>> properties;
};

using FunctionCall = std::function<RuntimeValue>(const std::vector<RuntimeValue>&, Environment&);

struct NativeFunctionValue : public RuntimeValue {
	ValueType getType() const override {
		return ValueType::nativeFunction;
	}

	FunctionCall call;
};

std::unique_ptr<NullValue> MAKE_NULL();
std::unique_ptr<NumberValue> MAKE_NUMBER(double n = 0.0);
std::unique_ptr<BooleanValue> MAKE_BOOL(bool b = true);
std::unique_ptr<NativeFunctionValue> MAKE_NATIVE_FUNCTION(FunctionCall call);