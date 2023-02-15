#include "notebook.h"
#include <malloc.h>
#include <stdio.h>
#include "utils.h"

#define SUCCESS 0
#define NO_MEMORY "no memory"
#define DESTROYING_ERROR "destroying error"
#define ADDING_ERROR "adding error"
#define PRINTING_ERROR "printing error"
#define EMPTY_NOTEBOOK_ERROR "Notebook is empty"
#define DELETING_ERROR "deleting error"
#define NO_RECORDS_FOUND_ERROR "no records found"
#define LOADING_ERROR "loading error"
#define WRONG_PATH_ERROR "wrong path"
#define SAVING_ERROR "saving error"
#define FINDING_ERROR "finding error"
#define WRONG_FIELD_TITLE_ERROR "wrong field"

struct NotebookElement{
    struct NotebookElement* next;
    struct Record value;
};

struct Notebook{
    struct NotebookElement* head;
    struct NotebookElement* tail;
};

Response findByPlaceIn2020(struct Notebook* notebook, const double min, const double max){
    bool noResultsFound = true;

    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        const double curValue = curElement->value.placeIn2020;
        if (min <= curValue && curValue <= max){
            PrintRecord(curElement->value);
            printf("\n");
            noResultsFound = false;
        }
        curElement = curElement->next;
    }

    if (noResultsFound){
        return NO_RECORDS_FOUND_ERROR;
    }
    return SUCCESS;
}
Response findByPlaceIn2021(struct Notebook* notebook, const double min, const double max){
    bool noResultsFound = true;

    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        const double curValue = curElement->value.placeIn2021;
        if (min <= curValue && curValue <= max){
            PrintRecord(curElement->value);
            printf("\n");
            noResultsFound = false;
        }
        curElement = curElement->next;
    }

    if (noResultsFound){
        return NO_RECORDS_FOUND_ERROR;
    }
    return SUCCESS;
}
Response findByRating(struct Notebook* notebook, const unsigned int min, const unsigned int max){
    bool noResultsFound = true;

    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        const double curValue = curElement->value.rating;
        if (min <= curValue && curValue <= max){
            PrintRecord(curElement->value);
            printf("\n");
            noResultsFound = false;
        }
        curElement = curElement->next;
    }

    if (noResultsFound){
        return NO_RECORDS_FOUND_ERROR;
    }
    return SUCCESS;
}
Response findByChange(struct Notebook* notebook, const unsigned int min, const unsigned int max){
    bool noResultsFound = true;

    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        const double curValue = curElement->value.change;
        if (min <= curValue && curValue <= max){
            PrintRecord(curElement->value);
            printf("\n");
            noResultsFound = false;
        }
        curElement = curElement->next;
    }

    if (noResultsFound){
        return NO_RECORDS_FOUND_ERROR;
    }
    return SUCCESS;
}
struct NotebookElement* createNotebookElement(const struct Record record){
    struct NotebookElement* newElement = malloc(sizeof(struct NotebookElement));

    if (!newElement){
        return NULL;
    }

    newElement->value = record;
    newElement->next = NULL;

    return newElement;
}
void initNotebook(struct Notebook* notebook){
    notebook->head = NULL;
    notebook->tail = NULL;
}

