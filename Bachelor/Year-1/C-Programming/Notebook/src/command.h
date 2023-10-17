#pragma once

#include "notebook.h"

#define MAX_COMMAND_TITLE_LENGTH 32

struct Command;

void Execute(const char title[], struct Notebook* notebook);
