#include <stdio.h>
#include <string.h>

char input[100], stack[100];
int top = -1, i = 0;

void push(char c) {
    stack[++top] = c;
    stack[top + 1] = '\0';
}

void pop(int n) {
    top -= n;
    stack[top + 1] = '\0';
}

int check() {
    // Rule: S → id
    if (top >= 1 && stack[top] == 'd' && stack[top - 1] == 'i') {
        pop(2);
        push('S');
        printf("$%s\t%s$\tReduced: S -> id\n", stack, input + i);
        return 1;
    }

    // Rule: S → (S)
    if (top >= 2 && stack[top] == ')' && stack[top - 2] == '(' && stack[top - 1] == 'S') {
        pop(3);
        push('S');
        printf("$%s\t%s$\tReduced: S -> (S)\n", stack, input + i);
        return 1;
    }

    // Rule: S → S+S
    if (top >= 2 && stack[top] == 'S' && stack[top - 1] == '+' && stack[top - 2] == 'S') {
        pop(3);
        push('S');
        printf("$%s\t%s$\tReduced: S -> S+S\n", stack, input + i);
        return 1;
    }

    // Rule: S → S-S
    if (top >= 2 && stack[top] == 'S' && stack[top - 1] == '-' && stack[top - 2] == 'S') {
        pop(3);
        push('S');
        printf("$%s\t%s$\tReduced: S -> S-S\n", stack, input + i);
        return 1;
    }

    return 0;
}

int main() {
    printf("Enter input string: ");
    scanf("%s", input);

    printf("\nStack\tInput\tAction\n");

    while (i < strlen(input)) {
        push(input[i]);
        i++;
        printf("$%s\t%s$\tShift\n", stack, input + i);

        while (check()); // keep reducing as long as possible
    }

    if (strcmp(stack, "S") == 0 && i == strlen(input))
        printf("\nString Accepted!\n");
    else
        printf("\nString Rejected!\n");

    return 0;
}
