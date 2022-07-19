#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define n_symbols 257 // 256 + 1 for EOF
#define max_freq 16383 // 2*14 - 1
#define MAX_ITER 100


#define cum_max_idx (n_symbols + 1) // EOF + 0 last value
#define cum_EOF_idx n_symbols

#define freq_max_idx n_symbols
#define freq_EOF_idx n_symbols

void decompress_file(char*in_filename, char*out_filename);
void compress_file(char*in_filename, char*out_filename);


void encode_symbol(int symbol);
int decode_symbol();

int search_symbol(int high, int low);

void start_model();
void update_model(int symbol);

void start_read_bit(char*filename);
void start_write_bit(char*filename);

void end_write_bit();
void end_read_bit();

int read_bit();
void write_bit(int bit);


