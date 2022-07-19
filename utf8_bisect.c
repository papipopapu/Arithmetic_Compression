#include "utf8_bisect.h"

static int buffer;
static int free_bits;
static FILE *comp_file;
static int cum_freq[n_symbols + 2]; 
static int freq[n_symbols + 1];

void compress_file(char*in_filename, char*out_filename) {
    start_model();
    start_write_bit(out_filename);
    FILE *in_file = fopen(in_filename, "r");
    if (in_file == NULL) {
        printf("Error opening file %s\n", in_filename);
        exit(1);
    }
    int symbol = fgetc(in_file);
    while (symbol != EOF) {
        encode_symbol(symbol);
        symbol = fgetc(in_file);
    } 
    encode_symbol(256); // EOF
    end_write_bit();
    fclose(in_file);
}
void decompress_file(char*in_filename, char*out_filename) {
    start_model();
    start_read_bit(in_filename);
    FILE *out_file = fopen(out_filename, "w");
    if (out_file == NULL) {
        printf("Error opening file %s\n", out_filename);
        exit(1);
    }
    int symbol = decode_symbol();
    while (symbol != 256) { // EOF
        fputc(symbol, out_file);
        symbol = decode_symbol();
    } 
    end_read_bit();
    fclose(out_file);
}
void encode_symbol(int symbol)
{
    int high, low, top, bottom, half;
    high = cum_freq[0];
    low = 0;

    int iter = 0;
    top = cum_freq[symbol];
    bottom = cum_freq[symbol + 1];
    while (!(high <= top && low >= bottom)) { // while symbol cum_freq interval isnt entirely contained between low and high [high, low]
        half = (high + low) / 2;
        if (top - half >= half - bottom) { // if there is a bigger (or equal) chunk of the symbol cum_freq interval in the high half
            write_bit(1);
            low = half;
        }
        else {
            write_bit(0);
            high = half;
        }
    }
    update_model(symbol);
}
int search_symbol(int high, int low)
{
    for (int i = 0; i < n_symbols; i++) {
        if (cum_freq[i] >= high && cum_freq[i + 1] <= low) return i;
        if (cum_freq[i + 1] < high) return -1;
    }
}
int decode_symbol()
{
    int high, low, half, symbol = -1;
    high = cum_freq[0]; low = 0;
    while (symbol < 0) {
        half = (high + low) / 2;
        if (read_bit()) low = half;      
        else high = half;
        symbol = search_symbol(high, low);
    }
    update_model(symbol);
    return symbol;
}

void start_model()
{
    cum_freq[n_symbols+1] = 0; cum_freq[n_symbols] = 1; freq[n_symbols] = 1;
    for (int i = n_symbols - 1; i >= 0; i--) {
        freq[i] = 1;
        cum_freq[i] = 1 + cum_freq[i + 1]; // first value is the biggest
    }
}
void update_model(int symbol)
{
    if (cum_freq[0] == max_freq) {
        freq[n_symbols] = (freq[n_symbols] + 1) / 2;
        cum_freq[n_symbols] = freq[n_symbols] + 0; // no need to update the last cum freq, since its 0
        for (int i = n_symbols - 1; i >= 0; i--) {
            freq[i] = (freq[i] + 1) / 2;
            cum_freq[i] = freq[i] + cum_freq[i + 1];
        }
    }
    freq[symbol]++;
    for (int i = symbol; i >= 0; i--) {
        cum_freq[i] += 1;
    }
}
void start_read_bit(char*comp_filename)
{
    comp_file = fopen(comp_filename, "rb");
    if (comp_file == NULL) {
        printf("Error opening file %s\n", comp_filename);
        exit(1);
    }
    free_bits = 8;
}
void start_write_bit(char*comp_filename)
{
    comp_file = fopen(comp_filename, "wb");
    if (comp_file == NULL) {
        printf("Error opening file %s\n", comp_filename);
        exit(1);
    }
    free_bits = 8;
}
void end_write_bit()
{
    if (free_bits != 8) {
        buffer >>= free_bits;
        fputc(buffer, comp_file);
    }
    fclose(comp_file);
}
void end_read_bit()
{
    fclose(comp_file);
}
int read_bit()
{
    int bit;
    if (free_bits == 8) {
        free_bits = 0;
        buffer = fgetc(comp_file);
    }
    bit = buffer & 1; 
    buffer >>= 1;
    free_bits++;
    return bit;
}
void write_bit(int bit)
{
    if (free_bits == 0) {
        fputc(buffer, comp_file);
        free_bits = 8;
    }
    buffer >>= 1;
    if (bit) buffer |= 0b10000000;
    free_bits--;
}

