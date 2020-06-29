#include "compression.h"

/* Using run length encoding to do the compression of the file */

/*******************************************************************************
* This function will encode the input string (text) using run length encoding.
* author:
* - Arian Jahiri
* inputs:
* - text: Pointer to the text that needs to be encoded.
* - text_length: length of the text input.
* outputs:
* - char pointer to the encoded string.
*******************************************************************************/
/* Returns the Run Length Encoded string for the  
   source string src */
char* encode(char* text, int text_length) 
{ 
    int rLen; 
    char count[MAX_ENCODED_LEN];
  
    /*
     * Possible worst case scenario for run length encoding results in
     * encoded string that is 2x length of the input.
     */
    char* encoded = (char*)malloc(sizeof(char) * (text_length * 2 + 1)); 
  
    int i, j = 0, k; 
  
    /* Traverse the input string one by one */
    for (i = 0; i < text_length; i++) { 
  
        /* Copy the first occurrence of the new character */
        encoded[j++] = text[i]; 
  
        /* Count the number of occurrences of the new character */
        rLen = 1; 
        while (i + 1 < text_length && text[i] == text[i + 1]) { 
            rLen++; 
            i++; 
        } 
  
        /* Store rLen in a character array count[] */
        sprintf(count, "%d", rLen); 
  
        /* Copy the count[] to destination */
        for (k = 0; *(count + k); k++, j++) { 
            encoded[j] = count[k]; 
        } 
    } 
  
    /*terminate the destination string */
    encoded[j] = '\0'; 
    return encoded; 
} 

/*******************************************************************************
* This function will decode an encoded string that was encoded using run length
* encoding.
* author:
* - Arian Jahiri
* inputs:
* - text: Pointer to the text that needs to be decoded.
* - text_length: length of the text input.
* outputs:
* - char pointer to the decoded string.
*******************************************************************************/
char* decode(char* text, int text_length) 
{
    char* decoded = (char*)malloc(sizeof(char) * (MAXUSERNAME_LENGTH + 1)); 
    int i = 0; 
    int k = 0; /*Track where we're upto in the decoded string*/

    while (i < text_length)
    {
        /*(i+1)th char is always the amt of times char occurs*/
        int char_count = (int)text[i+1] - 48; 
        /*printf("COMPRESSION.H Char count %d\n", char_count);*/
        int j;
        for (j=0; j < char_count; j++) 
        {
            decoded[k] = text[i];
            k++;
        }
        i = i + 2; /*Go to next (char,number) couple in the array*/
    }
    /*Terminate string*/
    decoded[i+1] = '\0'; 
    return decoded; 
}