Response CreateNotebook(struct Notebook** notebook){
    *notebook = malloc(sizeof(struct Notebook));

    if (!(*notebook)){
        return NO_MEMORY;
    }

    initNotebook((*notebook));

    return SUCCESS;
}
Response DestroyNotebook(struct Notebook* notebook){
    if (!notebook){
        return DESTROYING_ERROR;
    }

    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        struct NotebookElement* nextElement = curElement->next;
        free(curElement);
        curElement = nextElement;
    }
    free(notebook);

    return SUCCESS;
}
Response AddRecord(struct Notebook* notebook, struct Record record){
    if (!notebook){
        return ADDING_ERROR;
    }

    struct NotebookElement* newElement = createNotebookElement(record);

    if (!newElement){
        return NO_MEMORY;
    }

    if (!notebook->head){
        notebook->head = newElement;
    }
    else{
        notebook->tail->next = newElement;
    }
    notebook->tail = newElement;

    return SUCCESS;
}
Response PrintNotebook(const struct Notebook* notebook){
    if (!notebook){
        return PRINTING_ERROR;
    }

    if (!notebook->head){
        return EMPTY_NOTEBOOK_ERROR;
    }

    printf("\n");
    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        PrintRecord(curElement->value);
        printf("\n");
        curElement = curElement->next;
    }
    printf("\n");

    return SUCCESS;
}
Response Delete(struct Notebook* notebook, const char title[]){
    if (!notebook){
        return DELETING_ERROR;
    }

    if (!notebook->head){
        return EMPTY_NOTEBOOK_ERROR;
    }

    if (CompareStrings(title, notebook->head->value.title, MAX_RECORD_TITLE_LENGTH)){
        struct NotebookElement* newHead = notebook->head->next;
        free(notebook->head);
        notebook->head = newHead;
        return SUCCESS;
    }

    struct NotebookElement* curElement = notebook->head;
    while (curElement->next){
        if (CompareStrings(title, curElement->next->value.title, MAX_RECORD_TITLE_LENGTH)){
            struct NotebookElement* newNext = curElement->next->next;
            free(curElement->next);
            curElement->next = newNext;
            return SUCCESS;
        }

        curElement = curElement->next;
    }
    return NO_RECORDS_FOUND_ERROR;
}
Response LoadNotebook(struct Notebook* notebook, const char filePath[]){
    if (!notebook){
        return LOADING_ERROR;
    }

    FILE* notebookFile = fopen(filePath, "r");
    if (!notebookFile){
        return WRONG_PATH_ERROR;
    }

    struct Record curRecord;
    while (LoadRecord(notebookFile, &curRecord)){
        if (IsRecordEmpty(curRecord)){
            continue;
        }

        AddRecord(notebook, curRecord);
    }
    return SUCCESS;
}
Response SaveNotebook(struct Notebook* notebook, const char filePath[]){
    if (!notebook){
        return SAVING_ERROR;
    }

    if (!notebook->head){
        return EMPTY_NOTEBOOK_ERROR;
    }

    FILE* notebookFile = fopen(filePath, "w+");
    if (!notebookFile){
        return WRONG_PATH_ERROR;
    }

    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        SaveRecordToFile(notebookFile, curElement->value);
        fprintf(notebookFile, "\n");
        curElement = curElement->next;
    }

    fclose(notebookFile);

    return SUCCESS;
}
Response FindByTitle(struct Notebook* notebook, const char title[]){
    if (!notebook){
        return FINDING_ERROR;
    }

    bool noResultsFound = true;

    struct NotebookElement* curElement = notebook->head;
    while (curElement){
        if (CompareStrings(title, curElement->value.title, MAX_RECORD_TITLE_LENGTH)){
            PrintRecord(curElement->value);
            noResultsFound = false;
        }

        curElement = curElement->next;
    }

    if (noResultsFound){
        return NO_RECORDS_FOUND_ERROR;
    }
    return SUCCESS;
}
Response FindByNumericField(struct Notebook* notebook, const char fieldTitle[],
                            const double min, const double max){
    if (!notebook){
        return FINDING_ERROR;
    }

    bool isFieldPlaceIn2020 = CompareStrings(fieldTitle, "placeIn2020", 32);
    bool isFieldPlaceIn2021 = CompareStrings(fieldTitle, "placeIn2021", 32);
    bool isFieldRating = CompareStrings(fieldTitle, "rating", 32);
    bool isFieldChange = CompareStrings(fieldTitle, "change", 32);

    if (isFieldPlaceIn2020){
        return findByPlaceIn2020(notebook, min, max);
    }
    if (isFieldPlaceIn2021){
        return findByPlaceIn2021(notebook, min, max);
    }
    if (isFieldRating){
        return findByRating(notebook, (unsigned int) min, (unsigned int) max);
    }
    if (isFieldChange){
        return findByChange(notebook, (unsigned int) min,(unsigned int) max);
    }

    return WRONG_FIELD_TITLE_ERROR;
}
