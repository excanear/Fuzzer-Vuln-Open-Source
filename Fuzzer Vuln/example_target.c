// example_target.c
// Simple target for fuzzing demonstration
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/instrumentation/instrumentation.h"

int main(int argc, char *argv[])
{
    init_instrumentation();
    printf("Instrumentation initialized\n");

    if (argc < 2)
        return 0;

    FILE *f = fopen(argv[1], "rb");
    if (!f)
        return 0;

    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *input = malloc(len + 1);
    fread(input, 1, len, f);
    fclose(f);
    input[len] = 0;

    printf("Input: %s\n", input);

    instrument_basic_block(1);

    if (len > 0 && input[0] == 'A')
    {
        instrument_basic_block(2);
        printf("Block 2\n");
        if (len > 1 && input[1] == 'B')
        {
            instrument_basic_block(3);
            printf("Block 3\n");
            if (len > 2 && input[2] == 'C')
            {
                instrument_basic_block(4);
                printf("Block 4\n");
                // Potential crash
                if (len > 3 && input[3] == 'D')
                {
                    instrument_basic_block(5);
                    printf("Block 5 - crashing\n");
                    *(volatile int *)0 = 0; // SIGSEGV
                }
            }
        }
    }

    free(input);
    printf("Done\n");
    return 0;
}