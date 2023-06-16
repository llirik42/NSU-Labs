#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH_OF_PATTERN 16 // максимальная длина шаблона

#define BUFFER_SIZE 100000

// степени тройки
size_t DEGREES_OF_3[MAX_LENGTH_OF_PATTERN] =
        {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049, 177147, 531441, 1594323, 4782969, 14348907};

bool isCharEndOfStream(const int c){
    // является ли символ концом потока ввода

    return c == EOF;
}

bool isCharEndOfLine(const int c){
    // является ли символ концом строки

    return c == 10;
}

void inputPattern(char* pattern){
    // ввод шаблона

    for (unsigned char curIndex = 0; curIndex < MAX_LENGTH_OF_PATTERN + 1; curIndex++){
        int curChar = getchar();

        if (isCharEndOfLine(curChar)){
            pattern[curIndex] = '\0';

            return;
        }

        pattern[curIndex] = (char) curChar;
    }
}

size_t getPartialHash(const char c, const size_t multiplier){
    // получение "частичного хеша" символа

    return (((unsigned char)c) % 3) * multiplier;
}

size_t getHashOfString(const char string[], const unsigned char length){
    // получение хеша строки

    size_t result = 0;

    for (unsigned char curIndex = 0; curIndex < length; curIndex++){
        result += getPartialHash(string[curIndex], (size_t) DEGREES_OF_3[curIndex]);
    }

    return result;
}

size_t getHashByPrevHash(size_t prevHash, char firstChar, int lastChar, size_t multiplier){
    // получение хеша подстроки с помощью хеша предыдущей подстроки

    return (prevHash - ((unsigned char) firstChar % 3)) / 3 + getPartialHash((char) lastChar, multiplier);
}

void shiftString(char string[], const size_t length, const unsigned char count){
    // сдвиг последних "count" символов в начало

    for(unsigned char index = 0; index < count; index++){
        string[index] = string[length - count - 1 + index];
    }
}

void compareByChars(const char s1[], const char s2[], const unsigned char len, const size_t shift, const size_t startIndex){
    // посимвольное сравнение двух строк

    for (unsigned char curIndex = 0; curIndex < len; curIndex++){
        printf("%zu ", curIndex + shift + 1);

        if (s1[curIndex + startIndex] != s2[curIndex]){
            return;
        }
    }
}

int main() {
    char pattern[MAX_LENGTH_OF_PATTERN + 1]; // шаблон

    inputPattern(pattern);

    unsigned char lenOfPattern = strlen(pattern);

    char buffer[BUFFER_SIZE]; // буфер для хранения введённых символов
    size_t countOfFirstReadSymbols = fread(buffer, 1, lenOfPattern, stdin); // количество считанных
    // символов первой подстроки

    size_t hashOfPattern = getHashOfString(pattern, lenOfPattern); // хеш шаблона

    size_t curHash = getHashOfString(buffer, lenOfPattern); // хеш текущей подстроки

    printf("%zu ", hashOfPattern);

    if (curHash == hashOfPattern && countOfFirstReadSymbols != 0){
        compareByChars(pattern, buffer, lenOfPattern, 0, 0);
    }

    size_t subStringIndex = 0; // позиция первого символа текущей подстроки в тексте
    size_t indexInBuffer = lenOfPattern; // индекс только что считанного символа в буфере
    size_t maxMultiplier = DEGREES_OF_3[lenOfPattern - 1];
    while (1){
        int curChar = getchar();

        if (isCharEndOfStream(curChar)){
            break;
        }

        buffer[indexInBuffer] = (char) curChar;

        curHash = getHashByPrevHash(curHash, buffer[indexInBuffer - lenOfPattern], buffer[indexInBuffer], maxMultiplier);

        if (indexInBuffer == BUFFER_SIZE - 1){
            shiftString(buffer, BUFFER_SIZE, lenOfPattern);
            indexInBuffer = lenOfPattern;
        }

        if (curHash == hashOfPattern){
            compareByChars(buffer, pattern, lenOfPattern, subStringIndex + 1, indexInBuffer - lenOfPattern + 1);
        }
        subStringIndex++;
        indexInBuffer++;
    }

    return EXIT_SUCCESS;
}
