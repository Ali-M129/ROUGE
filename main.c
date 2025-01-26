#include <ncurses.h>
int signup_login_menu()
{
    initscr();                
    cbreak();                 
    noecho();                 
    keypad(stdscr, TRUE);     
    curs_set(0);              

    // Initialize variables
    int choice = 0;
    const char *options[] = {"Login", "Sign Up"};
    const int num_options = sizeof(options) / sizeof(options[0]);

    while (1) {
        //clear();

        // Print menu options
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

        // Handle user input
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
    ifsignup_login_menu();
    return 0;
}
