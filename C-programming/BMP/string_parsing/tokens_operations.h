#pragma once

#include "token.h"
#include "../utils/utils.h"

#define PI_NUMBER 3.14159265358979323846264338327950288419716939937510
#define E_NUMBER 2.71828182845904523536028747135266249775724709369995

typedef struct AdvancedValue OperationResult;

#define UNARY_ARGS struct Token operand
#define BINARY_ARGS struct Token operand1, struct Token operand2

OperationResult Sum(BINARY_ARGS);
OperationResult Sub(BINARY_ARGS);
OperationResult Mul(BINARY_ARGS);
OperationResult Div(BINARY_ARGS);
OperationResult Pow(BINARY_ARGS);
OperationResult Sin(UNARY_ARGS);
OperationResult Cos(UNARY_ARGS);
OperationResult Tg(UNARY_ARGS);
OperationResult Ctg(UNARY_ARGS);
OperationResult Arcsin(UNARY_ARGS);
OperationResult Arccos(UNARY_ARGS);
OperationResult Arctg(UNARY_ARGS);
OperationResult Arcctg(UNARY_ARGS);
OperationResult Sqrt(UNARY_ARGS);
OperationResult Ln(UNARY_ARGS);
OperationResult Log(BINARY_ARGS);
