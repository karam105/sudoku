#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

int validate_row(Board* board,int y)
{
	int output = 0;
	for(int x = 0; x < BOARD_WIDTH; x++)
	{
		int value = get_entry(board,x,y);
		output += value * value;
	}
	if(output == 285)
		return -1;
	return y;
}
int validate_column(Board* board,int x)
{

	int output = 0;
	for(int y = 0; y < BOARD_HEIGHT; y++)
	{
		int value = get_entry(board,x,y);
		output += value * value;

	}
	if(output == 285)
		return -1;
	return x;
}
int validate_square(Board* board,int x,int y)
{
	int xt = x*3;
	int yt = y*3;

	int output = 0;

	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			int value = get_entry(board,x + xt,y + yt);
			output += value * value;
		}
	}

	if(output == 285)
		return -1;
	return y * BOARD_WIDTH + x;
}

int get_entry(Board* board, int x, int y)
{

	return board->payload[y * BOARD_WIDTH + x];
}

Board* load_board(const char* file)
{
	int size = 0;
	FILE *f = fopen(file,"rb");

	if(!f)
		return NULL;


	fseek(f,0,SEEK_END);
	size = ftell(f);
	fseek(f,0,SEEK_SET);
	char* buffer = malloc(size);
	fread(buffer,1,size,f);


	int input[BOARD_ENTIRES];
	int entires = -1;

	int var;
	char* token = strtok (buffer, ",");
	while (token != NULL)
	{
		if(entires >= BOARD_ENTIRES)
			return NULL;
		sscanf (token, "%d", &var);
		input[++entires] = var;
		//printf("%d\n",var );

		token = strtok (NULL, ",");
	}
	free(buffer);

	Board* b = (Board*)malloc(sizeof(Board));
	b->payload = malloc(sizeof(int) * BOARD_ENTIRES);
	memcpy(b->payload,input,BOARD_ENTIRES * sizeof(int));
	return b;
}

void free_board(Board* board)
{
	free(board->payload);
	free(board);

}

void* row_thread(void *args) {
	RowArgs* row = (RowArgs*)args;
	int output = validate_row(row->board,row->row);
	free(args);
	pthread_exit((void*)(intptr_t)output);
}
void* column_thread(void *args) {
	RowArgs* row = (RowArgs*)args;
	int output = validate_column(row->board,row->row);
	free(args);
	pthread_exit((void*)(intptr_t)output);
}
void* quad_thread(void *args) {
	QuadArgs* quad = (QuadArgs*)args;
	int output = validate_square(quad->board,quad->x,quad->y);
	free(args);
	pthread_exit((void*)(intptr_t)output);
}
