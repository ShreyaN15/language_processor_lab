#include <stdio.h>
#include <string.h>

char inp[100]; // Input string
int i = 0;     // Index to track character in input

// Function declarations
int q1();
int q2();
int q3();
int q4();

int main() {
    printf("Enter binary input string: ");
    scanf("%s", inp);

    if (q1()) {
        printf("String accepted.\n");
    } else {
        printf("String not accepted.\n");
    }

    return 0;
}

// q1 is start state
int q1() {
    if (inp[i] == '\0') return 0; // End of input → not final state
    printf("State q1, input: %c --> ", inp[i]);
    if (inp[i] == '0') {
        printf("moving to q1\n");
        i++;
        return q1();
    } else if (inp[i] == '1') {
        printf("moving to q2\n");
        i++;
        return q2();
    } else {
        return 0; // invalid character
    }
}

int q2() {
    if (inp[i] == '\0') return 0;
    printf("State q2, input: %c --> ", inp[i]);
    if (inp[i] == '0') {
        printf("moving to q4\n");
        i++;
        return q4();
    } else if (inp[i] == '1') {
        printf("moving to q3\n");
        i++;
        return q3();
    } else {
        return 0;
    }
}

int q3() {
    if (inp[i] == '\0') return 1;
    printf("State q3, input: %c --> ", inp[i]);
    if (inp[i] == '0') {
        printf("moving to q3\n");
        i++;
        return q3();
    } else if (inp[i] == '1') {
        printf("moving to q4\n");
        i++;
        return q4();
    } else {
        return 0;
    }
}

int q4() {
    if (inp[i] == '\0') return 1; // Final state!
    printf("State q4, input: %c --> ", inp[i]);
    if (inp[i] == '0') {
        printf("moving to q3\n");
        i++;
        return q3();
    } else if (inp[i] == '1') {
        printf("moving to q1\n");
        i++;
        return q1();
    } else {
        return 0;
    }
}
