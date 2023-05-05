#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "grid.h"

char* get_letter_pointer(struct Grid* grid, size_t x, size_t y) {
    return grid->letters + (grid->size * x + y);
}

bool dfs(struct Grid* grid, char* word, bool* used, size_t x, size_t y) {
    if (*(word + 1) == '\0') {
        char* cur_letter = get_letter_pointer(grid, x, y);
        *cur_letter = toupper(*cur_letter);
        return true;
    }
    used[grid->size * x + y] = true;

    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};
    for (size_t i = 0; i < 4; i++) {
        int nx = (int) x + dx[i], ny = (int) y + dy[i];

        if (nx < 0 || (size_t) nx >= grid->size || ny < 0 || (size_t) ny >= grid->size) {
            continue;
        }
        if (*get_letter_pointer(grid, nx, ny) != *(word + 1) || used[grid->size * nx + ny]) {
            continue;
        }

        if (dfs(grid, word + 1, used, nx, ny)) {
            char* cur_letter = get_letter_pointer(grid, x, y);
            *cur_letter = toupper(*cur_letter);
            return true;
        }
    }

    used[grid->size * x + y] = false;
    return false;
}

bool find_word(struct Grid* grid, char* word) {
    bool used[grid->size * grid->size];
    memset(used, 0, sizeof(used));

    for (size_t x = 0; x < grid->size; x++) {
        for (size_t y = 0; y < grid->size; y++) {
            if (*get_letter_pointer(grid, x, y) == *word && dfs(grid, word, used, x, y)) {
                return true;
            }
        }
    }

    return false;
}

void print_separator(size_t len) {
    for (size_t x = 0; x < len; x++) {
        printf("+");
    }
    printf("\n");
}

void print_grid(struct Grid* grid) {
    print_separator(grid->size * 2 + 3);
    for (size_t x = 0; x < grid->size; x++) {
        printf("| ");
        for (size_t y = 0; y < grid->size; y++) {
            printf("%c ", *get_letter_pointer(grid, x, y));
        }
        printf("|\n");
    }
    print_separator(grid->size * 2 + 3);
}
