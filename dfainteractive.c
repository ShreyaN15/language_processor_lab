//this code is for the case in which q4 is the only final state

#include <stdio.h>
#include <string.h>
#include <conio.h>

int main() {
    char ch;
    int state = 1;  // Start at q1
    char input[100];
    int index = 0;

    //clrscr();
    printf("DFA Interactive Mode\n");
    printf("---------------------\n");
    printf("Enter '0' or '1' one by one. Type 'd' when done.\n\n");

    while (1) {
        ch = getch();

        if (ch == 'd') {
            break;
        }

        if (ch != '0' && ch != '1') {
            printf(" Invalid input: %c (only 0 and 1 allowed)\n", ch);
            continue;
        }

        input[index++] = ch;

        // Show current state and transition
        switch (state) {
            case 1:
                if (ch == '0') {
                    printf("q1 --0--> q1\n");
                    state = 1;
                } else {
                    printf("q1 --1--> q2\n");
                    state = 2;
                }
                break;

            case 2:
                if (ch == '0') {
                    printf("q2 --0--> q4\n");
                    state = 4;
                } else {
                    printf("q2 --1--> q3\n");
                    state = 3;
                }
                break;

            case 3:
                if (ch == '0') {
                    printf("q3 --0--> q3\n");
                    state = 3;
                } else {
                    printf("q3 --1--> q4\n");
                    state = 4;
                }
                break;

            case 4:
                if (ch == '0') {
                    printf("q4 --0--> q3\n");
                    state = 3;
                } else {
                    printf("q4 --1--> q1\n");
                    state = 1;
                }
                break;
        }
    }

    input[index] = '\0';

    printf("\n Input string: %s\n", input);
    if (state == 4) {
        printf("String accepted! Final state: q4\n");
    } else {
        printf("String rejected. Final state: q%d\n", state);
    }

    getch();
    return 0;
}
