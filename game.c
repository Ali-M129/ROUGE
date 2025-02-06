#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define MAP_ROWS 38
#define MAP_COLOMNES 144
#define RANDOM(min, max) ((rand() % ((max) - (min) + 1)) + (min))

typedef enum
{
  regular,
  enchant,
  treasure,
  nightmare 
} room_type;

typedef struct
{
    int x;
    int y;
    int rows;
    int colomnes;
    int doors[2][2]; // حداکثر 2 در
    int door_count;
    room_type type;
} room;

char map[4][MAP_ROWS][MAP_COLOMNES];
int rooms_number[4] = {0};
room rooms[4][12];

// تابع برای اضافه کردن در به اتاق
void generate_doors(int floor_number, int room_index) {
    room *r = &rooms[floor_number][room_index];
    r->door_count = RANDOM(1, 2); // تصادفی بین 1 یا 2 در

    for (int i = 0; i < r->door_count; i++) {
        int side, dx = 0, dy = 0;
        int valid_position = 0;
        
        // تلاش برای پیدا کردن موقعیت معتبر برای در
        while (!valid_position) {
            side = RANDOM(0, 3); // انتخاب یک طرف برای در
            
            // جلوگیری از قرار گرفتن در در گوشه‌ها و بررسی محدوده‌ها
            if (side == 0 && r->y > 0) { // بالا
                dy = 0;
                dx = RANDOM(1, r->colomnes - 2);
                valid_position = 1;
            }
            else if (side == 1 && r->y + r->rows < MAP_ROWS - 1) { // پایین
                dy = r->rows - 1;
                dx = RANDOM(1, r->colomnes - 2);
                valid_position = 1;
            }
            else if (side == 2 && r->x > 0) { // چپ
                dy = RANDOM(1, r->rows - 2);
                dx = 0;
                valid_position = 1;
            }
            else if (side == 3 && r->x + r->colomnes < MAP_COLOMNES - 1) { // راست
                dy = RANDOM(1, r->rows - 2);
                dx = r->colomnes - 1;
                valid_position = 1;
            }

            // بررسی اینکه دو در کنار هم نباشند
            if (i == 1) {
                int prev_x = r->doors[0][0];
                int prev_y = r->doors[0][1];
                if (abs((r->x + dx) - prev_x) <= 1 && abs((r->y + dy) - prev_y) <= 1) {
                    valid_position = 0; // موقعیت نامعتبر، دوباره امتحان کن
                }
            }
        }

        // قرار دادن در در موقعیت انتخاب شده
        r->doors[i][0] = r->x + dx;
        r->doors[i][1] = r->y + dy;
        map[floor_number][r->y + dy][r->x + dx] = '+';
    }
}

// تابع برای تولید اتاق‌ها
void generate_rooms(int floor_number, int room_number)
{
    if(room_number != 0)
    {
        int room_y = RANDOM(1, MAP_ROWS - 15);
        int room_x = RANDOM(1, MAP_COLOMNES - 15);
        
        int room_rows = RANDOM(6, 13);
        int room_colomnes = RANDOM(6, 13);

        int valid = 1;
        for(int i=room_x-1; i<room_x + room_colomnes +1; i++)
        {
            for(int j=room_y; j<room_y + room_rows + 1; j++)
            {
                if(map[floor_number][j][i] != ' ')
                    valid = 0;
            }
        }
        if(valid)
        {
            for(int i=room_x; i<room_x + room_colomnes; i++)
            {
                map[floor_number][room_y+room_rows-1][i] = '_';
            }
            for(int i=room_y; i<room_y + room_rows; i++)
            {
                map[floor_number][i][room_x] = '|';
                map[floor_number][i][room_x + room_colomnes-1] = '|';
            }
            for(int i=room_x; i<room_x + room_colomnes; i++)
            {
                map[floor_number][room_y][i] = '_';
            }
            for(int i=room_x+1; i<room_x + room_colomnes-1; i++)
            {
                for(int j=room_y+1; j<room_y + room_rows -1; j++)
                {
                    map[floor_number][j][i] = '.';
                }
            }

            int room_index = rooms_number[floor_number]++;

            rooms[floor_number][room_index].colomnes = room_colomnes;
            rooms[floor_number][room_index].rows = room_rows;
            rooms[floor_number][room_index].x = room_x;
            rooms[floor_number][room_index].y = room_y;
            rooms[floor_number][room_index].type = regular;

            generate_doors(floor_number, room_index); // ایجاد درها برای اتاق

            generate_rooms(floor_number, (room_number - 1));
        }
        else
        {
            generate_rooms(floor_number, room_number);
        }
    }
}

void generate_map(int floor_number, int room_number)
{
    for(int j=0; j<MAP_ROWS; j++)
    {
        for(int k=0; k<MAP_COLOMNES; k++)
        {
            map[floor_number][j][k] = ' ';
        }
    }
    generate_rooms(floor_number, room_number);
}

void Show_map()
{
    for(int i=0; i<MAP_COLOMNES; i++)
    {
        for(int j=0; j<MAP_ROWS; j++)
        {
            mvprintw(4+j, 7+i, "%c", map[0][j][i]);
        }
    }
    refresh();
}

void print(char* message)
{
    move(1, 7);
    clrtoeol();
    mvprintw(1, 7, "%s", message);
    refresh(); 
}

int main()
{
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    generate_map(0, 6);
    while (1)
    {
        int input = getch();
        if(input == 'm')
        {
            generate_map(0, 6);
            Show_map();
        }
    }
}
