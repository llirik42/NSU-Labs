#include "record.h"
#include "command.h"
#include <stdio.h>
#include "utils.h"
#include <stdlib.h>

#define executeCommand(response) {Response tmp = response; if(tmp != SUCCESS) { PrintWithPrefix(tmp); return;}}

#define MAX_COMMAND_DESCRIPTION_LENGTH 128
#define MAX_FILE_PATH_LENGTH 64

void printWrongInput(){
    PrintWithPrefix("Wrong input");
}

void helpCommand(struct Notebook* notebook);
void printAllCommand(struct Notebook* notebook);
void exitCommand(struct Notebook* notebook);
void addCommand(struct Notebook* notebook);
void clearCommand(struct Notebook* notebook);
void deleteCommand(struct Notebook* notebook);
void saveCommand(struct Notebook* notebook);
void loadCommand(struct Notebook* notebook);
void findCommand(struct Notebook* notebook);

struct Command{
    char title[MAX_COMMAND_TITLE_LENGTH];
    char description[MAX_COMMAND_DESCRIPTION_LENGTH];
    void (*execute)(struct Notebook*);
};

static const struct Command HELP_COMMAND = {
        .title = "help",
        .description = "shows commands list",
        .execute = &helpCommand
};
static const struct Command PRINT_ALL_COMMAND = {
        .title = "printall",
        .description = "prints all records",
        .execute = &printAllCommand
};
static const struct Command EXIT_COMMAND = {
        .title = "exit",
        .description = "exits the program",
        .execute = &exitCommand
};
static const struct Command ADD_COMMAND = {
        .title = "add",
        .description = "adds record to the notebook (you have to input fields sequentially)",
        .execute = &addCommand
};
static const struct Command CLEAR_COMMAND = {
    .title = "clr",
    .description = "clears command line",
    .execute = &clearCommand
};
static const struct Command DELETE_COMMAND = {
        .title = "delete",
        .description = "deletes records by title",
        .execute = &deleteCommand
};
static const struct Command SAVE_COMMAND = {
        .title = "save",
        .description = "saves notebook to file",
        .execute = &saveCommand
};
static const struct Command LOAD_COMMAND = {
        .title = "load",
        .description = "loads notebook from file",
        .execute = &loadCommand
};
static const struct Command FIND_COMMAND = {
        .title = "find",
        .description = "finds records with given field's values",
        .execute = &findCommand
};

static const struct Command* COMMANDS_LIST[] = {
        &HELP_COMMAND,
        &PRINT_ALL_COMMAND,
        &EXIT_COMMAND,
        &ADD_COMMAND,
        &CLEAR_COMMAND,
        &DELETE_COMMAND,
        &SAVE_COMMAND,
        &LOAD_COMMAND,
        &FIND_COMMAND
};
const unsigned int COMMANDS_COUNTER = sizeof(COMMANDS_LIST) / sizeof(COMMANDS_LIST[0]);

void helpCommand(struct Notebook* notebook){
    PrintWithPrefix("Commands list:");
    for (unsigned int i = 0; i < COMMANDS_COUNTER; i++){
        const struct Command curCommand = (*COMMANDS_LIST[i]);

        printf("%s    %s - %s\n", OUT_PREFIX, curCommand.title, curCommand.description);
    }
    printf("\n");
}
void printAllCommand(struct Notebook* notebook){
    executeCommand(PrintNotebook(notebook))
}
void exitCommand(struct Notebook* notebook){
    PrintWithPrefix("Exiting ...");
    DestroyNotebook(notebook);
    exit(0);
}
void addCommand(struct Notebook* notebook){
    struct Record newRecord = InputRecord();

    if (IsRecordEmpty(newRecord)){
        printWrongInput();
        return;
    }

    executeCommand(AddRecord(notebook, newRecord))

    PrintWithPrefix("Record was successfully added");
}
void clearCommand(struct Notebook* notebook){
    system("cls");
}
void deleteCommand(struct Notebook* notebook){
    char title[MAX_RECORD_TITLE_LENGTH];

    if (!InputString(title, "Title of record")){
        printWrongInput();
        return;
    }

    executeCommand(Delete(notebook, title))

    PrintWithPrefix("Record was successfully deleted");
}
void saveCommand(struct Notebook* notebook){
    char filePath[MAX_FILE_PATH_LENGTH];

    if (!InputString(filePath, "File path")){
        printWrongInput();
        return;
    }

    executeCommand(SaveNotebook(notebook, filePath))

    PrintWithPrefix("Notebook was successfully saved");
}
void loadCommand(struct Notebook* notebook){
    char filePath[MAX_FILE_PATH_LENGTH];

    if (!InputString(filePath, "File path")){
        printWrongInput();
        return;
    }

    executeCommand(LoadNotebook(notebook, filePath))

    PrintWithPrefix("Notebook was successfully loaded");
}
void findCommand(struct Notebook* notebook){
    char fieldTitle[MAX_FILE_PATH_LENGTH];

    if (!InputString(fieldTitle, "Field")){
        printWrongInput();
        return;
    }

    const bool isFieldTitle = CompareStrings(fieldTitle, "title", MAX_COMMAND_TITLE_LENGTH);
    const bool isFieldPlaceIn2020 = CompareStrings(fieldTitle, "placeIn2020", 32);
    const bool isFieldPlaceIn2021 = CompareStrings(fieldTitle, "placeIn2021", 32);
    const bool isFieldRating = CompareStrings(fieldTitle, "rating", 32);
    const bool isFieldChange = CompareStrings(fieldTitle, "change", 32);

    const bool isFieldCorrect = isFieldTitle || isFieldPlaceIn2020 || isFieldPlaceIn2021 ||
                                isFieldRating || isFieldChange;

    if (!isFieldCorrect){
        printWrongInput();
        return;
    }

    if (isFieldTitle){
        char title[MAX_RECORD_TITLE_LENGTH];
        if (!InputString(title, "Title")){
            printWrongInput();
            return;
        }
        executeCommand(FindByTitle(notebook, title))
        printf("\n");
        return;
    }

    double minValue;
    double maxValue;

    if (!InputDouble(&minValue, "Min value")){
        printWrongInput();
        return;
    }
    if (!InputDouble(&maxValue, "Max value")){
        printWrongInput();
        return;
    }

    executeCommand(FindByNumericField(notebook, fieldTitle, minValue, maxValue))
}

void Execute(const char title[], struct Notebook* notebook){
    for (unsigned int i = 0; i < COMMANDS_COUNTER; i++){
        const struct Command* curCommand = COMMANDS_LIST[i];
        const char* curCommandTitle = curCommand->title;

        if (CompareStrings(title, curCommandTitle, MAX_COMMAND_TITLE_LENGTH)){
            curCommand->execute(notebook);
            return;
        }
    }

    PrintWithPrefix("Wrong command");
}
