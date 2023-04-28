#include <stdbool.h>
#include <stdio.h>
#include "interaction.h"
#include "quantities_group.h"
#include "quantities_config.h"

void printEndOfChoiceMessage(){
    printf("\n");
}
void printPartOfChoiceMessage(const unsigned int curIndex, const char* curTitle){
    if (curIndex){
        printf(", ");

    }
    printf("%d for %s", curIndex + 1, curTitle);
}
void printQuantityChoice(const struct QuantityGroup* group){
    printf("\nYou have to choose old and new quantities. Type two numbers, ");

    for (unsigned int curIndex = 0; curIndex < getGroupQuantitiesCounter(group); curIndex++){
        struct Quantity curQuantity = getGroupQuantity(group, curIndex);

        printPartOfChoiceMessage(curIndex, getQuantityTitle(curQuantity));
    }

    printEndOfChoiceMessage();
}
void printGroupChoice(const struct QuantityGroup* groups[], const unsigned int groupsCounter){
    printf("You have to choose group of quantities. Type ");

    for (unsigned int curIndex = 0; curIndex < groupsCounter; curIndex++){
        printPartOfChoiceMessage(curIndex, getGroupTitle(groups[curIndex]));
    }

    printEndOfChoiceMessage();
}
void printValueInput(){
    printf("\nType value to convert\n");
}
void printWrongDataError(){
    printf("Wrong input data!\n\n");
}
void printResult(const double result){
    printf("\nResult: %lf\n\n\n", result);
}
void cleanStdin(){
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
void endIterationWithError(){
    printWrongDataError();
    cleanStdin();
}

bool inputIndex(int* index){
    const unsigned int countOfRead = scanf("%d", index);
    if (countOfRead == 0){
        return true;
    }
    return false;
}
bool inputValue(double* quantityValue){
    const unsigned int countOfRead = scanf("%lf", quantityValue);
    if (countOfRead == 0){
        return true;
    }
    return false;
}

bool isIndexCorrect(const unsigned int index, const unsigned int maxIndex){
    return index >= 1 && index <= maxIndex;
}
bool isValueCorrect(const double value, const double minValue){
    return value >= minValue;
}

void startInteraction(){
    while (1){
        int groupIndex = 0;
        printGroupChoice(GROUP_POINTERS, GROUPS_COUNTER);
        if (inputIndex(&groupIndex) || !isIndexCorrect(groupIndex, GROUPS_COUNTER)){
            endIterationWithError();
            continue;
        }
        const struct QuantityGroup* currentGroup = GROUP_POINTERS[groupIndex - 1];
        const unsigned int maxQuantityIndex = getGroupQuantitiesCounter(currentGroup);

        int oldQuantityIndex = 0;
        printQuantityChoice(currentGroup);
        if (inputIndex(&oldQuantityIndex) || !isIndexCorrect(oldQuantityIndex, maxQuantityIndex)){
            endIterationWithError();
            continue;
        }

        int newQuantityIndex = 0;
        if (inputIndex(&newQuantityIndex) || !isIndexCorrect(newQuantityIndex, maxQuantityIndex)){
            endIterationWithError();
            continue;
        }
        const struct Quantity oldQuantity = getGroupQuantity(currentGroup, oldQuantityIndex - 1);
        const struct Quantity newQuantity = getGroupQuantity(currentGroup, newQuantityIndex - 1);

        double oldQuantityValue = 0;
        const double oldQuantityMinValue = getQuantityMinValue(oldQuantity, *currentGroup);
        printValueInput();
        if (inputValue(&oldQuantityValue) || !isValueCorrect(oldQuantityValue, oldQuantityMinValue)){
            endIterationWithError();
            continue;
        }

        printResult(convert(oldQuantityValue, oldQuantity, newQuantity));
    }
}
