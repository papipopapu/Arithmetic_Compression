#include "utf8_bisect.h"

int main()
{

    compress_file("c1.png", "compressed.bin");
    decompress_file("compressed.bin", "out.png");

    return 0;
}