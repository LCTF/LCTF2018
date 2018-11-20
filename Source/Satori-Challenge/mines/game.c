#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "game.h"
#include "HsFFI.h"
#include "Get_stub.h"

Map *init_game(int width, int height, int boom_count){
    int booms = boom_count;
    int blocks = width * height;

    // check inf-loop
    if(booms > blocks){
        // This is right setting, I think...
        booms = blocks - 1;
    }

    Map *map = malloc(sizeof(Map));
    Block *block = malloc(sizeof(Block) * width * height);

    if(map == NULL || block == NULL){
        perror("Init Map failed");
        exit(-1);
    }

    map->x_cursor = 0;
    map->y_cursor = 0;
    map->width = width;
    map->height = height;
    map->data = block;

    // init all block status
    for(int i = 0; i < blocks; ++i){
        block[i].status = s_cover;
        block[i].is_boom = false;
    }

    // set boom
    int index;
    srand((unsigned int)time(NULL));

    // b = rand() % (blocks - 1 + 1 - 0) + 0
    // rand() % (max_number + 1 - minimum_number) + minimum_number
    while(booms){
        index = rand() % blocks;
        if(block[index].is_boom != true){
            block[index].is_boom = true;
            booms = booms - 1;
        }
    }

    return map;
}


Block *get_block(Map *map, int x, int y){
    return &map->data[map->width * y + x];
}

#define inc_int(d, max) { d = d == max ? d : d + 1; }
#define dec_int(d, min) { d = d == min ? d : d - 1; }

void move_curosr(Map *map, Keyboard key){
    switch(key){
    case k_up:
        dec_int(map->y_cursor, 0);
        return;
    case k_down:
        inc_int(map->y_cursor, map->height - 1);
        return;
    case k_left:
        dec_int(map->x_cursor, 0);
        return;
    case k_right:
        inc_int(map->x_cursor, map->width - 1);
        return;
    default:
        return;
    }
}


bool open_block(Map *map){
    Block *b = get_block(map, map->x_cursor, map->y_cursor);
    
    if(b->status == s_cover){
        if(b->is_boom){
            // find boom
            b->status = s_boom;
            return true;
        }
        else{
            // normal block
            b->status = s_open;
            return false;
        }
    }
    else{
        // This block is opened
        return false;
    }
}

bool is_win(Map *map){
    int count = map->width * map->height;

    for(int i = 0; i < count; ++i){
        if( map->data[i].status == s_cover &&
            map->data[i].is_boom == false)
        {
            return false;
        }
    }

    return true;
}

void clear(){
    system("CLS");
}

Keyboard get_key(){
    int key = getch();

    switch(key){
    case UP:
        return k_up;
    case DOWN:
        return k_down;
    case LEFT:
        return k_left;
    case RIGHT:
        return k_right;
    case SCAN:
        return k_scan;
    case QUIT:
        return k_quit;
    default:
        return k_unknown;
    }
}

void draw_map(Map *map, bool drawCurosr){
    Block *b;

    puts("[>_<] Mines - Lunatic Mode");
    puts("\n--- --- ---\n");

    for(int y = 0; y < map->height; ++y){
        for(int x = 0; x < map->width; ++x){
            if( drawCurosr &&
                x == map->x_cursor &&
                y == map->y_cursor )
            {
                printf(" # ");
            }
            else{
                b = get_block(map, x, y);
                switch(b->status){
                case s_cover:
                    printf(" * "); break;
                case s_open:
                    printf(" O "); break;
                case s_boom:
                    printf(" X "); break;
                }
            }
        }
        puts("");
    }

    puts("\n--- --- ---\n");
    puts("Use wasd to move, space to scan block, q to exit");
}

void game_win(){
    // show flag
    puts("\n---\n");
    puts("You win");
    showflag();
}

void game_over(){
    puts("\n---\n");
    puts("Game over");
    puts("Still play mines, you will fail");
}

#define DEFAULT_WIDTH  10
#define DEFAULT_HEIGHT 10
#define DEFAULT_BOOM   50

void game(){
    Keyboard key;
    bool result;

    Map *map = init_game(
        DEFAULT_WIDTH,
        DEFAULT_HEIGHT,
        DEFAULT_BOOM
    );

    while(1){
        clear();
        draw_map(map, true);
        key = get_key();

        if(key == k_quit){
            break;
        }
        else if(key == k_unknown){
            continue;
        }
        else if(key == k_scan){
            result = open_block(map);
            if(result == true){
                clear();
                draw_map(map, false);
                game_over();
                break;
            }
            if( result == false &&
                is_win(map))
            {
                clear();
                draw_map(map, false);
                game_win();
                break;
            }
        }
        else{
            // key == move key
            move_curosr(map, key);
        }
    }
}

int main(int argc, char *argv[]){
    hs_init(&argc, &argv);

    game();

    return 0;
}
