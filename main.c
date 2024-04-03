#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define SLEEP_SEC 86400
#define BLOCK_SIZE_MB 256

int main() {
    uint32_t mem_mb = 0;
    uint32_t mem_do_free = 0;

    char *envin_mem_mb = getenv("MEM_ALLOCATE_MB");
    char *envin_mem_do_free = getenv("MEM_DO_FREE");

    if(envin_mem_mb == NULL) {
        printf("Environment variable MEM_ALLOCATE_MB empty, exiting\n");
        return EXIT_FAILURE;
    }

    mem_mb = atoi(envin_mem_mb);

    if(mem_mb == 0) {
        printf("Environment variable MEM_ALLOCATE_MB has an invalid or zero value, exiting\n");
        return EXIT_FAILURE;
    }

    if(envin_mem_do_free != NULL) {
        mem_do_free = atoi(envin_mem_do_free);

        if(mem_do_free < 0 || mem_do_free > 1) {
            printf("Environment variable MEM_DO_FREE can only be 1 or 0, exiting\n");
            return EXIT_FAILURE;
        }
    }

    printf("Running memory eater with %d MB memory\n", mem_mb);

    void *mem_alloc = malloc((uint64_t)mem_mb * 1024 * 1024);

    if(mem_alloc == NULL) {
        printf("Failed to allocate memory, malloc failed\n");
        return EXIT_FAILURE;
    }

    uint64_t offset = 0;
    uint64_t mem_mb_last = 0;
    for(uint64_t i = 0; i < (mem_mb / BLOCK_SIZE_MB); i++) {
        printf("Allocating block %lu, %luMB to %luMB\n", i, i * BLOCK_SIZE_MB, (i+1) * BLOCK_SIZE_MB);
        mem_mb_last = (i+1) * BLOCK_SIZE_MB;

        offset = i * BLOCK_SIZE_MB * 1024 * 1024;

        memset(mem_alloc + offset, 0xFF, BLOCK_SIZE_MB * 1024 * 1024);
    }

    uint64_t mem_mb_remaining = mem_mb - mem_mb_last;
    if(mem_mb_remaining > 0) {
        printf("Allocating remaining block %luMB to %luMB\n", mem_mb_last, mem_mb_last + mem_mb_remaining);

        offset = mem_mb_last * 1024 * 1024;

        memset(mem_alloc + offset, 0xFF, mem_mb_remaining * 1024 * 1024);
    }

    if(mem_do_free == 1) {
        free(mem_alloc);
        printf("%dMB memory allocated and immediately freed, sleeping for %d seconds\n", mem_mb, SLEEP_SEC);
    } else {
        printf("%dMB memory allocated, sleeping for %d seconds\n", mem_mb, SLEEP_SEC);
    }

    sleep(SLEEP_SEC);

    if(mem_do_free == 0)
        free(mem_alloc);

    return EXIT_SUCCESS;
}