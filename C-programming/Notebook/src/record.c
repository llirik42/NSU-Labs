#include "record.h"
#include <string.h>
#include <math.h>
#include "utils.h"

int charToDigit(const char c){
    return c - '0';
}
bool isDigit(const char c){
    return c >= '0' && c <= '9';
}
bool convertStrToInt(const char str[], unsigned int* number){
    const unsigned int length = strlen(str);

    if (!str || length == 0){
        return false;
    }

    bool isFirstCharValid = isDigit(str[0]) && (length == 1 || str[0] != '0');
    if (!isFirstCharValid){
        return false;
    }

    *number = charToDigit(str[0]);

    for (unsigned int i = 1; i < length; i++){
        const char curChar = str[i];

        if (!isDigit(curChar)){
            return false;
        }

        *number = *number * 10 + charToDigit(curChar);
    }

    return true;
}
bool convertStrToDouble(const char* str, double* number){
    const unsigned int length = strlen(str);

    if (!str || length == 0){
        return false;
    }

    const bool isStringSingleChar = length == 1;
    if (isStringSingleChar){
        if (isDigit(str[0])){
            *number = charToDigit(str[0]);
            return true;
        }
        return false;
    }

    const bool isNumberNegative = str[0] == '-';
    const bool firstSymbolsCorrect = isDigit(str[0]) || (isNumberNegative && isDigit(str[1]));
    if (!firstSymbolsCorrect){
        return false;
    }

    *number = isNumberNegative ? charToDigit(str[1]) : charToDigit(str[0]);

    bool metPoint = false;
    int indexOfPoint = 0;
    const int startIndex = isNumberNegative ? 2 : 1;
    for (int i = startIndex; i < length; i++){
        const char curChar = str[i];

        if (curChar == '.'){
            if (metPoint){
                return false;
            }
            indexOfPoint = i;
            metPoint = true;
            continue;
        }

        if (!isDigit(curChar)){
            return false;
        }

        if (!metPoint){
            *number = *number * 10 + charToDigit(curChar);
            continue;
        }

        *number = *number + charToDigit(curChar) * pow(10, indexOfPoint - i);
    }

    if (isNumberNegative){
        *number = -(*number);
    }

    return true;
}
bool readRecordFields(char buffer[], struct Record* record, const char* separator){
    char* title = strtok(buffer, separator);

    char* placeIn2020Str = strtok(NULL, separator);

    char* placeIn2021Str = strtok(NULL, separator);

    char* ratingStr = strtok(NULL, separator);

    char* changeStr = strtok(NULL, separator);

    if (!title || !placeIn2020Str || !placeIn2021Str || !ratingStr || !changeStr){
        return false;
    }

    strcpy(record->title, title);

    bool conversionError = !convertStrToInt(placeIn2020Str, &record->placeIn2020) ||
                           !convertStrToInt(placeIn2021Str, &record->placeIn2021) ||
                           !convertStrToDouble(ratingStr, &record->rating) ||
                           !convertStrToDouble(changeStr, &record->change);

    return !conversionError;
}
struct Record createRecordFromString(char string[]){
    struct Record newRecord;

    if (!readRecordFields(string, &newRecord, SEPARATOR)){
        return EMPTY_RECORD;
    }

    return newRecord;
}

struct Record InputRecord(){
    struct Record newRecord;

    bool inputError = !InputString(newRecord.title, "Title") ||
                      !InputInt(&newRecord.placeIn2020, "Place in 2020") ||
                      !InputInt(&newRecord.placeIn2021, "Place in 2021") ||
                      !InputDouble(&newRecord.rating, "Rating") ||
                      !InputDouble(&newRecord.change, "Change");

    if (inputError){
        ClearStdin();
        return EMPTY_RECORD;
    }

    return newRecord;
}
void PrintRecord(const struct Record record){
    if (IsRecordEmpty(record)){
        printf("%stitle=%20s   placeIn2020=%3s   placeIn2021=%3s   rating=%10s   change=%10s", OUT_PREFIX,
               "XXXXXXXXXXXXXXXXXXXX", "XXX", "XXX", "XXXXXXXXXX", "XXXXXXXXXX");
        return;
    }

    printf("%stitle=%20s   placeIn2020=%3u   placeIn2021=%3u   rating=%10f   change=%10f",
           OUT_PREFIX, record.title, record.placeIn2020, record.placeIn2021, record.rating, record.change);
}
void SaveRecordToFile(FILE* file, const struct Record record){
    if (!file){
        return;
    }

    fprintf(file, "%s,%u,%u,%f,%f", record.title, record.placeIn2020, record.placeIn2021,
            record.rating, record.change);
}
bool LoadRecord(FILE* file, struct Record* record){
    if (!file){
        *record = EMPTY_RECORD;
        return false;
    }

    char buffer[MAX_FIELD_LENGTH];

    int curChar = fgetc(file);

    unsigned int curIndex = 0;
    while (curChar != EOF){
        if (curChar == '\n'){
            buffer[curIndex] = '\0';
            *record = createRecordFromString(buffer);
            return true;
        }

        buffer[curIndex++] = (char) curChar;

        curChar = fgetc(file);
    }

    *record = EMPTY_RECORD;
    return false;
}
bool IsRecordEmpty(const struct Record record){
    return record.title[0] == ' ';
}
