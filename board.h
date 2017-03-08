#ifndef _BOARD_H
#define _BOARD_H

#include <stdbool.h>
#include <stddef.h>

#define BOARD_ENTIRES 81
#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9

typedef struct _board
{
	int* payload;
} Board;

typedef struct _row_args {
    Board* board;
    int row;
} RowArgs;

typedef struct _quad_args {
    Board* board;
    int x;
    int y;
} QuadArgs;


void* row_thread(void *args);
void* column_thread(void *args);
void* quad_thread(void *args);


int validate_row(Board* board,int x);
int validate_column(Board* board,int x);
int validate_square(Board* board,int x,int y);

int get_entry(Board* board, int x, int y);

Board* load_board(const char* file);

void free_board(Board* board);

#endif
