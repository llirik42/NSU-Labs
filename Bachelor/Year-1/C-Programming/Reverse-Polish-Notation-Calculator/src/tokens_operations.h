#pragma once

#include "status_code.h"
#include "token.h"


struct StatusCode tokenSum(struct Token operand1, struct Token operand2);

struct StatusCode tokenSub(struct Token operand1, struct Token operand2);

struct StatusCode tokenMul(struct Token operand1, struct Token operand2);

struct StatusCode tokenDiv(struct Token operand1, struct Token operand2);

struct StatusCode tokenPow(struct Token operand1, struct Token operand2);
