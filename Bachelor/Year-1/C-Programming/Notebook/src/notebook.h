#pragma once

#include <stdbool.h>
#include "record.h"

typedef char* Response;
#define SUCCESS 0

struct Notebook;

Response CreateNotebook(struct Notebook** notebook);
Response DestroyNotebook(struct Notebook* notebook);
Response AddRecord(struct Notebook* notebook, struct Record record);
Response PrintNotebook(const struct Notebook* notebook);
Response Delete(struct Notebook* notebook, const char title[]);
Response LoadNotebook(struct Notebook* notebook, const char filePath[]);
Response SaveNotebook(struct Notebook* notebook, const char filePath[]);
Response FindByTitle(struct Notebook* notebook, const char title[]);
Response FindByNumericField(struct Notebook* notebook, const char fieldTitle[], double min, double max);
