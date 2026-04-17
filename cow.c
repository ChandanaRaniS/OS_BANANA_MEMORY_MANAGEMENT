#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MB (1024 * 1024)
#define ALLOC_MB 128

static long get_minor_faults(void) {
    FILE *f = fopen("/proc/self/stat", "r");
    char line[4096];
    fgets(line, sizeof(line), f);
    fclose(f);

    char *p = strrchr(line, ')');
    p += 2;

    long minflt;
    char state;
    int dummy;
    unsigned long flags;

    sscanf(p, "%c %d %d %d %d %d %lu %ld",
           &state, &dummy, &dummy, &dummy, &dummy, &dummy, &flags, &minflt);

    return minflt;
}

int main(void) {
    char *region = malloc(ALLOC_MB * MB);
    memset(region, 'P', ALLOC_MB * MB);

    printf("Parent: touched %d MB\n", ALLOC_MB);

    pid_t pid = fork();

    if (pid == 0) {
        long start = get_minor_faults();

        // Read only
        volatile char x = 0;
        for (int i = 0; i < ALLOC_MB * MB; i += 4096)
            x += region[i];

        long after_read = get_minor_faults();

        printf("\nREAD phase faults: %ld\n", after_read - start);

        long before_write = get_minor_faults();

        memset(region, 'C', ALLOC_MB * MB);

        long after_write = get_minor_faults();

        printf("WRITE phase faults: %ld\n", after_write - before_write);
        printf("Expected: %d\n", ALLOC_MB * 256);

        _exit(0);
    }

    wait(NULL);
    free(region);
    return 0;
}
