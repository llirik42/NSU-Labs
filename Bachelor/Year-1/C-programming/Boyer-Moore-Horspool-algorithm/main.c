#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LENGTH_OF_PATTERN 16 // Максимальная длина шаблона

#define BUFFER_SIZE 1024 * 512 // Размер буфера

#define POSSIBLE_SYMBOLS_COUNT 256 // Количество символов в алфавите

struct Buffer{
    // Буфер

    char array[BUFFER_SIZE]; // элементы буфера
    size_t index; // индекс нового элемента
};

void initBuffer(struct Buffer* buffer){
    // Инициализация буфера

    buffer->index = 0;
}

void shiftBuffer(struct Buffer* buffer, size_t count){
    // Перемещает последние "count" элементов буфера в начало

    for(size_t index = 0; index < count; index++){
        buffer->array[index] = buffer->array[BUFFER_SIZE + index - count - 1];
    }

    buffer->index = count;
}

bool readToBuffer(struct Buffer* buffer, const unsigned char count){
    // Прочитать из stdin первые "count" символов, сложить их в буфер и вернуть
    // false - получилось считать символов меньше, чем "count"
    // true - получилось считать все "count" символов

    size_t countOfRead = fread(buffer->array + buffer->index, sizeof(char), count,
                               stdin);
    if (count != countOfRead){
        return false;
    }

    buffer->index += count;

    return true;
}

bool hasCapacity(const struct Buffer* buffer, const unsigned char shift){
    // Есть ли у буфера запас на добавление в него "shift" элементов

    return buffer->index + shift <= BUFFER_SIZE;
}

char getElement(const struct Buffer* buffer, const unsigned char index){
    // Получение "index"-ого элемента с конца

    return buffer->array[buffer->index - index - 1];
}

void inputPattern(char* pattern){
    // Ввод шаблона

    for (unsigned char curIndex = 0; curIndex < MAX_LENGTH_OF_PATTERN + 1; curIndex++){
        int curChar = getchar();

        if (curChar == '\n'){
            pattern[curIndex] = '\0';

            return;
        }

        pattern[curIndex] = (char) curChar;
    }
}

void fillShiftTable(size_t* shiftTable, const char* pattern, const size_t patternLen){
    // Заполнение таблицы сдвигов

    for (size_t index = 0; index < POSSIBLE_SYMBOLS_COUNT; index++){
        shiftTable[(unsigned char) index] = patternLen;
    }

    for (size_t index = patternLen - 1; index > 0; index--){
        unsigned char curChar = (unsigned char) pattern[index - 1];

        if (shiftTable[curChar] == patternLen){
            shiftTable[curChar] = patternLen - index;
        }
    }
}

int main() {
    char pattern[MAX_LENGTH_OF_PATTERN + 1];
    inputPattern(pattern);
    unsigned char lenOfPattern = strlen(pattern);

    size_t shiftTable[POSSIBLE_SYMBOLS_COUNT];
    fillShiftTable(shiftTable, pattern, lenOfPattern);

    struct Buffer buffer;
    initBuffer(&buffer);

    if (!readToBuffer(&buffer, lenOfPattern)) {
        return EXIT_SUCCESS;
    }

    size_t subStringIndex = 0; // позиция в тексте
    while (1) {
        unsigned char shift = lenOfPattern; // на сколько символов нужно сдвинуться в тексте

        for (unsigned char i = 0; i < lenOfPattern; i++) {
            size_t indexInPattern = lenOfPattern - i - 1;
            size_t indexInText = lenOfPattern + subStringIndex - i - 1;

            printf("%zu ", indexInText + 1);

            if (getElement(&buffer, i) != pattern[indexInPattern]) {
                unsigned char shiftChar = i == 0 ?
                                          getElement(&buffer, 0) :
                                          pattern[lenOfPattern - 1];

                shift = shiftTable[shiftChar];
                break;
            }
        }

        subStringIndex += shift;

        if (!hasCapacity(&buffer, shift)){
            size_t bufferShiftCount = BUFFER_SIZE - buffer.index + lenOfPattern;
            shiftBuffer(&buffer, bufferShiftCount);
        }

        if (!readToBuffer(&buffer, shift)){
            return EXIT_SUCCESS;
        }
    }
}
