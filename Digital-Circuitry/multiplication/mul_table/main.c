#include <stdio.h>

void printBin(FILE* file, unsigned char number){
    unsigned char binaryNotation[8];

    for (unsigned char i = 0; i < 8; i++){
        binaryNotation[i] = number % 2;
        number /= 2;
    }

    for (unsigned int i = 8; i > 0; i--){
        fprintf(file, "%u", binaryNotation[i - 1]);
    }

    fprintf(file, "\n");
}

int main (){
    const char* PATH = "C:\\VivadoProjects\\mul_table\\mul_table.srcs\\sources_1\\new\\table.mem";

    FILE* table = fopen(PATH, "w");

    for (unsigned char a = 0; a < 16; a++){
        for (unsigned char b = 0; b < 16; b++){
            printBin(table, a * b);
        }
    }

    fclose(table);
}
