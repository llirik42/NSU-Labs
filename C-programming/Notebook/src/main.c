#include "command.h"
#include "utils.h"

int main() {
    struct Notebook* notebook = NULL;

    Response creatingResponse = CreateNotebook(&notebook);
    if (creatingResponse != SUCCESS){
        PrintWithPrefix(creatingResponse);
        return 0;
    }

    char command[MAX_COMMAND_TITLE_LENGTH];
    Execute("help", notebook);
    while (1){
        InputCommand(command);

        Execute(command, notebook);
    }
}
