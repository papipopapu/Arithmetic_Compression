#include "utf8_bisect.h"
int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s [-dc] [input_file] [output_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'c': 
                compress_file(argv[2], argv[3]);
                break;
            case 'd': 
                decompress_file(argv[2], argv[3]);
                break;
            default:
                fprintf(stderr, "Usage: %s [-dc] [input_file] [output_file]\n", argv[0]);
                exit(EXIT_FAILURE);
        }   
    } else {
        fprintf(stderr, "Usage: %s [-dc] [input_file] [output_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    return 0;
}