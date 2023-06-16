#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// максимальная длина вводимого числа
#define MAX_LENGTH_OF_INPUT_NUMBER 14

// максимальная длина дробной части
#define MAX_LENGTH_OF_FRACTIONAL_PART 13

// максимальная длина числа в новой системе счисления
// учитывая, что максимальное число, которое можно получить на входе это
// "fffffffffffff", то максимальное по длине число, получаемое на выходе = '1' * 52
#define MAX_LENGTH_OF_OUTPUT_NUMBER 53

// Длины больше на 1, чем должны быть, чтобы оставалась ячейка памяти под '\0'


void printBadInput(){
    // вывести сообщения о неправильном вводе данных

    printf("bad input");
}

void inputBase(unsigned int *base) {
    // ввод основания системы счисления

    if(!scanf("%u", base)){
        printBadInput();

        exit(0);
    }
}

void inputNumber(char *number){
    // ввод числа, которое нужно перевести

    if(!scanf("%14s", number)){
        printBadInput();

        exit(0);
    }
}



bool isBaseCorrect(const unsigned int base){
    // корректно ли основание системы счисления

    return base >= 2 && base <= 16;
}

bool isCharPoint(const char c){
    // является ли символ символом точки

    return c == '.';
}

unsigned char letterToDecimalNumber(const char letter){
    // перевод буквы в десятичное число ('f' -> 15, 'a' -> 10)

    unsigned char result = (char) tolower(letter) - 'a' + 10;

    if (result < 10 || result > 15){
        return -1;
    }

    return result;
}

unsigned char charOfDigitToDecimalDigit(const char c){
    // перевод символа в цифру ('0' -> 0, '9' -> 9)

    unsigned char result = (unsigned char) (c - '0');

    if (result > 9){
        return -1;
    }

    return result;
}

unsigned char charToDecimalNumber(const char c){
    // функция, совмещающая в себе работу функций "charOfDigitToDecimalDigit" и "letterToDecimalNumber"

    if (isalnum(c)){
        if (isalpha(c)){
            return letterToDecimalNumber(c);
        }
        return charOfDigitToDecimalDigit(c);
    }

    return -1;
}

bool isNumberCorrect(const char number[], const unsigned int base){
    // является ли введённое число корректным

    unsigned char length = strlen(number);

    if(isCharPoint(number[0]) || isCharPoint(number[length - 1])){
        return false;
    }

    //pointsCounter - счётчик количества вхождения символа '.' в число
    for (unsigned char curIndex = 0, pointsCounter = 0; curIndex < length; curIndex++){
        char curChar = number[curIndex]; // текущий символ в строковой записи числа

        bool isCharCorrect = isalnum(curChar) || isCharPoint(curChar);
        if (!isCharCorrect){
            return false;
        }

        if (isCharPoint(curChar)){
            pointsCounter++;

            if (pointsCounter > 1){
                return false;
            }

            continue;
        }

        unsigned char curDigit = charToDecimalNumber(curChar); // текущая цифра числа [0; 16]

        if (curDigit >= base){
            return false;
        }
    }

    return true;
}

unsigned char getLenOfIntPart(const char number[]){
    // получение длины целой части числа

    int curIndex = 0;

    unsigned char length = strlen(number);

    for (; curIndex < length; curIndex++){
        if (number[curIndex] == '.'){
            break;
        }
    }

    return curIndex;
}

double toDecimal(const char number[], const unsigned int oldBase){
    // перевод числа в десятичную систему счисления

    double result = 0; // результат перевода

    unsigned char length = strlen(number);

    char curPower = (char)(getLenOfIntPart(number) - 1); // текущая степень основания системы
    for (unsigned char curIndex = 0; curIndex < length; curIndex++){
        char curChar = number[curIndex]; // текущий символ в записи числа
        if (curChar == '.'){
            continue;
        }

        unsigned char curDigit = charToDecimalNumber(curChar); // текущая цифра в числе [0; 16]

        result += curDigit * pow(oldBase, curPower);

        curPower--;
    }

    return result;
}

char decimalNumberToLetter(const unsigned char decimalNumber){
    // перевод десятичного числа в соответствующую ему букву (10 -> 'a', 'f' -> 15)

    if (decimalNumber < 10 || decimalNumber > 15){
        return ' ';
    }

    return (char)((int)'a' + decimalNumber - 10);
}

char decimalDigitToNonLetterChar(const unsigned char digit){
    // перевод десятичной цифры в соответствующий ей символ

    if (digit > 9){
        return ' ';
    }

    return (char) (digit + '0');
}

char decimalDigitToChar(const unsigned char digit){
    // функция, совмещающая в себе работу функций "decimalDigitToNonLetterChar" и "decimalNumberToLetter"

    if (digit < 10){
        return decimalDigitToNonLetterChar(digit);
    }

    return decimalNumberToLetter(digit);
}

void swap(char* pointer1, char* pointer2){
    // Обмен переменных значениями

    char tmp = *pointer1;
    *pointer1 = *pointer2;
    *pointer2 = tmp;
}

void reversePartOfString(char string[], const unsigned char amountToReverse) {
    // функция, переворачивающая подстроку в строке "string", состоящую из первых "amountToReverse" элементов

    for (unsigned char curIndex = 0; curIndex < amountToReverse / 2; curIndex++) {
        unsigned int indexToSwap = amountToReverse - curIndex - 1;

        swap(&string[curIndex], &string[indexToSwap]);
    }
}

