#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

const int n_symbols = 3;
const int max_freq = 16383; // 2*14 - 1
static int buffer;
static int free_bits;
static int eof;
static FILE *file;

static int cum_freq[n_symbols]; // length to 64 first bits of file // first lets do symbols 'a', 'b', 'c'
static int freq[n_symbols]; 

//static int symbol_to_idx[n_symbols]; // symbol to index in freq array
//static int idx_to_symbol[n_symbols]; // index to symbol in freq array

// utf-8 == index in freq array for now
// ahora se llama metodo de bisseccion, encodear ahsta que intervalo este enteramente entre los cum_freq
// de un simbolo
// quizas añadir cum_frq[n_symbols] = 0

void encode_symbol(int symbol) {
    
}
void start_model() {
    cum_freq[n_symbols - 1] = 1;
    freq[n_symbols - 1] = 1;
    for (int i = n_symbols - 2; i >= 0; i--) {
        freq[i] = 1;
        cum_freq[i] = 1 + cum_freq[i + 1]; // first value is the biggest  
    }
}
void update_model(int symbol) {
    if (cum_freq[0] == max_freq) {
        freq[n_symbols-1] = (freq[n_symbols-1] + 1) / 2;
        cum_freq[n_symbols-1] = freq[n_symbols-1] + 0;
        for (int i = n_symbols-2; i >= 0; i--) {
            freq[i] = (freq[i] + 1) / 2;
            cum_freq[i] = freq[i] + cum_freq[i+1];
        }
    }
    freq[symbol]++;
    for (int i = symbol; i >= 0; i--) {
        cum_freq[i] += 1;
    }
}
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
    // for actual implementation, follow acm87 scheme
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