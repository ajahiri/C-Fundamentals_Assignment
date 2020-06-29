#include "encryption.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*******************************************************************************
* This function will trigger the encryption for the file. This will most likely
* make calls to many functions in the encryption.h library.
* author:
* - Arian Jahiri 13348469
* inputs:
* - plain_text: The string that is to be encrypted.
* - text_length: The length of the array of text that is to be encrypted.
* - key: The key or "master_password" used for both encryption and decryption.
* - key_length: The length of the master password char array.
* outputs:
* - none
*******************************************************************************/
void encrypt(char* plain_text, int text_length, const char* key, int key_length)
{
    /* Encrypt every character using the master_password/key */
    int i;
    for (i=0; (i < text_length && plain_text[i] != '\0'); i++) {
        plain_text[i] = plain_text[i] + key[i % key_length];
    };
};

/*******************************************************************************
* This function will trigger the encryption for the file. This will most likely
* make calls to many functions in the encryption.h library.
* author:
* - Arian Jahiri 13348469
* inputs:
* - decrypted_text: The string that is to be decrypted.
* - text_length: The length of the array of text that is to be encrypted.
* - key: The key or "master_password" used for both encryption and decryption.
* - key_length: The length of the master password char array.
* outputs:
* - none
*******************************************************************************/
void decrypt(char* decrypted_text, int text_length, const char* key, int key_length)
{
    /* Encrypt every character using the master_password/key */
    int i;
    for (i=0; (i < text_length && decrypted_text[i] != '\0'); i++) {
        decrypted_text[i] = decrypted_text[i] - key[i % key_length];
    };
};