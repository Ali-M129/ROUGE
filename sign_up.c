#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>

#define MAX_LEN 100

int is_valid_email(const char* email) 
{
    regex_t regex;
    const char *pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    int result = !regcomp(&regex, pattern, REG_EXTENDED) && !regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    return result;
}


int is_this_user_exist(const char* username) 
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) 
        return 0;
    char line[MAX_LEN + 2];
    char stored_username[MAX_LEN + 2];

    while (fgets(line, sizeof(line), file) != NULL) 
    {    
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
    return 0; 
}

int is_password_strong(const char* password)
{
    int number = 0;
    int upper_case = 0;
    int lower_case = 0;
    int long_enogh = 0;
    if(strlen(password) > 6)
        long_enogh = 1;
    for(int i=0; i<strlen(password); i++)
    {
        if('A' <= password[i] && password[i] <= 'Z')
            upper_case = 1;
        if('a' <= password[i] && password[i] <= 'z')
            lower_case = 1;
        if('0' <= password[i] && password[i] <= '9')
            number = 1;
    }
    if((number + upper_case + lower_case + long_enogh) == 4)
        return 1;
    else
        return 0;

}


void save_user(const char* username, const char* password, const char* email) 
{
    FILE *file = fopen("users.txt", "a");
    if (file != NULL) {
        fprintf(file, "Username: %s, Password: %s, email: %s, High_score: %d.\n", username, password, email, 0);
        fclose(file);
    }
}

void sign_up_page() 
{
    clear();
    cbreak();                 // Disable line buffering
    noecho();                 // Disable echoing of characters
    keypad(stdscr, TRUE);     // Enable special keys like arrow keys
    curs_set(1);              // Show the cursor


    char username[MAX_LEN + 2] = {0};
    char password[MAX_LEN + 2] = {0};
    char email[MAX_LEN + 2] = {0};
    char confirm_password[MAX_LEN + 2] = {0};
    int index = 0;
    int ch;

    
    while(1) 
    {
        mvprintw(5, 0, "Enter username: ");
        move(6, index);
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
            break; 
        }
    }


    clear();
    index = 0;
    while(1) 
    {
        mvprintw(5, 0, "Enter email: ");
        move(6, index);
        refresh();
        while ((ch = getch()) != '\n' && index < MAX_LEN - 1) 
        {
            if (ch == KEY_BACKSPACE || ch == 127) 
            {
                if (index > 0) {
                    index--;
                    email[index] = '\0';
                    move(6, 0); 
                    clrtoeol(); 
                    mvprintw(6, 0, "%s", email);  
                    refresh();
                }
            } 
            else if (ch >= 32 && ch <= 126) 
            {
                email[index++] = ch;
                email[index] = '\0'; 
                mvprintw(6, 0, "%s", email);
                refresh();
            }
        }
        email[index] = '\0';

        if (is_valid_email(email) == 0) 
        {
            mvprintw(7, 0, "This email is not valid! Try again."); 
            refresh();
        } 
        else 
        {
            break;
        }
    }
    
    

    clear();
    index = 0;
    while(1) 
    {
        mvprintw(5, 0, "Enter password: ");
        move(6, index);
        refresh();
        while ((ch = getch()) != '\n' && index < MAX_LEN - 1) 
        {
            if (ch == KEY_BACKSPACE || ch == 127) 
            {
                if (index > 0) {
                    index--;
                    password[index] = '\0';
                    move(6, 0); 
                    clrtoeol(); 
                    mvprintw(6, 0, "%s", password);  
                    refresh();
                }
            } 
            else if (ch >= 32 && ch <= 126) 
            {
                password[index++] = ch;
                password[index] = '\0'; 
                mvprintw(6, 0, "%s", password);
                refresh();
            }
        }
        password[index] = '\0';

        if (is_password_strong(password) == 0) 
        {
            mvprintw(7, 0, "This password is not strong! Try again."); 
            refresh();
        } 
        else 
        {
            break;
        }
    }

    clear();
    save_user(username, password, email);
    mvprintw(5, 0, "Account created successfully!\npress any key to countinue");
    refresh();

    getch();

}
