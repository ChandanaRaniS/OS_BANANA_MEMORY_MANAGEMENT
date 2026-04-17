#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MB (1024UL * 1024)
#define PAGE 4096

int main(void) {
    FILE *f = fopen("/proc/meminfo", "r");
    unsigned long memtotal_kb;
    fscanf(f, "MemTotal: %lu kB", &memtotal_kb);
    fclose(f);

    unsigned long total_ram_mb = memtotal_kb / 1024;

    printf("%-10s %-10s %-15s %-10s\n",
           "Alloc", "Time", "Throughput", "Status");

    int percents[] = {25, 50, 75, 100, 130};

    for (int i = 0; i < 5; i++) {
        unsigned long mb = total_ram_mb * percents[i] / 100;
        unsigned long bytes = mb * MB;

        char *region = malloc(bytes);

        for (unsigned long j = 0; j < bytes; j += PAGE)
            region[j] = 1;

        struct timespec s, e;
        clock_gettime(CLOCK_MONOTONIC, &s);

        volatile char x = 0;
        for (unsigned long j = 0; j < bytes; j += PAGE)
            x += region[j];

        clock_gettime(CLOCK_MONOTONIC, &e);

        double t = (e.tv_sec - s.tv_sec)
                 + (e.tv_nsec - s.tv_nsec) / 1e9;

        const char *status = (t < 1) ? "OK" : (t < 5) ? "SLOW" : "THRASH";

        printf("%-10lu %-10.2f %-15.0f %-10s\n",
               mb, t, (bytes/PAGE)/t, status);

        free(region);
        sleep(2);
    }

    return 0;
}
