#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MB (1024UL * 1024)

int main(void) {
    FILE *f = fopen("/proc/meminfo", "r");
    unsigned long memtotal_kb;
    fscanf(f, "MemTotal: %lu kB", &memtotal_kb);
    fclose(f);

    unsigned long alloc_mb = (memtotal_kb / 1024) * 80 / 100;
    printf("Allocating %lu MB\n", alloc_mb);

    char *region = malloc(alloc_mb * MB);

    printf("Touching memory...\n");
    for (unsigned long i = 0; i < alloc_mb * MB; i += 4096)
        region[i] = 1;

    printf("Sleeping... check vmstat\n");
    sleep(5);

    printf("Re-reading...\n");
    volatile char x = 0;
    for (unsigned long i = 0; i < alloc_mb * MB; i += 4096)
        x += region[i];

    printf("\nMemory status:\n");
    system("grep -E 'VmRSS|VmSwap' /proc/self/status");

    free(region);
    return 0;
}
