#include "values.h"

std::unique_ptr<NullValue> MAKE_NULL()
{
    return std::make_unique<NullValue>();
}

std::unique_ptr<NumberValue> MAKE_NUMBER(double n)
{
    return std::make_unique<NumberValue>(n);
}

std::unique_ptr<BooleanValue> MAKE_BOOL(bool b)
{
    return std::make_unique<BooleanValue>(b);
}