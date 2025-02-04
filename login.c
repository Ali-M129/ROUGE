#include <ncurses.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 100

int login(const char* username, const char* password)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) 
        return -1;
    char line[MAX_LEN + 2];
    char stored_username[MAX_LEN + 2];
    char stored_password[MAX_LEN + 2];

    while (fgets(line, sizeof(line), file) != NULL) 
    {    
        if (sscanf(line, "Username: %[^,], Password: %[^,],", stored_username, stored_password) == 2) 
        {
            if (strcmp(username, stored_username) == 0) 
            {
                fclose(file);
                if(strcmp(password, stored_password) == 0)
                {
                    return 1; 
                }
                return 0;
            }
        }
    }
    return -1;
}
int login_page() {
    clear();
    cbreak();               // Disable line buffering
    noecho();               // Disable echoing of characters
    keypad(stdscr, TRUE);   // Enable special keys like arrow keys
    curs_set(0);            // Hide cursor

    // Variables for user input
    char username[MAX_LEN] = {0};
    char password[MAX_LEN] = {0};
    int current_field = 0;  // 0: username, 1: password, 2: login button
    int username_index = 0; // Separate index for username
    int password_index = 0; // Separate index for password
    int ch;
    int login_status = -2;

    // Main loop
    while (1) {

        // Display the form
        clear();
        mvprintw(5, 10, "Login Page");
        mvprintw(7, 10, "Username: ");
        mvprintw(9, 10, "Password: ");
        mvprintw(11, 10, "[ Login ]");
        if(login_status == -1)
        {
            mvprintw(2, 10, "Username not found!");
        }
        else if(login_status == 0)
        {
            mvprintw(2, 10, "password is inccorect");
        }
        else if(login_status == 1)
        {
            mvprintw(2, 10, "login successfuly!");
        }

        if (current_field == 0) 
        {
            mvprintw(7, 20, "%s_", username);
        } 
        else 
        {
            mvprintw(7, 20, "%s", username);
        }

        if (current_field == 1) 
        {
            mvprintw(9, 20, "%s_", password);
        } 
        else 
        {
            mvprintw(9, 20, "%s", password);
        }

        if (current_field == 2) 
        {
            attron(A_REVERSE);
            mvprintw(11, 10, "[ Login ]");
            attroff(A_REVERSE);
        }

        refresh();

        ch = getch();
        switch (ch) 
        {
            case KEY_UP: 
                current_field = (current_field - 1 + 3) % 3;
                break;

            case KEY_DOWN: // Move down
                current_field = (current_field + 1) % 3;
                break;

            case '\n':
                if (current_field == 2 || current_field == 1) 
                {
                    login_status = login(username, password);
                }
                else if(current_field == 0)
                {
                    current_field = 1;
                }
                break;

            default:
                if (current_field == 0 && username_index < MAX_LEN - 1) 
                {
                    if (ch == KEY_BACKSPACE || ch == 127) 
                    {
                        if (username_index > 0) 
                            username[--username_index] = '\0';
                    }
                    else 
                    {
                        username[username_index++] = ch;
                        username[username_index] = '\0';
                    }
                }
                else if (current_field == 1 && password_index < MAX_LEN - 1) 
                {
                    if (ch == KEY_BACKSPACE || ch == 127) 
                    {
                        if (password_index > 0) 
                            password[--password_index] = '\0';
                    }
                    else 
                    {
                        password[password_index++] = ch;
                        password[password_index] = '\0';
                    }
                }
                break;
        }
    }
    return 0;
}
