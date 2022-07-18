#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int buffer;
static int free_bits;
static int eof;
static FILE *file;

void start_read_bit(const char *filename) {
    file = fopen(filename, "rb");
    free_bits = 8;
    eof = 0;
}
void start_write_bit(const char *filename) {
    file = fopen(filename, "wb");
    free_bits = 8;
}
void end_write_bit() {
    if (free_bits != 8) {
        buffer <<= free_bits;
        fputc(buffer, file);
    }
    
    fclose(file);
}
void end_read_bit() {
    fclose(file);
}
int read_bit() {
    int bit;
    if (free_bits == 8) {
        free_bits = 0;
        buffer = fgetc(file);
        if (buffer == EOF) {
            eof = 1;
            return 0; // last bit is bs
        }
    }
    bit = (buffer & 0b10000000) >> 7; // slower but we can now write utf-8 and read it from .txt
    buffer <<= 1;
    free_bits++;
    return bit;
}
void write_bit(int bit) {
    if (free_bits == 0) {
        fputc(buffer, file);
        free_bits = 8;
    }
    buffer <<= 1;
    if (bit) buffer |= 1;
    free_bits--;
}





int main() {

    start_write_bit("test.txt");
    write_bit(1);
    end_write_bit();

    start_read_bit("test.txt");
    printf("%d\n", read_bit());
    end_read_bit();

    return 0;
}