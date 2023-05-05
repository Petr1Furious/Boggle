#include <stdbool.h>

struct Grid {
    size_t size;
    char* letters;
};

bool find_word(struct Grid* grid, char* word);

void print_grid(struct Grid* grid);
