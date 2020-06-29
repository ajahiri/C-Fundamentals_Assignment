#include "userpass_database.h"

#include <stdio.h> /* 
                    * scanf, printf, fscanf, fprintf, fopen, fclose, getchar,
                    * fgets                                                 
                    */
#include <string.h> /* strncmp, strlen, strcpy */
#include <math.h> 
#include <stdlib.h> /* srand, rand */
#include "encryption.c" /* encrypt, decrypt */
#include "compression.c" /* encode, decode */

/* Using a linked list data structure to store the entries. */
struct node {
    database_entry_t data;
    struct node* nextp;
};
typedef struct node node_t;

/*Create our linkedlist pointer (head)*/
node_t* database_listp;
int element_counter = 1;
int debug_mode = 0; /*Global var to check if we're in debug mode.*/

/*******************************************************************************
* This function prints the main menu of the application, here the user can 
* choose between the main functions (open existing DB, create new DB)
* author:
* - Arian Jahiri 13348469
* inputs:
* - none
* outputs:
* - none
*******************************************************************************/
void main_menu(int debug)
{
    if (debug == 1) debug_mode = 1; /*Enable debug mode*/
    /*Initialise our linked list pointer.*/
    database_listp = NULL; /* List should start as null with no entries. */
      
    int option = 0;
    int done = 0;
    /* call the appropriate function based on users choice */
    while (done != 1) {
        printf( 
            "\n"
            "1. Start with a fresh database.\n"
            "2. Load an existing database.\n"
            "3. Exit program.\n"
        );
        scanf("%d", &option);
        switch (option)
        {
            case 1:
                database_menu();
                break;
            case 2:
                load_database();
                break;
            case 3:
                done = 1;
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }
    

    /* Free memory and NULL pointer  */
    free(database_listp);
    database_listp = NULL;
};

/*******************************************************************************
* This function will show the menu specific for the database functions itself
* these include add/remove entries, save DB, display entries, generate pw, 
* remove entry, etc.
* author:
* - Arian Jahiri 13348469
* inputs:
* - none
* outputs:
* - none
*******************************************************************************/
void database_menu()
{
    /* Database menu code here */
    int done = 0; /* Means to break the loop. */
    int menuInput = 0;

    while (done != 1) {
        printf("\n"
                "1. add an entry\n"
                "2. remove an entry\n"
                "3. display database entries\n"
                "4. save database\n"
                "5. exit the database menu\n"
                "Enter choice (number between 1-5)>\n");
        scanf("%d", &menuInput);
        switch(menuInput) {
            case 1:
                add_entry();
                break;
            case 2:
                remove_entry();
                break;
            case 3:
                display_data();
                break;
            case 4:
                save_database();
                break;
            case 5:
                while(database_listp != NULL) {
                    node_t* to_delete = database_listp;
                    database_listp = database_listp->nextp;
                    free(to_delete);
                    to_delete=NULL;
                }
                done = 1;
                break;
            default:
                printf("Invalid choice\n");
        }
    }
};

/*******************************************************************************
* This function will add an entry to the password manager database.
* author:
* - Arian Jahiri 13348469
* inputs:
* - none
* outputs:
* - none
*******************************************************************************/
void add_entry()
{
    /* Create a new entry node to later add to our list. */
    node_t* newEntry = (node_t*) malloc(1*sizeof(node_t));
    newEntry->nextp = NULL; /* Give the entry a null next */

    printf("Please enter the username for the account you want to add> ");
    scanf("%s", newEntry->data.username);

    int added = 0;
    int choice;

    char* new_password;

    while(added != 1){
        printf("\n"
               "Choose from an option below:\n"
               "1. Input your own password.\n"
               "2. Generate a random password of maximum 30 characters\n"
               "Enter choice (number between 1 or 2)> ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                input_password(newEntry->data.password);
                added = 1;
                break;
            case 2:
                new_password = generate_password();
                strcpy(newEntry->data.password, new_password);

                free(new_password);
                new_password = NULL;

                added = 1;
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    if (debug_mode == 1) {
        printf("DEBUG: Added node with U:%s P:%s\n", newEntry->data.username, 
                                                    newEntry->data.password);
    }
    
    /*
     * Ask user to enter username and passwords,also option for user to generate
     * passwords randomly.
     */

    newEntry->data.key = element_counter; /*Give node unique key for deletion*/
    newEntry->nextp = database_listp; /*Add new node to the top of the list*/
    database_listp = newEntry; /*Change the head of the list*/
    
    printf("Entry addition was successful!\n"); 

    /*
     * Increment counter so we can make sure to add unique keys 
     * when adding more nodes
     */
    element_counter++; 
};

/*******************************************************************************
* This function will save and encrypt all the entries into a database file.
* - Arian Jahiri 133484679
* inputs:
* - none
* outputs:
* - none
*******************************************************************************/
void save_database() {
    if (database_listp == NULL) {
        printf("Cannot save empty list to file.\n");
        return;
    }

    char master_password[30];
    printf("Please enter a master password to encrypt the database file> ");
    scanf("%s", master_password);

    /* Save database code goes here */
    FILE* filep;
    filep = fopen(FILE_NAME, "w"); /* Open in write mode */
    node_t* curr_node = database_listp;

    /* There may be some issue writing the file, if so let user know. */
    if (filep != NULL) {
        while(curr_node != NULL) {
            char* username = (char*) malloc(sizeof(curr_node->data.username));
            strcpy(username, curr_node->data.username);
            char* password = (char*) malloc(sizeof(curr_node->data.password));
            strcpy(password, curr_node->data.password);
            
            /* 
             * Firstly compress the username and password using run length 
             * compression algorithm.
             */
            char* encoded_username = encode(username, strlen(username));
            char* encoded_password = encode(password, strlen(password));

            /* Encrypt both username and password using master_password */
            encrypt(encoded_username, strlen(encoded_username), 
                    master_password,strlen(master_password));
            encrypt(encoded_password, strlen(encoded_password), 
                    master_password,strlen(master_password));

            /* 
             * Following implementation ensures space char divides user and 
             * pass. 
             */
            fputs(encoded_username,filep);
            fputs(" ", filep);
            fputs(encoded_password, filep);
            fputs("\n", filep);

            if(debug_mode == 1) {
                printf("DEBUG: Encrypted User&Pass: %s : %s\n", 
                encoded_username, encoded_password);
            }

            free(username);
            free(password);
            free(encoded_username);
            free(encoded_password);
            username = NULL;
            password = NULL;
            encoded_username = NULL;
            encoded_password = NULL;
            
            /* Increment the pointer to next node. */
            curr_node = curr_node->nextp; 
        }
        printf("Successfully saved database! Please check file: %s", FILE_NAME);
    } else {
        printf("Write error\n"); /* Some error must have occured. */
    }

    /*free(curr_node); Not sure if we should do this.*/
    curr_node = NULL;

    fclose(filep); /* Close the file */
    filep = NULL;
};

/*******************************************************************************
* This function will load and decrypt the database file for use with the 
* program.
* author:
* - Arian Jahiri 13348469
* inputs:
* - none
* outputs:
* - none
*******************************************************************************/
void load_database()
{
    char master_password[30];
    printf("Please enter the master password to decrypt the database file> ");
    scanf("%s", master_password);

    FILE* filep;
    filep = fopen(FILE_NAME, "r"); /* Open in read mode */
    
    /* There may be some issue reading the file, if so let user know. */
    if (filep) {    
        char username[MAX_ENCODED_LEN+1];/*Encoded string can be 2x normal len*/
        char password[MAX_ENCODED_LEN+1];

        while (fscanf(filep, "%s %s", username, password) != EOF)
        {
            /* 
             * Decrypt the username and password entries using master_password.
             * Decryption must be done first before decoding as during saving, 
             * compression was done first.
             */
            decrypt(username, strlen(username), 
                    master_password,strlen(master_password));
            decrypt(password, strlen(password), 
                    master_password,strlen(master_password));

            char* decoded_username = decode(username, strlen(username));
            char* decoded_password = decode(password, strlen(password));
            
            /*DEBUG*/
            if (debug_mode == 1) {
                printf("DEBUG: Decrypted Entry: %s:%s\n", username, password);
                printf("DEBUG: Decoded Entry: %s:%s\n", 
                decoded_username, decoded_password);
            }
            
            /* Start adding the entry to the list */
            node_t* new_entry = (node_t*) malloc(1*sizeof(node_t));

            new_entry->data.key = element_counter;

            strcpy(new_entry->data.username, decoded_username);
            strcpy(new_entry->data.password, decoded_password);
            
            /*Add new entry to the head of the list*/
            new_entry->nextp = database_listp;
            database_listp = new_entry;
            
            element_counter++; /*Increment counter to maintain key integrity.*/
            new_entry = NULL; /*Avoid dangling pointer*/
        }

        fclose(filep); /* Close the file */
        printf("Successfully loaded database file!\n");

        database_menu(); /*Send to db menu if loading was successful.*/
    } else {
        printf("Read error\n"); /* Some error must have occured. */
    }
};


/*******************************************************************************
* This function will ask user to input a password of a specific size.
* author:
* - Arian Jahiri 13348469
* inputs:
* - pointer to entry_password that can be edited by this function.
* outputs:
* - none
*******************************************************************************/
void input_password(char* entry_password)
{
    char password_input[MAXPASSWORD_LENGTH+1];
    int valid = 0;
    while(valid==0)
    {
        printf("Password maximum length is 30 character.\n");
        printf("Enter the password that you want> ");
        /* scanf("%s", password); */
        getchar(); /*Eat \n*/
        fgets(password_input, 1000, stdin); /*Large buffer handles long input*/
        
        if(strlen(password_input) > MAXPASSWORD_LENGTH+1)
        {
            printf("Error: Password maximum length is 30 character.\n");
        } else {
            password_input[MAXPASSWORD_LENGTH] = '\0';
            /* Remove newline char from password due to fgets() */
            password_input[strcspn(password_input, "\n")] = 0;
            strcpy(entry_password, password_input);
            valid = 1;
        }
    }
};

/*******************************************************************************
* This function will generate a randomized password of a specific size.
* author:
* - Arian Jahiri 13348469
* inputs:
* - none
* outputs:
* - pointer to random password string generated by this function.
*******************************************************************************/
char* generate_password()
{   
    /*Initialize variables, p_size should never be 0*/
    int valid_size = 0; 
    int p_size = 1;

    while(valid_size != 1) {
        printf("Enter size of random password (max 30)> ");
        scanf("%d", &p_size);
        if (p_size > 30 || p_size < 0) {
            valid_size = 0;
            printf("Invalid input.\n");
        } else {
            valid_size = 1;
        }
    }

    /* 
     * C program requires a source of randomness to ensure subsequent passwords
     * are indeed random. This is usually done by seeding with the current time
     * from time.h but this was not allowed.
     */
    int random_seed = 1;
    int valid_seed = 0;

    while(valid_seed != 1) {
        printf( "\nTo generate a strong password, please enter a random  "
                "number as a source of randomness.\nThe longer the integer "
                "the better, no greater than 5 digits long (99999).\n");
        printf("Seed> ");
        scanf("%d", &random_seed);
        if (random_seed > 99999) {
            valid_seed = 0;
        } else {
            valid_seed = 1;
        }
    }
    
    srand(random_seed); /*Seed the randomness using input from user*/

    char* random_password = (char*) malloc((p_size+1) * sizeof(char));

    /*Possible to add special characters to charset.*/
    const char charset[] = 
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    /*Actual generator that picks random chars from charset*/
    if (p_size) {
        int n;
        for (n = 0; n < p_size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            random_password[n] = charset[key];
        }
        random_password[p_size] = '\0'; /*Terminate password string*/
    }

    /*Show result and return*/
    printf("Random password generated: %s\n", random_password);
    return random_password;
};
    

/*******************************************************************************
* This function will display the database data entries by looping through
* the linked list nodes.
* author:
* - Adrian Tang
* inputs: 
* - none
* outputs: 
* - none
*******************************************************************************/
void display_data()
{
    /*
     * Possible for list to be empty (in beginning), handle this case to avoid
     * errors.
     */
    if (database_listp == NULL) {
        printf("No entries to show, list is empty.\n");
        return;
    }

    if(debug_mode==1) {
        printf("DEBUG: HEAD=> U:%s P:%s\n", 
        database_listp->data.username, database_listp->data.password);
    }
    
    printf("Printing the linked list items.\n");

    /*Iterate through all nodes of the linked list and display entry info*/
    node_t* curr_node = database_listp;
    while(curr_node != NULL) {
        printf("Key:%d U:%s P:%s\n",
        curr_node->data.key,
        curr_node->data.username, 
        curr_node->data.password);
        curr_node = curr_node->nextp; /*Iterate the node pointer to next node*/
    }
}

/*******************************************************************************
* This function will remove an entry from the list according to the input
* key.
* - Arian Jahiri 13348469
* inputs:
* - none
* outputs:
* - none
*******************************************************************************/
void remove_entry()
{
    /* If list is empty do nothing. */
    if(database_listp == NULL) {
        printf("List is empty, can't delete.\n");
        return;
    }

    /*
     * Key of entries is displayed in display_data() function, it is a unique
     * identifier in order to id all nodes in the linked list.
     */
    int key_input;
    printf("Please enter the key of the entry you want to delete> ");
    scanf("%d", &key_input);

    /* Search through the linked list and find the element with that key. */
    node_t* current = database_listp;
    node_t* previous = NULL;

    /* Run through list until found, then current will 
     * be the node we need to "delete" and previous is the node before it. */
    while (current->data.key != key_input) {
        /* If current is the last node */
        if (current->nextp == NULL) {
            printf("Entry could not be found, please check the key and "
                    "try again!\n");
            return;
        } else {
            /* Track current and previous */
            previous = current;
            current = current->nextp;
        }
    }

    /* Delete the node and update the list */
    if (current == database_listp) {
        /* If element found is the head of the list just change the head. */
        database_listp = database_listp->nextp;
    } else {
        previous->nextp = current->nextp; /*Update node to skip the match*/
        free(current); /*Delete tha matched element*/
        current = NULL; /* Avoid dangling pointers*/
    }
};
