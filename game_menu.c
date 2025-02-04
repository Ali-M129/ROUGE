#include <stdio.h>
#include <string.h>
#include <ncurses.h>

int main()
{
    initscr();              
    cbreak();                 
    noecho();                 
    keypad(stdscr, TRUE);     
    curs_set(0);              

    // Initialize variables
    int choice = 0;
    const char *options[] = {"New game", "Load game", "switch account", "score board", "log out"};
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