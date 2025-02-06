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
    room_type type;
} room;

char map[4][MAP_ROWS][MAP_COLOMNES];
int rooms_number[4] = {0};
room rooms[4][12];

void generate_rooms(int floor_number, int room_number)
{
    if(room_number != 0)
    {
        int room_y = RANDOM(1, MAP_ROWS - 14);
        int room_x = RANDOM(1, MAP_COLOMNES - 14);
        
        int room_rows = RANDOM(6, 13);
        int room_colomnes = RANDOM(6, 13);

        int f = 1;
        for(int i=room_x-1; i<room_x + room_colomnes +1; i++)
        {
            for(int j=room_y; j<room_y + room_rows + 1; j++)
            {
                if(map[floor_number][j][i] != ' ')
                    f = 0;
            }
        }
        if(f == 1)
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


            generate_rooms(floor_number, (room_number-1));
        }
        else
        {
            make_rooms(floor_number, room_number);
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
    make_rooms(floor_number, room_number);
    
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
        if(input = 'm')
        {
            generate_map(0, 6);
            Show_map();
        }
    }
    
}