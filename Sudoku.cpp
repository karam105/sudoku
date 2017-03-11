#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <pthread.h>

using namespace std;

int *board[9]; //initialize board
int row, col; //initialize variables
void *is_row_ok(void *param); //initialize functions
void *is_col_ok(void *param);
void *is_square_ok(void *param);

int main()
{
	for (int i = 0; i < 9; ++i)
	{
		board[i] = new int[9]; //creates the 2d array
	}

	string line;
	ifstream myFile("Testfile4.txt");  //reads in the file

	for (int row = 0; row < 9; ++row) //reads file line by line
	{
		string line;
		getline(myFile, line);

		stringstream iss(line);
		cout << endl;

		for (int col = 0; col < 9; ++col) //reads each number in the line
		{
			string val;
			getline(iss, val, ','); //removes commas

			stringstream convertor(val);
			convertor >> board[row][col]; //adds numbers to array
			cout << board[row][col] << " "; //prints out array
		}
	}
	cout << endl;

	//initializes the threads
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;

	//attaches each thread to a function
	pthread_create(&thread1, NULL, is_row_ok, NULL);
	pthread_create(&thread2, NULL, is_col_ok, NULL);
	pthread_create(&thread3, NULL, is_square_ok, NULL);

	//joins the threads back together when they complete
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	return 0;
}

void *is_row_ok(void *param)
{
	cout << endl;
	int element_count = 0;
	char element_value;
	for (int i = 0; i < 9; ++i) //goes through the 2d array line by line
	{
		for (int j = 0; j < 9; ++j) //goes through the 2d array column by column
		{
			element_count = 0; //initializes a counter
			element_value = board[i][j];
			if (element_value != ' ')
			{
				for (int k = 0; k < 9; ++k) //compares numbers in the array
				{
					if (board[i][k] == element_value)
						element_count++;
				}
			}
		}
		if (element_count >= 2) //checks to see if the counter has reached 2 or not (it shouldn't)
		{ //prints out the respective resonses
			cout << "Row " << i << " is invalid." << endl;
		}
		else
		{
			cout << "Row " << i << " is valid." << endl;
		}
	}
	pthread_exit(NULL); //terminates the thread
}

void *is_col_ok(void *param)
{
	cout << endl;
	int element_count = 0;
	char element_value;
	for (int j = 0; j < 9; ++j) //goes through the 2d array column by column
	{
		for (int i = 0; i < 9; ++i) //goes through the 2d array line by line
		{
			element_count = 0; //initializes a counter
			element_value = board[j][i];
			if (element_value != ' ')
			{
				for (int k = 0; k < 9; ++k)
				{
					if (board[k][j] == element_value) //compares numbers in the array
						element_count++;
				}
			}
		}

		if (element_count >= 2) //checks to see if the counter has reached 2 or not (it shouldn't)
		{ //prints out the respective resonses
			cout << "Column " << j << " is invalid." << endl;
		}
		else
		{
			cout << "Column " << j << " is valid." << endl;
		}
	}
	pthread_exit(NULL); //terminates the thread
}

void *is_square_ok (void *param)
{
	cout << endl;
	//initializes counters
	int square = 1;
	int element_count = 0;
	int square_count = 0;
	int element_value;
	/* the following for loops go through each sub-square in the 2d array
	* and compare each value to every one of the 9 values in that sub-square
	* to make sure that none match
	*/
	for (int m = 0; m < 9; m+=3)
	{
		for (int n = 0; n < 9; n+=3)
		{
			for (int i = m; i < m+3; i++)
			{
				for (int j = n; j < n+3; j++)
				{
					element_count = 0; //initializes a counter
					element_value = board[i][j];

					for (int k = m; k < m+3; k++)
					{
						for (int l = n; l < n+3; l++)
						{
							if (board[k][l] == element_value) //checks to see if the number
							{																  //at the inidcated point matches another number
								element_count++; //if so, increment the counter by 1
							}
						}
					}
					if (element_count == 1) //if no duplicate is found increment the counter by 1
					{
						square_count++;
					}
					else
					{
						square_count = square_count; //if a duplicate is found keep the counter the same
					}
				}
			}

			if(square_count == 9) //checks to see if the counter has reached 9.  if it has then the square is valid
			{
				cout << "Square " << square << " is valid." << endl;
				square_count = 0;
			}
			else //otherwise the square is not valid
			{
				cout << "Square " << square << " is invalid." << endl;
				square_count = 0;
			}
			square++;
		}
		square_count = 0;
	}
	pthread_exit(NULL); //terminates the thread
}
