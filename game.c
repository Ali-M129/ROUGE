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
    int door[2];
    room_type type;
} room;



char map[4][MAP_ROWS][MAP_COLUMNS];
int rooms_number[4] = {0};
room rooms[4][MAX_ROOMS];

/*generate_window(int floor_number)
{
    for(int i=0; i,)
}

generate_pillars(int floor_number)
{
    for(int i=0; i<rooms_number[floor_number]; i++)
    {
        int area = (rooms[floor_number][i].columns - 2) * (rooms[floor_number][i].rows - 2);
        
    }
}*/

int over_door(int floor_number)
{
    int door_count = 0;
    for (int i = 0; i < MAP_COLUMNS; i++) {
        for (int j = 0; j < MAP_ROWS; j++) {
            if(map[floor_number][j][i] == '+')
            {
                door_count++;
            }
        }
    }
    if(door_count>rooms_number[floor_number]*3.6)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void connect_doors(int x1, int y1, int x2, int y2, int floor_number) 
{

    if (rand() % 2) 
    {
        while (x1 != x2) 
        {
            if ((map[floor_number][y1][x1] == '|' || map[floor_number][y1][x1] == '_')) 
            {
                map[floor_number][y1][x1] = '+';
            } 
            else if (map[floor_number][y1][x1] == ' ') 
            {
                map[floor_number][y1][x1] = '#';
            }
            x1 += (x2 > x1) ? 1 : -1;
        }

        while (y1 != y2) 
        {
            if ((map[floor_number][y1][x1] == '|' || map[floor_number][y1][x1] == '_')) 
            {
                map[floor_number][y1][x1] = '+';
            } 
            else if (map[floor_number][y1][x1] == ' ') 
            {
                map[floor_number][y1][x1] = '#';
            }
            y1 += (y2 > y1) ? 1 : -1;
        }
    } 
    else 
    {
        while (y1 != y2) 
        {
            if ( (map[floor_number][y1][x1] == '|' || map[floor_number][y1][x1] == '_')) 
            {
                map[floor_number][y1][x1] = '+';

            } 
            else if (map[floor_number][y1][x1] == ' ') 
            {
                map[floor_number][y1][x1] = '#';
            }
            y1 += (y2 > y1) ? 1 : -1;
        }

        while (x1 != x2) 
        {
            if ( (map[floor_number][y1][x1] == '|' || map[floor_number][y1][x1] == '_')) 
            {
                map[floor_number][y1][x1] = '+';

            } 
            else if (map[floor_number][y1][x1] == ' ') 
            {
                map[floor_number][y1][x1] = '#';
            }
            x1 += (x2 > x1) ? 1 : -1;
        }
    }
}

void generate_corridors(int floor_number)
{
    for(int i=2; i<rooms_number[floor_number]; i++)
    {
        connect_doors(rooms[floor_number][i-2].door[0], rooms[floor_number][i-2].door[1], rooms[floor_number][i].door[0], rooms[floor_number][i].door[1], floor_number);
    }
    for(int i=1; i<rooms_number[floor_number]; i++)
    {
        connect_doors(rooms[floor_number][i-1].door[0], rooms[floor_number][i-1].door[1], rooms[floor_number][i].door[0], rooms[floor_number][i].door[1], floor_number);
    }
    
}

void generate_doors(int floor_number, int room_index) {
    room *r = &rooms[floor_number][room_index];


        int dx, dy, valid_position = 0;

            int side = RANDOM(0, 3);
            dx = dy = 0;

            if (side == 0 && r->y > 0) 
            {
                dy = 1;
                dx = RANDOM(1, r->columns - 2);
            } 
            else if (side == 1 && r->y + r->rows < MAP_ROWS - 1) 
            {
                dy = r->rows-2 ;
                dx = RANDOM(1, r->columns - 2);

            } 
            else if (side == 2 && r->x > 0) 
            {
                dy = RANDOM(1, r->rows - 2);
                dx = 1;

            } 
            else if (side == 3 && r->x + r->columns < MAP_COLUMNS - 1) 
            {
                dy = RANDOM(1, r->rows - 2);
                dx = r->columns-2 ;
            }


            r->door[0] = r->x + dx;
            r->door[1] = r->y + dy;
            valid_position = 1;

}


void generate_rooms(int floor_number, int room_number) 
{
    if (room_number == 0 || rooms_number[floor_number] >= MAX_ROOMS) return;

    int attempts = 100;
    while (attempts--) {
        int room_x = RANDOM(1, MAP_COLUMNS - 15);
        int room_y = RANDOM(1, MAP_ROWS - 15);
        int room_rows = RANDOM(6, 13);
        int room_columns = RANDOM(6, 13);
        int valid = 1;

        for (int i = room_x - 1; i < room_x + room_columns + 1; i++) 
        {
            for (int j = room_y; j < room_y + room_rows + 1; j++) 
            {
                if (map[floor_number][j][i] != ' ') valid = 0;
            }
        }

        if (valid) 
        {
            for (int i = room_x; i < room_x + room_columns; i++) 
            {
                map[floor_number][room_y + room_rows - 1][i] = '_';
            }
            for (int i = room_y; i < room_y + room_rows; i++) 
            {
                map[floor_number][i][room_x] = '|';
                map[floor_number][i][room_x + room_columns - 1] = '|';
            }
            for (int i = room_x; i < room_x + room_columns; i++) 
            {
                map[floor_number][room_y][i] = '_';
            }
            for (int i = room_x + 1; i < room_x + room_columns - 1; i++) 
            {
                for (int j = room_y + 1; j < room_y + room_rows - 1; j++) 
                {
                    map[floor_number][j][i] = '.';
                }
            }

            int room_index = rooms_number[floor_number]++;
            room_type type;
            rooms[floor_number][room_index] = (room){room_x, room_y, room_rows, room_columns, {}, regular};

            generate_doors(floor_number, room_index);
            generate_rooms(floor_number, room_number - 1);
            return;
        }
    }
}


void generate_map(int floor_number, int room_number) 
{
    
    do
    {
        for (int j = 0; j < MAP_ROWS; j++)
            for (int k = 0; k < MAP_COLUMNS; k++)
                map[floor_number][j][k] = ' ';

        rooms_number[floor_number] = 0;
        generate_rooms(floor_number, room_number);
        generate_corridors(floor_number);
    } while(over_door(floor_number));
}

void show_map() 
{
    for (int i = 0; i < MAP_COLUMNS; i++) 
    {
        for (int j = 0; j < MAP_ROWS; j++) 
        {
            mvprintw(4 + j, 7 + i, "%c", map[0][j][i]);
        }
    }
    refresh();
}

void print_message(char *message) 
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
    show_map();

    while (1) 
    {
        int input = getch();
        if (input == 'm') 
        {
            generate_map(0, 6);
            show_map();
        }
    }
}
