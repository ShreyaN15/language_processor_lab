#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include<conio.h>
// Check for separator (except space)
int isSeparator(char ch) {
    return (ch == ',' || ch == ';' || ch == '(' || ch == ')' ||
	    ch == '{' || ch == '}' || ch == '[' || ch == ']' ||
        ch == '"' || ch == '\'');
}

// Check for operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' ||
	    ch == '<' || ch == '=' || ch == '!');
}

// Check for keyword
int isKeyword(char* str) {
    const char* keywords[] = {
	"if", "else", "while", "do", "for", "int",
	"float", "char", "return", "void", "long", "short", "include", "printf"
    };
    int i;
    int numKeywords = 14;
    for (i = 0; i < numKeywords; i++) {
	if (strcmp(str, keywords[i]) == 0) {
	    return 1;
	}
    }
    return 0;
}

// Check for integer
int isInteger(char* str) {
    int i = 0;
    if (str == NULL || *str == '\0') {
	return 0;
    }
    if (str[0] == '-') {
	i = 1;
	if (str[i] == '\0') return 0;
    }
    for (; str[i] != '\0'; i++) {
	if (!isdigit(str[i])) {
	    return 0;
	}
    }
    return 1;
}

// Check for identifier
int isIdentifier(char* str) {
    int i;
    if (str == NULL || *str == '\0' || isKeyword(str)) {
	return 0;
    }
    if (!isalpha(str[0]) && str[0] != '_') {
	return 0;
    }
    for (i = 1; str[i] != '\0'; i++) {
	if (!isalnum(str[i]) && str[i] != '_') {
	    return 0;
	}
    }
    return 1;
}

// Lexical Analyzer
void analyze(char* input) {
    int left = 0, right;
    int len = strlen(input);
    char* lexeme;

    printf("--- Lexical Analysis Results ---\n");
    printf("%-25s | %s\n", "Lexeme", "Token Type");
    printf("----------------------------------------\n");

    while (left < len) {
	if (isspace(input[left])) {
	    left++;
	    continue;
	}

	if (input[left] == '#') {
	    right = left;
	    while (right < len && input[right] != '\n') {
		right++;
	    }
	    lexeme = (char*)malloc(right - left + 1);
	    strncpy(lexeme, input + left, right - left);
	    lexeme[right - left] = '\0';
	    printf("%-25s | PREPROCESSOR_DIRECTIVE\n", lexeme);
	    free(lexeme);
	    left = right;
	    continue;
	}

	if (input[left] == '"' || input[left] == '\'') {
            // First handle the opening quote as a separator
            printf("%-25c | SEPARATOR\n", input[left]);
            
            // Move past the quote
            left++;
            right = left;
            
            // Find the end of the string
            char quote = input[left - 1]; // Remember which quote type we're looking for
            while (right < len && input[right] != quote) {
                right++;
            }
            
            // Extract and print the string content
            if (right > left) {
                lexeme = (char*)malloc(right - left + 1);
                strncpy(lexeme, input + left, right - left);
                lexeme[right - left] = '\0';
                printf("%-25s | STRING_LITERAL\n", lexeme);
                free(lexeme);
            }
            
            // Handle the closing quote if present
            if (right < len && input[right] == quote) {
                printf("%-25c | SEPARATOR\n", input[right]);
                right++;
            }
            
            left = right;
            continue;
	}

	if (isOperator(input[left])) {
	    printf("%-25c | OPERATOR\n", input[left]);
	    left++;
	    continue;
	}

	if (isSeparator(input[left])) {
	    printf("%-25c | SEPARATOR\n", input[left]);
	    left++;
	    continue;
	}

	right = left;
	while (right < len && !isspace(input[right]) &&
	       !isOperator(input[right]) && !isSeparator(input[right])) {
	    right++;
	}

	lexeme = (char*)malloc(right - left + 1);
	strncpy(lexeme, input + left, right - left);
	lexeme[right - left] = '\0';

	if (isKeyword(lexeme)) {
	    printf("%-25s | KEYWORD\n", lexeme);
	} else if (isInteger(lexeme)) {
	    printf("%-25s | INTEGER\n", lexeme);
	} else if (isIdentifier(lexeme)) {
	    printf("%-25s | IDENTIFIER\n", lexeme);
	} else {
	    printf("%-25s | UNKNOWN\n", lexeme);
	}

	free(lexeme);
	left = right;
    }
}

int main() {
    FILE *file;
    char *inputBuffer = NULL;
    long fileSize;
    const char* filename = "example.c";
    size_t readBytes;

    file = fopen(filename, "r");

    if (file == NULL) {
	printf("File '%s' not found. Creating a default file.\n", filename);
	file = fopen(filename, "w");
	if (file == NULL) {
	    perror("Error creating file");
	    return 1;
	}

	fprintf(file, "#include<stdio.h>\n\nint main(){\n    int a1, b1, s1;\n    a1 = 1;\n    b1 = 2;\n    s1 = a1 + b1;\n    printf(\"The value of s1 is:\",s1);\n}");
	fclose(file);
	file = fopen(filename, "r");
	if (file == NULL) {
	    perror("Error opening file after creation");
	    return 1;
	}
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    inputBuffer = (char*)malloc(fileSize + 1);
    if (inputBuffer == NULL) {
	printf("Could not allocate memory for file content.\n");
	fclose(file);
	return 1;
    }

    readBytes = fread(inputBuffer, 1, fileSize, file);
    inputBuffer[readBytes] = '\0';
    fclose(file);

    printf("Analyzing content from file: \"%s\"\n\n", filename);
    analyze(inputBuffer);
    free(inputBuffer);

    getch();
    return 0;
}



