#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

static void query_pagemap(const char *label, void *vaddr) {
    uint64_t data;
    long page_size = sysconf(_SC_PAGESIZE);
    unsigned long vpn = (unsigned long)vaddr / page_size;

    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) { perror("open"); return; }

    lseek(fd, vpn * 8, SEEK_SET);
    read(fd, &data, 8);
    close(fd);

    int present  = (data >> 63) & 1;
    int swapped  = (data >> 62) & 1;
    uint64_t pfn = data & 0x7FFFFFFFFFFFFF;

    printf("%-12s VA=%p present=%d swapped=%d",
           label, vaddr, present, swapped);

    if (present)
        printf(" PFN=0x%lx", (unsigned long)pfn);
    else
        printf(" (no frame)");

    printf("\n");
}

int main(void) {
    char *p = malloc(4096 * 4);

    printf("=== BEFORE TOUCH ===\n");
    query_pagemap("page[0]", p);
    query_pagemap("page[1]", p + 4096);
    query_pagemap("page[2]", p + 8192);

    // Touch only first two pages
    p[0] = 'X';
    p[4096] = 'Y';

    printf("\n=== AFTER TOUCH ===\n");
    query_pagemap("page[0]", p);
    query_pagemap("page[1]", p + 4096);
    query_pagemap("page[2]", p + 8192);

    free(p);
    return 0;
}
