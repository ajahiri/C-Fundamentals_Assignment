/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Names: Arian Jahiri
 * Student IDs: 13348469
*******************************************************************************/
#include "string.h"
#include "userpass_database.c"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        main_menu(0); /*Start program without debug mode.*/
    } else if (argc == 2) {
        if(strcmp(argv[1], "-debug")==0) main_menu(1);
    } else {
        printf("Invalid mode identified.\n");
    }
    return 0;
}
