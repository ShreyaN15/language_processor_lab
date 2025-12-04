#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Namtab
{
    char name[20];
    int start;
    int end;
    int clcnt;
};

char Deftab[1000];
struct Namtab Namtabs[20];
int NamtabCount = 0;

FILE *namtabFile, *deftabFile, *argtabFile;

int isMacro(char *word)
{
    for (int i = 0; i < NamtabCount; i++)
    {
        if (strcmp(word, Namtabs[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void stripSymbols(char *line)
{
    int size = strlen(line);
    for (int i = 0; i < size; i++)
    {
        if (line[i] == '\t' || line[i] == ',')
        {
            line[i] = ' ';
        }
    }
    if (line[size - 1] == '\n')
    {
        line[size - 1] = '\0';
    }
}

void getInstructions(char *line, char *instructions[], int *instructionCount)
{
    stripSymbols(line);
    int i = 0;

    char *token = strtok(line, " ");
    while (token != NULL)
    {
        if (strlen(token) > 0)
        {
            instructions[i] = token;
            i++;
        }
        token = strtok(NULL, " ");
    }
    *instructionCount = i;
}

void defineMacro(FILE *inputFile, char *macroDefinition[], int macroDefCount)
{
    char *macroName = macroDefinition[0];
    int argCount = macroDefCount - 2;
    Namtabs[NamtabCount].clcnt = 0;
    strcpy(Namtabs[NamtabCount].name, macroName);

    Namtabs[NamtabCount].start = strlen(Deftab);

    // Add macro definition line
    for (int i = 0; i < macroDefCount; i++)
    {
        strcat(Deftab, macroDefinition[i]);
        strcat(Deftab, " ");
    }
    strcat(Deftab, "\n");

    char macroLine[100];
    char *macroInstructions[20];
    int macroInstructionCount;

    while (fgets(macroLine, 100, inputFile))
    {
        getInstructions(macroLine, macroInstructions, &macroInstructionCount);
        if (macroInstructionCount == 0)
        {
            continue;
        }
        if (strcmp(macroInstructions[0], "MEND") == 0)
        {
            break;
        }

        strcat(Deftab, macroInstructions[0]);
        strcat(Deftab, " ");
        for (int i = 1; i < macroInstructionCount; i++)
        {
            char *word = macroInstructions[i];
            int replaced = 0;
            for (int j = 0; j < argCount; j++)
            {
                if (strcmp(word, macroDefinition[j + 2]) == 0)
                {
                    strcat(Deftab, "?");
                    char num[12];
                    sprintf(num, "%d", j);
                    strcat(Deftab, num);
                    replaced = 1;
                    break;
                }
            }
            if (!replaced)
            {
                strcat(Deftab, word);
            }
            strcat(Deftab, " ");
        }
        strcat(Deftab, "\n");
    }

    Namtabs[NamtabCount].end = strlen(Deftab) - 1;
    NamtabCount++;
}

void expandMacro(FILE *outputFile, char *macroName, char *argTab[], int macroArgCount)
{
    int macroIndex = isMacro(macroName);
    int macroStart = Namtabs[macroIndex].start;
    int macroEnd = Namtabs[macroIndex].end;
    Namtabs[macroIndex].clcnt++;

    int callCount = Namtabs[macroIndex].clcnt;

    // === Write macro call line to output ===
    fprintf(outputFile, ".%s ", macroName);
    for (int i = 0; i < macroArgCount; i++)
    {
        fprintf(outputFile, "%s", argTab[i]);
        if (i < macroArgCount - 1) fprintf(outputFile, ",");
    }
    fprintf(outputFile, "\n");

    // === Write to argtab.txt ===
    fprintf(argtabFile, "Macro Call #%d: %s\n", callCount, macroName);
    for (int i = 0; i < macroArgCount; i++)
    {
        fprintf(argtabFile, "ARG[%d] = %s\n", i, argTab[i]);
    }
    fprintf(argtabFile, "\n");

    int i = macroStart;

    // Skip macro header line
    while (Deftab[i] != '\n' && i < macroEnd)
        i++;
    i++;

    for (; i < macroEnd; i++)
    {
        char c = Deftab[i];

        if (c == '?')
        {
            int argNumber = Deftab[i + 1] - '0';
            fprintf(outputFile, "%s ", argTab[argNumber]);
            i++;
        }
        else if (c == '$')
        {
            fprintf(outputFile, "%d", callCount);
        }
        else
        {
            fprintf(outputFile, "%c", c);
        }
    }
    fprintf(outputFile, "\n");
}

int main()
{
    FILE *inputFile = fopen("macroinput.txt", "r");
    FILE *outputFile = fopen("macrooutput.txt", "w");

    namtabFile = fopen("namtab.txt", "w");
    deftabFile = fopen("deftab.txt", "w");
    argtabFile = fopen("argtab.txt", "w");

    if (!inputFile || !outputFile || !namtabFile || !deftabFile || !argtabFile)
    {
        printf("Error opening files.\n");
        return 1;
    }

    char line[100];
    char *instructions[10];
    int instructionCount;

    while (fgets(line, 100, inputFile))
    {
        getInstructions(line, instructions, &instructionCount);
        if (instructionCount == 0)
        {
            continue;
        }
        if (strcmp(instructions[0], "END") == 0)
        {
            fprintf(outputFile, "END\n");
            break;
        }
        else if (strcmp(instructions[1], "MACRO") == 0)
        {
            defineMacro(inputFile, instructions, instructionCount);
        }
        else if (isMacro(instructions[0]) != -1)
        {
            expandMacro(outputFile, instructions[0], &instructions[1], instructionCount - 1);
        }
        else if (isMacro(instructions[1]) != -1)
        {
            fprintf(outputFile, "%s ", instructions[0]);  // Print the label
            expandMacro(outputFile, instructions[1], &instructions[2], instructionCount - 2);
        }
        else
        {
            for (int i = 0; i < instructionCount; i++)
            {
                fprintf(outputFile, "%s ", instructions[i]);
            }
            fprintf(outputFile, "\n");
        }
    }

    // === Write Namtab to file ===
    for (int i = 0; i < NamtabCount; i++)
    {
        fprintf(namtabFile, "Name: %s Start: %d End: %d CLCount: %d\n",
                Namtabs[i].name, Namtabs[i].start, Namtabs[i].end, Namtabs[i].clcnt);
    }

    // === Write Deftab to file ===
    fprintf(deftabFile, "%s", Deftab);

    // === Close all files ===
    fclose(inputFile);
    fclose(outputFile);
    fclose(namtabFile);
    fclose(deftabFile);
    fclose(argtabFile);

    printf("Macro processing complete.\nCheck 'macrooutput.txt', 'namtab.txt', 'deftab.txt', and 'argtab.txt'.\n");
    return 0;
}