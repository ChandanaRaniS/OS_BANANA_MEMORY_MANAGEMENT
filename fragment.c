#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POOL_SIZE (4 * 1024 * 1024)
#define BLOCK_SIZE 4096
#define NUM_BLOCKS (POOL_SIZE / BLOCK_SIZE)

int main(void) {
    char *pool = malloc(POOL_SIZE);

    int allocated[NUM_BLOCKS];
    memset(allocated, 1, sizeof(allocated));

    printf("Total blocks: %d\n", NUM_BLOCKS);

    int freed = 0;
    for (int i = 0; i < NUM_BLOCKS; i += 2) {
        allocated[i] = 0;
        freed++;
    }

    printf("Freed blocks: %d\n", freed);

    int need = 128;
    int found = 0, run = 0;

    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (!allocated[i]) {
            run++;
            if (run >= need) { found = 1; break; }
        } else {
            run = 0;
        }
    }

    if (found)
        printf("SUCCESS\n");
    else
        printf("FAILED - fragmentation\n");

    printf("\n--- Internal Fragmentation ---\n");

    char *a = malloc(1);
    char *b = malloc(1);

    printf("Distance: %ld bytes\n", (long)(b - a));

    free(a);
    free(b);
    free(pool);
    return 0;
}
