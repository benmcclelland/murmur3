# murmur3

```sh
$ make
cc    -c -o main.o main.c
cc    -c -o murmur3.o murmur3.c
cc    -c -o PMurHash128.o PMurHash128.c
cc main.o murmur3.o PMurHash128.o -o hashtest

$ ./hashtest
HASH                      SIZE  time(s)  perf(MB/s)  Digest
-----------------------------------------------------------
MurmurHash3_x64_128       16    0.100    1000.000    4631346889031e71fd37a661cfb81713
PMurHash128x64            16    0.120     833.333    4631346889031e71fd37a661cfb81713
PMurHash128x64 - Chunked  16    0.120     833.333    4631346889031e71fd37a661cfb81713
```