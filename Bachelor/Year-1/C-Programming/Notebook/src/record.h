#pragma once

#include <stdio.h>
#include <stdbool.h>

#define MAX_RECORD_TITLE_LENGTH 64
#define MAX_FIELD_LENGTH 128

struct Record{
    char title[MAX_RECORD_TITLE_LENGTH];
    unsigned int placeIn2020;
    unsigned int placeIn2021;
    double rating;
    double change;
};

struct Record InputRecord();
void PrintRecord(struct Record record);
void SaveRecordToFile(FILE* file, struct Record record);
bool LoadRecord(FILE* file, struct Record* record);
bool IsRecordEmpty(struct Record record);

static const struct Record EMPTY_RECORD = {.title = " "};
