#ifndef _GAME
#define _GAME

typedef int bool;
#define true 1
#define false 0

typedef enum{
    s_cover, s_open, s_boom
} Status;

typedef struct _block{
    Status status;
    bool is_boom;
} Block;

typedef struct{
    int width;
    int height;
    int x_cursor;
    int y_cursor;
    Block *data;
} Map;

typedef enum{
    k_up, k_down, k_left, k_right, k_scan, k_quit, k_unknown
} Keyboard;

#define UP 'w'      // w
#define DOWN 's'    // s
#define LEFT 'a'    // a
#define RIGHT 'd'   // d
#define SCAN ' '    // space
#define QUIT 'q'    // q

void game();

void clear();
Keyboard get_key();
void draw_map(Map *map, bool drawCurosr);

void game_win();
void game_over();

Map *init_game(int width, int height, int boom_count);
bool is_win(Map *map);
bool open_block(Map *map);
Block *get_block(Map *map, int x, int y);
void move_curosr(Map *map, Keyboard key);

#endif /* _GAME */
