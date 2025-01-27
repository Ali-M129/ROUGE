#include <ncurses.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 100


int is_this_user_exist(char username[MAX_LEN + 2]) 
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) 
        return 0;
    char line[MAX_LEN + 2];
    char stored_username[MAX_LEN + 2];

    while (fgets(line, sizeof(line), file) != NULL) {
        
        if (sscanf(line, "Username: %s,", stored_username) == 1) 
        {
            stored_username[strlen(stored_username)-1] = '\0';
            if (strcmp(username, stored_username) == 0) 
            {
                fclose(file);
                return 1; 
            }
        }
    }

    fclose(file);
    return 0; // یوزرنیم پیدا نشد
}


void save_user(const char *username, const char *password) 
{
    FILE *file = fopen("users.txt", "a");
    if (file != NULL) {
        fprintf(file, "Username: %s, Password: %s, High_score: %d.\n", username, password, 0);
        fclose(file);
    }
}

int main() {
    initscr();                // Start ncurses mode
    cbreak();                 // Disable line buffering
    noecho();                 // Disable echoing of characters
    keypad(stdscr, TRUE);     // Enable special keys like arrow keys
    curs_set(1);              // Show the cursor


    char username[MAX_LEN + 2] = {0};
    char password[MAX_LEN + 2] = {0};
    char confirm_password[MAX_LEN + 2] = {0};
    int index = 0;
    int ch;

    
    while(1) 
    {
        mvprintw(5, 0, "Enter username: ");
        refresh();
        while ((ch = getch()) != '\n' && index < MAX_LEN - 1) 
        {
            if (ch == KEY_BACKSPACE || ch == 127) 
            {
                if (index > 0) {
                    index--;
                    username[index] = '\0'; // آخرین کاراکتر را حذف کن
                    move(6, 0); // مکان‌نمای خط یوزرنیم
                    clrtoeol(); // پاک کردن خط فعلی
                    mvprintw(6, 0, "%s", username); // نمایش مجدد یوزرنیم
                    refresh();
                }
            } 
            else if (ch >= 32 && ch <= 126) 
            {
                username[index++] = ch;
                username[index] = '\0'; 
                mvprintw(6, 0, "%s", username);
                refresh();
            }
        }
        username[index] = '\0';

        if (is_this_user_exist(username) == 1) 
        {
            mvprintw(7, 0, "This username already exists! Try again."); // پیغام خطا
            refresh();
        } 
        else 
        {
            break; // یوزرنیم معتبر است
        }
    }



    index = 0;
    //mvprintw(7, 0, "%s", username);
    refresh();
    while ((ch = getch()) != '\n' && index < MAX_LEN - 1) 
    {
        password[index++] = ch;
        mvprintw(8, 0, "%s", password);
        refresh();
    }
    password[index] = '\0'; // Null-terminate the password

    index = 0;
    mvprintw(9, 0, "Confirm password: ");
    refresh();
    while ((ch = getch()) != '\n' && index < MAX_LEN - 1) 
    {
        confirm_password[index++] = ch;
        mvprintw(10, 0, "%s", confirm_password);
        refresh();
    }
    confirm_password[index] = '\0'; // Null-terminate the confirm password

    if (strcmp(password, confirm_password) == 0) {
        save_user(username, password);
        mvprintw(12, 10, "Account created successfully!");
    } else {
        mvprintw(12, 10, "Passwords do not match!");
    }
    refresh();

    getch(); // Wait for a key press
    endwin(); // End ncurses mode

    return 0;
}
