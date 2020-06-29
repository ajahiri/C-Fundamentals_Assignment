#ifndef ENCRYPTION_H
#define ENCRYPTION_H

/* Encryption/Decryption Functions go Here */
void encrypt(char* plain_text, int text_length, const char* key, 
             int key_length);
void decrypt(char* plain_text, int text_length, const char* key, 
             int key_length);

#endif