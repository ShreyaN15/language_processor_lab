#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char input[100][10];        // tokens (id, +, -, (, ), $ or invalid char)
int isValidToken[100];      // 1 if valid, 0 if invalid
int tokenCount = 0;
char stack[100][10];
int top = -1;
int i = 0;

// ---------- Helper for aligned printing ----------
void printStep(const char *action) {
    char stackStr[200] = "";
    char inputStr[200] = "";

    // Print all stack contents including $
    for (int j = 0; j <= top; j++) {
        strcat(stackStr, stack[j]);
    }
    // Print remaining input tokens + terminating $
    for (int j = i; j < tokenCount; j++) {
        strcat(inputStr, input[j]);
    }
    strcat(inputStr, "$");

    printf("%-20s %-20s %-20s\n", stackStr, inputStr, action);
}

void push(char *c) {
    strcpy(stack[++top], c);
}

void pop(int n) {
    top -= n;
}

int check() {
    // Rule: S → id
    if (top >= 0 && strcmp(stack[top], "id") == 0) {
        pop(1);
        push("S");
        printStep("Reduced: S -> id");
        return 1;
    }

    // Rule: S → (S)
    if (top >= 2 && strcmp(stack[top], ")") == 0 &&
        strcmp(stack[top - 2], "(") == 0 &&
        strcmp(stack[top - 1], "S") == 0) {
        pop(3);
        push("S");
        printStep("Reduced: S -> (S)");
        return 1;
    }

    // Rule: S → S+S
    if (top >= 2 && strcmp(stack[top], "S") == 0 &&
        strcmp(stack[top - 1], "+") == 0 &&
        strcmp(stack[top - 2], "S") == 0) {
        pop(3);
        push("S");
        printStep("Reduced: S -> S+S");
        return 1;
    }

    // Rule: S → S-S
    if (top >= 2 && strcmp(stack[top], "S") == 0 &&
        strcmp(stack[top - 1], "-") == 0 &&
        strcmp(stack[top - 2], "S") == 0) {
        pop(3);
        push("S");
        printStep("Reduced: S -> S-S");
        return 1;
    }

    return 0;
}

// Tokenizer: split input into id, +, -, (, ), or invalid
void tokenize(char *expr) {
    int len = strlen(expr);
    for (int k = 0; k < len;) {
        if (isspace(expr[k])) {
            k++;
        } else if (expr[k] == '+' || expr[k] == '-' || expr[k] == '(' || expr[k] == ')') {
            input[tokenCount][0] = expr[k];
            input[tokenCount][1] = '\0';
            isValidToken[tokenCount++] = 1;
            k++;
        } else if (expr[k] == 'i' && expr[k + 1] == 'd') {
            strcpy(input[tokenCount], "id");
            isValidToken[tokenCount++] = 1;
            k += 2;
        } else {
            // Store the invalid character itself
            input[tokenCount][0] = expr[k];
            input[tokenCount][1] = '\0';
            isValidToken[tokenCount++] = 0;
            k++;
        }
    }
}

int main() {
    char expr[100];
    printf("Enter input string : ");
    fgets(expr, sizeof(expr), stdin);

    // Remove newline if present
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }
    if (expr[0] == '\0') {
        printf("error: empty string\n");
        exit(0);
    }

    tokenize(expr);

    // Push $ to bottom of stack first
    push("$");

    printf("\n%-20s %-20s %-20s\n", "Stack", "Input", "Action");
    printf("------------------------------------------------------------\n");

    while (i < tokenCount) {
        // If invalid token, stop and show error
        if (!isValidToken[i]) {
            printStep("error");
            printf("\nerror: Unexpected token '%s' at position %d\n", input[i], i);
            return 1;
        }

        push(input[i]);
        i++;
        printStep("Shift");

        while (check()); // keep reducing after each shift
    }

    // Final reductions after input is consumed
    while (check());

    // Acceptance condition: stack should be "$" at bottom and "S" on top
    if (top == 1 && strcmp(stack[0], "$") == 0 && strcmp(stack[1], "S") == 0) {
        pop(1);  // pop S to show empty stack
        printStep("Accepted");
        printf("\nString Accepted!\n");
    } else {
        printStep("Rejected");
        printf("\nString Rejected!\n");
    }

    return 0;
}