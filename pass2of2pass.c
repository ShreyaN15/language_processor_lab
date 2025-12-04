#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char label[20], opcode[20], operand[20], line[100];
int locctr = 0, start_addr = 0, program_length = 0;
char program_name[20];

FILE *intermediate, *symtab, *optab, *objectcode;

char* getSymbolAddress(char *sym) {
    static char addr[10];
    symtab = fopen("symtab.txt", "r");
    char symbol[20], address[10];

    while (fscanf(symtab, "%s %s", symbol, address) != EOF) {
        if (strcmp(symbol, sym) == 0) {
            strcpy(addr, address);
            fclose(symtab);
            return addr;
        }
    }

    fclose(symtab);
    return NULL;
}

char* getOpcodeHex(char *op) {
    static char hex[10];
    optab = fopen("optab.txt", "r");
    char mnemonic[20], code[10];

    while (fscanf(optab, "%s %s", mnemonic, code) != EOF) {
        if (strcmp(mnemonic, op) == 0) {
            strcpy(hex, code);
            fclose(optab);
            return hex;
        }
    }

    fclose(optab);
    return NULL;
}

// Parses lines with or without label
void parseLine(char *line) {
    label[0] = opcode[0] = operand[0] = '\0';
    int fields = sscanf(line, "%x %s %s %s", &locctr, label, opcode, operand);

    // Handle lines without label
    if (fields == 3) {
        // Shift fields: label is empty
        strcpy(operand, opcode);
        strcpy(opcode, label);
        label[0] = '\0';
    }
}

int main() {
    intermediate = fopen("intermediate.txt", "r");
    objectcode = fopen("output.obj", "w");

    if (!intermediate || !objectcode) {
        printf("Error opening files.\n");
        return 1;
    }

    // Read first line (START)
    fgets(line, sizeof(line), intermediate);
    parseLine(line);

    strcpy(program_name, label);
    int starting_address = locctr;

    // Get program length from last line
//     fseek(intermediate, 0, SEEK_END);
// long pos = ftell(intermediate);
rewind(intermediate);
char last_line[100] = "";

// while (pos-- > 0) {
//     fseek(intermediate, pos, SEEK_SET);
//     if (fgetc(intermediate) == '\n') {
//         fgets(last_line, sizeof(last_line), intermediate);
//         if (strstr(last_line, "Program Length") != NULL) {
//             break;
//         }
//     }
// }
while (fgets(last_line, sizeof(last_line), intermediate)) {
    if (strstr(last_line, "Program Length") != NULL) {
        sscanf(last_line, "Program Length: %X", &program_length);
        break;
    }
}
// sscanf(last_line, "Program Length: %X", &program_length);

    // Header record
    fprintf(objectcode, "H^%-6s^%06X^%06X\n", program_name, starting_address, program_length);

    // Go back to start after START
    rewind(intermediate);
    fgets(line, sizeof(line), intermediate);  // Skip START line

    char text_record[100] = "", objcode[30], temp[40];
    int text_start = 0, text_length = 0;
    int new_record = 1;

    while (fgets(line, sizeof(line), intermediate)) {
        if (strstr(line, "Program Length") != NULL) break;

        parseLine(line);

        if (strcmp(opcode, "END") == 0 || opcode[0] == '\0') continue;

        objcode[0] = '\0';

        char *ophex = getOpcodeHex(opcode);
        if (ophex) {
            // Standard instruction
            char *symaddr = getSymbolAddress(operand);
            if (symaddr) {
                sprintf(objcode, "%s%s", ophex, symaddr);
            } else {
              //  sprintf(objcode, "%s0000", ophex); // Unknown symbol
                  printf("Error: Undefined symbol '%s' at location %X\n", operand, locctr);
    exit(1);
            }
        } else if (strcmp(opcode, "WORD") == 0) {
            sprintf(objcode, "%06X", atoi(operand));
        } else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C') {
                char *start = strchr(operand, '\'') + 1;
                char *end = strrchr(operand, '\'');
                for (char *p = start; p < end; p++) {
                    char temp_byte[3];
                    sprintf(temp_byte, "%02X", *p);
                    strcat(objcode, temp_byte);
                }
            } else if (operand[0] == 'X') {
                // char *start = strchr(operand, '\'') + 1;
                // strncpy(objcode, start, 2);
                // objcode[2] = '\0';

                char *start = strchr(operand, '\'') + 1;
                char *end = strrchr(operand, '\'');
                int len = end - start;
                strncpy(objcode, start, len);
                objcode[len] = '\0';

            }
        } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            // Flush current text record
            // if (text_length > 0) {
            //     fprintf(objectcode, "T^%06X^%02X%s\n", text_start, text_length, text_record);
            //     text_record[0] = '\0';
            //     text_length = 0;
            //     new_record = 1;
            // }
            continue;
        }
        else {
    // Invalid mnemonic
    printf("Error: Invalid opcode '%s' at location %X\n", opcode, locctr);
    exit(1); // or set a flag to continue assembling
}

        // If object code was generated
        // If object code was generated
if (strlen(objcode) > 0) {
    // If starting a new text record, set text_start
    if (new_record) {
        text_start = locctr;
        text_record[0] = '\0';
        text_length = 0;  // Reset length for new record
        new_record = 0;
    }

    int objcode_bytes = strlen(objcode) / 2;

    // If adding objcode exceeds 30 bytes, flush current record first
    if (text_length + (strlen(objcode)/2) > 30) {
        fprintf(objectcode, "T^%06X^%02X%s\n", text_start, text_length, text_record);
        // Start a new record with current objcode
        text_start = locctr;
        text_record[0] = '\0';
        text_length = 0;
    }
sprintf(temp, "^%s", objcode);
   // snprintf(temp, sizeof(temp), "^%s", objcode);
    strcat(text_record, temp);
    text_length += objcode_bytes;
}

    }

    // Write any remaining text record
    if (text_length > 0) {
        fprintf(objectcode, "T^%06X^%02X%s\n", text_start, text_length, text_record);
    }

    // End record
    fprintf(objectcode, "E^%06X\n", starting_address);

    fclose(intermediate);
    fclose(objectcode);

    printf("PASS 2 completed successfully. Output written to output.obj\n");
    return 0;
}



