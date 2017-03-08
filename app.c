#include "./../src/board.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
		return -1;
	Board* board = load_board(argv[1]);//"./testfile.txt");
	pthread_t tids[BOARD_WIDTH + BOARD_HEIGHT + 9];

	int index = -1;
	for(int x = 0; x < BOARD_WIDTH; x++)
	{
		RowArgs *args = (RowArgs *)malloc(sizeof(RowArgs));
		args->board = board;
		args->row = x;
		pthread_create(&tids[++index], NULL, row_thread, args);
	}

	for(int x = 0; x < BOARD_HEIGHT; x++)
	{
		RowArgs *args = (RowArgs *)malloc(sizeof(RowArgs));
		args->board = board;
		args->row = x;
		pthread_create(&tids[++index], NULL, column_thread, args);
	}

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			QuadArgs *args = (QuadArgs *)malloc(sizeof(QuadArgs));
			args->board = board;
			args->x = x;
			args->y = y;
			pthread_create(&tids[++index], NULL, quad_thread, args);
		}
	}
	bool has_displayed_error = false;
	for(int j = 0; j < BOARD_WIDTH + BOARD_HEIGHT + 9;++j)
	{
		void* ptr;
		pthread_join( tids[j],  &ptr);

		int returnValue = (intptr_t)ptr;
		if(returnValue != -1)
		{
			if(!has_displayed_error)
			{
				printf("%s\n","the board is invalid");
				has_displayed_error = true;
			}
			if(j < BOARD_WIDTH)
			{
				printf("invalid row:%d\n",returnValue+1);

			}
			else if(j < BOARD_WIDTH + BOARD_HEIGHT)
			{

				printf("invalid column:%d\n",returnValue+1);
			}
			else
			{
				printf("invalid quad:(%d,%d)\n",(returnValue%BOARD_WIDTH)+1,(returnValue/BOARD_WIDTH)+1);
			}
		}

	}
	if(!has_displayed_error)
		printf("%s\n","the board is valid");

	free_board(board);
	return(0);
}
