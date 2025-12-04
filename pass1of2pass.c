#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global variables
char label[20], opcode[20], operand[20];
int locctr, start, program_length;
char line[100];

// Function to read a line and parse into label, opcode, and operand
int readLine(FILE *file) {
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    // Remove newline
    line[strcspn(line, "\n")] = 0;
    
    // Clear previous values 
    label[0] = opcode[0] = operand[0] = '\0';
    
    char *token = strtok(line, " \t");
    if (token == NULL) return 0;
    
    // Check if line starts with a label
    if (!isspace(line[0])) {
        strcpy(label, token);
        token = strtok(NULL, " \t");
        if (token == NULL) return 1;
    }
    
    // Get opcode
    if (label[0] == '\0') {
        strcpy(opcode, token);
    } else {
        strcpy(opcode, token);
    }
    
    // Get operand if present
    token = strtok(NULL, " \t");
    if (token != NULL) {
        strcpy(operand, token);
    }
    
    return 1;
}

int searchOptab(char *code) {
    FILE *optab = fopen("optab.txt", "r");
    if (optab == NULL) {
        printf("Error: Cannot open optab.txt\n");
        return 0;
    }
    char op[20], val[10];
    int found = 0;
    while (fscanf(optab, "%s %s", op, val) != EOF) {
        if (strcmp(op, code) == 0) {
            found = 1;
            break;
        }
    }
    fclose(optab);
    return found;
}

void writeSymtab(char *label, int address) {
    if (label[0] == '\0') return;  // Skip if no label
    FILE *symtab = fopen("symtab.txt", "a");
    fprintf(symtab, "%s\t%04X\n", label, address);
    fclose(symtab);
}
int isDuplicate(char *label) {
    FILE *symtab = fopen("symtab.txt", "r");
    if (!symtab) return 0;

    char existing_label[20];
    int address;
    while (fscanf(symtab, "%s %X", existing_label, &address) != EOF) {
        if (strcmp(existing_label, label) == 0) {
            fclose(symtab);
            return 1;  // Duplicate found
        }
    }
    fclose(symtab);
    return 0;  // No duplicate
}
int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *inter = fopen("intermediate.txt", "w");
    
    if (input == NULL || inter == NULL) {
        printf("Error opening files\n");
        return 1;
    }

    // Clear symtab.txt
    FILE *symtab = fopen("symtab.txt", "w");
    fclose(symtab);

    // Process first line (START)
    readLine(input);
    if (strcmp(opcode, "START") == 0) {
        start = strtol(operand, NULL, 16);
        locctr = start;
        fprintf(inter, "%04X\t%-8s\t%-6s\t%s\n", locctr, label, opcode, operand);
    } else {
        locctr = 0;
        rewind(input);
    }

    // Process rest of the program
    while (readLine(input)) {
        if (line[0] == '\0' || line[0] == '.') continue;  // Skip empty lines and comments
        
        // Write to intermediate file
        fprintf(inter, "%04X\t%-8s\t%-6s\t%s\n", locctr, label, opcode, operand);

        // Add label to symbol table
        if (label[0] != '\0') {
            if (isDuplicate(label)) {
            printf("Error: Duplicate label '%s' at location %04X\n", label, locctr);
            exit(1);
        }
        writeSymtab(label, locctr);
           
        }

        // Update LOCCTR
        if (searchOptab(opcode)) {
            locctr += 3;
        } else if (strcmp(opcode, "WORD") == 0) {
            locctr += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            locctr += 3 * atoi(operand);
        } else if (strcmp(opcode, "RESB") == 0) {
            locctr += atoi(operand);
        } else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C') {
                // Find string between quotes
                char *start = strchr(operand, '\'');
                char *end = strrchr(operand, '\'');
                if (start && end && start != end) {
                    locctr += (end - start - 1);  // Length of character string
                }
            } else if (operand[0] == 'X') {
                // Find hex string between quotes
                char *start = strchr(operand, '\'');
                char *end = strrchr(operand, '\'');
                if (start && end && start != end) {
                    locctr += (end - start - 1) / 2;  // Length of hex string
                }
            }
        } else if (strcmp(opcode, "END") == 0) {
            program_length = locctr - start;
            fprintf(inter, "\nProgram Length: %04X\n", program_length);
            break;
        } else if (opcode[0] != '.') {  // Ignore comments
            printf("Error: Invalid opcode %s\n", opcode);
        }
    }

    fclose(input);
    fclose(inter);

    printf("PASS 1 completed successfully\n");
    printf("Program Length: %04X\n", program_length);
    return 0;

    return 0;
}
