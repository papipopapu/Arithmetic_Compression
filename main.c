#include "utf8_bisect.h"

int main()
{
    // main
    compress_file("alice29.txt", "c1.bin");
    compress_file("c1.bin", "c2.bin");
    decompress_file("c2.bin", "o2.bin");
    decompress_file("o2.bin", "o1.bin");

    return 0;
}