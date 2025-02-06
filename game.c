#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define MAP_ROWS 38
#define MAP_COLUMNS 144
#define MAX_ROOMS 12
#define RANDOM(min, max) ((rand() % ((max) - (min) + 1)) + (min))

typedef enum { regular, enchant, treasure, nightmare } room_type;

typedef struct {
    int x, y, rows, columns;
    int doors[2][2];
    int door_count;
    room_type type;
} room;

char map[4][MAP_ROWS][MAP_COLUMNS];
int rooms_number[4] = {0};
room rooms[4][MAX_ROOMS];


void generate_doors(int floor_number, int room_index) {
    room *r = &rooms[floor_number][room_index];
    r->door_count = RANDOM(1, 2); 

    for (int i = 0; i < r->door_count; i++) {
        int dx, dy, valid_position = 0;

        while (!valid_position) {
            int side = RANDOM(0, 3);
            dx = dy = 0;

            if (side == 0 && r->y > 0) {
                dy = 0;
                dx = RANDOM(1, r->columns - 2);
            } else if (side == 1 && r->y + r->rows < MAP_ROWS - 1) {
                dy = r->rows - 1;
                dx = RANDOM(1, r->columns - 2);
            } else if (side == 2 && r->x > 0) {
                dy = RANDOM(1, r->rows - 2);
                dx = 0;
            } else if (side == 3 && r->x + r->columns < MAP_COLUMNS - 1) {
                dy = RANDOM(1, r->rows - 2);
                dx = r->columns - 1;
            }

            if (i == 1) {
                int prev_x = r->doors[0][0];
                int prev_y = r->doors[0][1];
                if (abs((r->x + dx) - prev_x) <= 1 && abs((r->y + dy) - prev_y) <= 1) {
                    continue;
                }
            }

            r->doors[i][0] = r->x + dx;
            r->doors[i][1] = r->y + dy;
            map[floor_number][r->y + dy][r->x + dx] = '+';
            valid_position = 1;
        }
    }
}


void generate_rooms(int floor_number, int room_number) {
    if (room_number == 0 || rooms_number[floor_number] >= MAX_ROOMS) return;

    int attempts = 100;
    while (attempts--) {
        int room_x = RANDOM(1, MAP_COLUMNS - 15);
        int room_y = RANDOM(1, MAP_ROWS - 15);
        int room_rows = RANDOM(6, 13);
        int room_columns = RANDOM(6, 13);
        int valid = 1;

        for (int i = room_x - 1; i < room_x + room_columns + 1; i++) {
            for (int j = room_y; j < room_y + room_rows + 1; j++) {
                if (map[floor_number][j][i] != ' ') valid = 0;
            }
        }

        if (valid) {
            for (int i = room_x; i < room_x + room_columns; i++) {
                map[floor_number][room_y + room_rows - 1][i] = '_';
            }
            for (int i = room_y; i < room_y + room_rows; i++) {
                map[floor_number][i][room_x] = '|';
                map[floor_number][i][room_x + room_columns - 1] = '|';
            }
            for (int i = room_x; i < room_x + room_columns; i++) {
                map[floor_number][room_y][i] = '_';
            }
            for (int i = room_x + 1; i < room_x + room_columns - 1; i++) {
                for (int j = room_y + 1; j < room_y + room_rows - 1; j++) {
                    map[floor_number][j][i] = '.';
                }
            }

            int room_index = rooms_number[floor_number]++;
            rooms[floor_number][room_index] = (room){room_x, room_y, room_rows, room_columns, {}, 0, regular};

            generate_doors(floor_number, room_index);
            generate_rooms(floor_number, room_number - 1);
            return;
        }
    }
}

// تابع برای ساخت مپ
void generate_map(int floor_number, int room_number) {
    for (int j = 0; j < MAP_ROWS; j++)
        for (int k = 0; k < MAP_COLUMNS; k++)
            map[floor_number][j][k] = ' ';

    rooms_number[floor_number] = 0;
    generate_rooms(floor_number, room_number);
}

// نمایش مپ در ncurses
void show_map() {
    for (int i = 0; i < MAP_COLUMNS; i++) {
        for (int j = 0; j < MAP_ROWS; j++) {
            mvprintw(4 + j, 7 + i, "%c", map[0][j][i]);
        }
    }
    refresh();
}

// نمایش پیام در بالای صفحه
void print_message(char *message) {
    move(1, 7);
    clrtoeol();
    mvprintw(1, 7, "%s", message);
    refresh();
}

int main() {
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    generate_map(0, 6);
    show_map();

    while (1) {
        int input = getch();
        if (input == 'm') {
            generate_map(0, 6);
            show_map();
        }
    }
}
