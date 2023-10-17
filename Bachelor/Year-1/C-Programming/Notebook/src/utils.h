#pragma once

#include <stdbool.h>

#define OUT_PREFIX "out < "
#define IN_PREFIX "in > "
#define SEPARATOR ","

bool CompareStrings(const char string1[], const char string2[], unsigned int maxLength);
void ClearStdin();
void PrintWithPrefix(const char string[]);
bool InputString(char string[], const char title[]);
bool InputInt(unsigned int* number, const char title[]);
bool InputDouble(double * number, const char title[]);
bool InputCommand(char command[]);
