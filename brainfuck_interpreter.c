#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_DIMENSION 30000 
char array[ARR_DIMENSION];
int cursor = 0;
int start_loop = -1;
int end_loop   = -1;

// test function
void test(char arr[], int arr_size){
    int i;
    for (i = 0; i < arr_size; i++){
        printf("%c", arr[i]);
    }
    printf("\n");
    sleep(1);
}

// * WORKS FINE
// determinate how long the file is
int numberOfCommand(char file_name[]) {
    FILE *f;
    char c;
    int number_of_command = 0;

    f = fopen(file_name, "r");
    c = fgetc(f);
    while (c != EOF) {
        number_of_command++;
        c = fgetc(f);
    }

    fclose(f);
    return number_of_command;
}

// * WORKS FINE
// make a copy of the code
void copyCodeFromFile(char file_name[], char arr[], int arr_size) {
    FILE *f;
    int i;
    char c;

    f = fopen(file_name, "r");
    for (i = 0; i < arr_size; i++) {
        c = fgetc(f);
        arr[i] = c;
    }
    fclose(f);
}

// move array's cursor
void moveCursor(char command) {
    if (command == '>') { 
        cursor++;
    } 
    if (command == '<') {
        cursor--;
    }
    
    if (cursor > 29999) {
        cursor = 0;
    }
    if (cursor < 0) {
        cursor = 29999;
    }
}

// copy loop code
char* loop(char code[], int start, int end) {
    int i;
    int loop_lenght = end - start - 1;
    char* temp_code = malloc(loop_lenght);
    
    for (i = 0; i < loop_lenght; i++) {
        temp_code[i] = code[i + start + 1];
    }

    return temp_code;
}

// * BrainFuck commands
// *
// * > -> move forward
// * < -> move backward
// * + -> add 1
// * - -> sub 1
// * . -> output
// * , -> input
// * [ -> open loop
// * ] -> close loop
void interpreter(char code[], int dimension) {
    int i, j;                                                      
    int bracket_counter = 0;                                                      
    for (i = 0; i < dimension; i++) {                                              
        if (code[i] == '>' || code[i] == '<') { // * move cursor
            moveCursor(code[i]);
        } else if (code[i] == '+') {            // * increment value
            array[cursor]++;
        } else if (code[i] == '-') {            // * decrese value
            array[cursor]--;
        } else if (code[i] == '.') {            // * output
            printf("%c", array[cursor]);
        } else if (code[i] == ',') {            // * input
            scanf(" %c", &array[cursor]);
        // * start loop code
        } else if (code[i] == '[') {            
            start_loop = i;
            for (j = start_loop + 1; j < dimension; j++) {
                if (code[j] == '[') {
                    bracket_counter++;
                } else if (code[j] == ']') {
                    if (bracket_counter != 0) {
                        bracket_counter--;
                    } else {
                            end_loop = j;
                            break;
                        }
                }
            }
            i = end_loop + 1;
            while(array[cursor] != 0) {
                interpreter(loop(code, start_loop, end_loop), end_loop - start_loop + 1);
            }
        }
        // * end loop code
    }
}

int main(int argc, char *file_path[]) {
    int code_dimension = numberOfCommand(file_path[1]) * sizeof(char); // dimension of array where code is stored
    char* code  = malloc(code_dimension);

    copyCodeFromFile(file_path[1], code, code_dimension);
    // ? test(code, code_dimension);
    interpreter(code, code_dimension);
    free(code);
    return 0;
}
