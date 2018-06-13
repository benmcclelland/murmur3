#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "PMurHash128.h"
#include "murmur3.h"

#define MSGSZ 104857600

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t n;

    if (size) {
        --size;
        for (n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size)
{
     char *s = malloc(size + 1);
     if (s) {
         rand_string(s, size);
     }
     return s;
}

static void printres (char *name, int hashlen, double time_taken, int msglen, unsigned char* hash) {
    int i;

    printf("%-25s %2d    %.3f    %8.3f    ", name, hashlen, time_taken, msglen/time_taken/1048576);
    for(i = 0; i < hashlen; i++) printf("%02x", hash[i]);
    printf("\n");
}

#define START do { t = clock(); } while(0);
#define STOP do {                            \
    t = clock() - t;                         \
    output_len = 16;                         \
    time_taken = ((double)t)/CLOCKS_PER_SEC; \
 } while(0);

int main()
{
    clock_t t;
    char* input;
    int i, output_len;
    double time_taken;
    unsigned char murmurhash[16];
    uint32_t seed = 42;
    uint64_t carry[2] = {0, 0};
    uint64_t h[2] = {seed, seed};

    input = rand_string_alloc(MSGSZ);
    if (!input) {
        perror("alloc input");
        exit(1);
    }

    printf("HASH                      SIZE  time(s)  perf(MB/s)  Digest\n");
    printf("-----------------------------------------------------------\n");

    START
    MurmurHash3_x64_128(input, MSGSZ, seed, murmurhash);
    STOP
    printres("MurmurHash3_x64_128", output_len, time_taken, MSGSZ, murmurhash);

    START
    PMurHash128x64(input, MSGSZ, seed, murmurhash);
    STOP
    printres("PMurHash128x64", output_len, time_taken, MSGSZ, murmurhash);

    START
    for (i = 0; i < 10; i++) {
        PMurHash128_Process(h, carry, &input[i*(MSGSZ/10)], MSGSZ/10);
    }
    PMurHash128_Result(h, carry, MSGSZ, (uint64_t *)murmurhash);
    STOP
    printres("PMurHash128x64 - Chunked", output_len, time_taken, MSGSZ, murmurhash);
}
