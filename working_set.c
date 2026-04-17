#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MB (1024UL * 1024)
#define PAGE 4096

int main(int argc, char *argv[]) {
    unsigned long total_mb  = argc > 1 ? atol(argv[1]) : 256;
    unsigned long stride_mb = argc > 2 ? atol(argv[2]) : 64;
    int iterations = argc > 3 ? atoi(argv[3]) : 5;

    unsigned long total_bytes  = total_mb * MB;
    unsigned long stride_bytes = stride_mb * MB;

    char *region = malloc(total_bytes);
    memset(region, 0, total_bytes);

    printf("Total: %lu MB, Working set: %lu MB\n\n", total_mb, stride_mb);

    for (int iter = 0; iter < iterations; iter++) {
        struct timespec start, end;

        clock_gettime(CLOCK_MONOTONIC, &start);

        volatile char x = 0;
        for (unsigned long i = 0; i < stride_bytes; i += PAGE)
            x += region[i];

        clock_gettime(CLOCK_MONOTONIC, &end);

        double t = (end.tv_sec - start.tv_sec)
                 + (end.tv_nsec - start.tv_nsec) / 1e9;

        printf("Iteration %d: %.4f sec\n", iter, t);
    }

    free(region);
    return 0;
}
