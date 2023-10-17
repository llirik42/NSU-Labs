#pragma once

#include <stdio.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 1024

struct String{
    char chars[MAX_STRING_LENGTH];
    unsigned int length;
};

struct String createString();

void inputString(struct String* string, FILE* file);

bool isStringEmpty(const struct String* string);

unsigned int getStringLength(const struct String* string);

char getStringChar(const struct String* string, unsigned int index);
