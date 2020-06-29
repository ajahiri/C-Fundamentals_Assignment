#ifndef COMPRESSION_H
#define COMPRESSION_H

#define MAX_ENCODED_LEN 60

/* Compression Functions go Here */
char* compress(char* text, int text_length);
char* decompress(char* text, int text_length);

#endif