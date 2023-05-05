#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

enum {
    MIN_ARGS_COUNT = 2,
    ERROR_EXIT_CODE = 84,
};

void print_usage_and_exit(char* binary_name) {
    fprintf(stderr, "Usage: %s -g GRID [-s SIZE] [-w WORD]\n"
                    "-s SIZE defines the size of a size of the grid (default: 4).\n"
                    "-g GRID specifies a hard-coded grid.\n"
                    "-w WORD force the word to be looked for.\n",
            binary_name);
    exit(ERROR_EXIT_CODE);
}

bool try_word(struct Grid* grid, char* word) {
    if (strlen(word) > grid->size * grid->size) {
        printf("The word \"%s...\" is not in the grid.\n", word);
        return false;
    }

    if (find_word(grid, word)) {
        print_grid(grid);
        return true;
    } else {
        printf("The word \"%s\" is not in the grid.\n", word);
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 1 + MIN_ARGS_COUNT || strcmp(argv[1], "-g") != 0) {
        print_usage_and_exit(argv[0]);
    }

    char* letters = argv[2];

    char* selected_word = NULL;
    size_t size = 4;

    int cur_arg = 1 + MIN_ARGS_COUNT;
    while (cur_arg < argc) {
        if (strcmp(argv[cur_arg], "-s") == 0) {
            if (cur_arg + 1 >= argc) {
                fprintf(stderr, "Expected SIZE.\n");
                print_usage_and_exit(argv[0]);
            }

            errno = 0;
            char* end;
            size = strtol(argv[cur_arg + 1], &end, 10);
            if (errno || *end != '\0' || size <= 0) {
                fprintf(stderr, "SIZE must be an integer greater than 0.\n");
                print_usage_and_exit(argv[0]);
            }
        } else if (strcmp(argv[cur_arg], "-w") == 0) {
            if (cur_arg + 1 >= argc) {
                fprintf(stderr, "Expected WORD.\n");
                print_usage_and_exit(argv[0]);
            }

            selected_word = argv[cur_arg + 1];
        } else {
            fprintf(stderr, "Unrecognized argument: %s\n", argv[cur_arg]);
            print_usage_and_exit(argv[0]);
        }
        cur_arg += 2;
    }

    if (size * size != strlen(letters)) {
        fprintf(stderr, "The grid does not contain the correct number of characters.\n");
        return ERROR_EXIT_CODE;
    }

    struct Grid grid = {size, letters};

    if (selected_word != NULL) {
        try_word(&grid, selected_word);
    } else {
        print_grid(&grid);

        selected_word = calloc(1, size * size + 1 + 1);
        if (selected_word == NULL) {
            fprintf(stderr, "Could not allocate memory.\n");
            return ERROR_EXIT_CODE;
        }

        int c;
        size_t pos = 0;
        while ((c = getchar()) != EOF) {
            if (c == '\n') {
                selected_word[pos] = '\0';
                if (try_word(&grid, selected_word)) {
                    break;
                }
                pos = 0;
            } else if (pos < size * size + 1) {
                selected_word[pos++] = c;
            }
        }

        free(selected_word);
    }
    return 0;
}
