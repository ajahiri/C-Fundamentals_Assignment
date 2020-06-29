/* Libraries */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#define MAXUSERNAME_LENGTH 30
#define MAXPASSWORD_LENGTH 30
#define FILE_NAME "PASSWORDS"

/* Structure definitions */
struct database_entry {
    int key; /*Key to be able to search and delete entries.*/
    char username[MAXUSERNAME_LENGTH];
    char password[MAXPASSWORD_LENGTH];
};
typedef struct database_entry database_entry_t;

/* Function Prototypes */
void main_menu();

void load_database();
void save_database();

void database_menu();
void add_entry();
void input_password(char* entry_password);
char* generate_password();
void display_data();
void remove_entry();