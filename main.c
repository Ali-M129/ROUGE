#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include "sign_up.c"
#include "login.c"

int signup_login_menu()
{
    initscr();
    clear();              
    cbreak();                 
    noecho();                 
    keypad(stdscr, TRUE);     
    curs_set(0);              

    // Initialize variables
    int choice = 0;
    const char *options[] = {"Login", "Sign Up", "Login as Guest"};
    const int num_options = sizeof(options) / sizeof(options[0]);

    while (1) {
        clear();
        for (int i = 0; i < num_options; i++) {
            if (i == choice) {
                attron(A_REVERSE); 
            }
            mvprintw(5 + i, 10, options[i]); 
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }

        int input = getch(); 


        if (input == 'w' || input == KEY_UP || input == 'W') 
        {
            choice = (choice - 1 + num_options) % num_options; // Move up
        }
         else if (input == 's' || input == KEY_DOWN || input == input == 'S') 
        {
            choice = (choice + 1) % num_options; // Move down
        } 
        else if (input == '\n') 
        {
            return choice;
            refresh();
            getch();
            break;
        }
    }
    endwin(); 
}
int main() 
{
    int choise = signup_login_menu();
    if(choise == 0)
    {
        login_page();
    }
    else if(choise == 1)
    {
        sign_up_page();
        main();
    }
    else if(choise == 2)
    {
        
    }
    return 0;
}