void convertDecimalInteger(unsigned long long int decimalInteger, const unsigned int newBase,
                           unsigned char* lenOfIntPartInNewBase, char numberInNewBase[]){
    // функция по переводу целого десятичного числа в другую систему счисления

    for(unsigned char curIndex = 0; curIndex < MAX_LENGTH_OF_OUTPUT_NUMBER; curIndex++, decimalInteger /= newBase){
        if (decimalInteger == 0){
            (*lenOfIntPartInNewBase) = curIndex;
            break;
        }

        numberInNewBase[curIndex] = decimalDigitToChar(decimalInteger % newBase);
    }

    if (*lenOfIntPartInNewBase == 0){
        *lenOfIntPartInNewBase = 1;
        numberInNewBase[0] = '0';
    }

    reversePartOfString(numberInNewBase, *lenOfIntPartInNewBase);
}

void convertFromDecimalFraction(double decimalFraction, const unsigned int newBase,
                                unsigned char* lenOfFractionPartInNewBase, char fractionInNewBase[]){
    // функцию по переводу не целого десятичного числа < 1 в другую систему счисления

    for (unsigned char curIndex = 0; curIndex < MAX_LENGTH_OF_FRACTIONAL_PART; curIndex++){
        if (decimalFraction == 0){
            break;
        }

        decimalFraction *= newBase;

        unsigned int curIntegerPart = (int) decimalFraction; // текущая целая часть числа при переводе

        fractionInNewBase[curIndex] = decimalDigitToChar(curIntegerPart);

        decimalFraction -= curIntegerPart;

        (*lenOfFractionPartInNewBase)++;
    }
}

void convertFromDecimal(const double decimalNumber, const unsigned int newBase,
                        unsigned char *lenOfNumberInNewBase, char numberInNewBase[]){
    // функция, совмещающая в себе работу функций "convertFromDecimalFraction" и "convertDecimalInteger"

    // целая часть в десятичной системе
    unsigned long long int oldIntegerPart = (unsigned long long int) decimalNumber;

    unsigned char lenOfIntPartInNewBase = 0; // длина целой части в новой системе счисления
    char intPartInNewBase[MAX_LENGTH_OF_OUTPUT_NUMBER]; // целая часть в новой системе счисления
    convertDecimalInteger(oldIntegerPart, newBase, &lenOfIntPartInNewBase, intPartInNewBase);

    double oldFractionalPart = decimalNumber - (double) oldIntegerPart; // дробная часть в десятичной системе
    unsigned char lenOfFractionPartInNewBase = 0; // длина дробной части в новой системе счисления
    char fractionalPartInNewBase[MAX_LENGTH_OF_FRACTIONAL_PART]; // дробная часть в новой системе счисления
    convertFromDecimalFraction(oldFractionalPart, newBase, &lenOfFractionPartInNewBase, fractionalPartInNewBase);

    // максимальное значение индекса в цикле (целая часть + точка + дробная часть)
    unsigned char stop = lenOfIntPartInNewBase + 1 + MAX_LENGTH_OF_FRACTIONAL_PART;

    for (unsigned char curIndex = 0, indexOfFractionalPart = 0; curIndex < stop; curIndex++, (*lenOfNumberInNewBase)++){
        // символ, который сейчас нужно записать в результат (в число в новой системе счисления)
        char charToWriteToResult = '0';

        if (curIndex < lenOfIntPartInNewBase){
            charToWriteToResult = intPartInNewBase[curIndex];
        }

        if(curIndex == lenOfIntPartInNewBase && lenOfFractionPartInNewBase == 0){
            break;
        }

        if(curIndex == lenOfIntPartInNewBase){
            charToWriteToResult = '.';
        }

        if(curIndex > lenOfIntPartInNewBase){
            if(indexOfFractionalPart < lenOfFractionPartInNewBase){
                charToWriteToResult = fractionalPartInNewBase[indexOfFractionalPart];
                indexOfFractionalPart++;
            }
            else{
                charToWriteToResult = '0';
            }
        }

        numberInNewBase[curIndex] = charToWriteToResult;
    }
}

void printResult(const char newBaseNumber[], const unsigned char length){
    // вывод результата работы программы

    for (unsigned char curIndex = 0; curIndex < length; curIndex++){
        printf("%c", newBaseNumber[curIndex]);
    }
}

int main() {
    unsigned int oldBase; // старое основание системы (из какой системы счисления нужно перевести)
    unsigned int newBase; // новое основание системы (в какую систему счисления нужно перевести)
    char oldSystemNumber[MAX_LENGTH_OF_INPUT_NUMBER + 1] = {'\0'}; // число в старой системе счисления

    // ввод данных
    inputBase(&oldBase);
    inputBase(&newBase);
    inputNumber(oldSystemNumber);

    bool isInputDataCorrect; // корректны ли введённые данные
    isInputDataCorrect = isBaseCorrect(oldBase) && isBaseCorrect(newBase) && isNumberCorrect(oldSystemNumber, oldBase);

    if (isInputDataCorrect){
        // введённое число, переведённое в десятичную систему счисления
        double decimalOldNumber = toDecimal(oldSystemNumber, oldBase);

        unsigned char lenOfNewNumber = 0; // длина числа в новой системе счисления
        char newSystemNumber[MAX_LENGTH_OF_OUTPUT_NUMBER]; // число в новой системе счисления
        convertFromDecimal(decimalOldNumber, newBase, &lenOfNewNumber, newSystemNumber);
        printResult(newSystemNumber, lenOfNewNumber);

        return EXIT_SUCCESS;
    }

    printBadInput();

    return EXIT_SUCCESS;
}
