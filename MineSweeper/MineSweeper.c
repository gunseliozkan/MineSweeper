#include <stdio.h>
#include <stdlib.h>
#define GRIDSIZE 4

typedef enum {mined,empty,flaggedMined,flaggedEmpty,closedEmpty,closedMined}cell;
void printGrid (cell grid[][GRIDSIZE]);
int openCell(cell grid[][GRIDSIZE], int x, int y);
void flagCell(cell grid[][GRIDSIZE], int x, int y);
int isCellEmpty(cell grid[][GRIDSIZE], int x, int y);
int isLocationLegal(int x, int y);
void initGrid(cell grid[][GRIDSIZE]);
int asMain();
int isGameFinished(cell grid[][GRIDSIZE]);

int main()
{
	asMain();
	return 0;
}

void initGrid(cell grid[][GRIDSIZE])
{
	int row, column;
	for(row=0; row<GRIDSIZE; row++)
	{
		for(column=0; column<GRIDSIZE; column++) /*fill with closed empty first*/
		{
			grid[row][column] = closedEmpty;
		}
	}

    /*then, place mines*/
	grid[0][3] = closedMined;
	grid[1][2] = closedMined;
	grid[3][1] = closedMined;
	grid[2][0] = closedMined;
}

void printGrid (cell grid[][GRIDSIZE])
{
	/*this does nothing on the grid, print function just shows user what he/she
	  is doing.*/
	int row, column;

	for(row=0; row<GRIDSIZE; row++) /*for rows*/
	{
		for(column=0; column<GRIDSIZE; column++) /*for columns*/
		{
			switch(grid[row][column]) /*user sees the grid differently than the
										actual grid.*/
			{
				case mined:
					printf("m ");
					break;
				case empty:
					printf("e ");
					break;
				case closedEmpty:
					printf(". ");
					break;
				case closedMined:
					printf(". ");
					break;
				case flaggedMined:
					printf("f ");
					break;
				case flaggedEmpty:
					printf("f ");
					break;
			}
		}
		printf("\n");
	}
}

int isLocationLegal(int x, int y)
{
	int result; /*return value is 0 if error otherwise 1*/
	if(x>=GRIDSIZE || y>=GRIDSIZE || x<0 || y<0)
		result = 0;
	else
		result = 1;
	return result;
}

int openCell(cell grid[][GRIDSIZE], int x, int y)
{
	int result=0; /*-2 if flagged cell or illegal location, -1 if mined cell, 0 otherwise*/
	int checkLocation = isLocationLegal(x,y);

	if(checkLocation == 0 || grid[x][y] == flaggedMined || grid[x][y] == flaggedEmpty)
	{
		result = -2;
		printf("Location isn't legal or flagged cell!\n");
	}

	else
	{
		if(grid[x][y] == closedMined) /*trying to open mined cell*/
			result=-1;

		else if(grid[x][y] == closedEmpty) /*good choice, closed empty cell is
											 selected to be opened*/
		{

			grid[x][y] = empty;

			if(grid[x-1][y]==closedEmpty && isLocationLegal(x-1,y)==1) /*ALWAYS
																		 CHECK CELL*/
				grid[x-1][y] = empty;

			if(grid[x+1][y]==closedEmpty && isLocationLegal(x+1,y)==1)
				grid[x+1][y] = empty;

			if(grid[x-1][y-1]==closedEmpty && isLocationLegal(x-1,y-1)==1)
				grid[x-1][y-1] = empty;

			if(grid[x-1][y+1]==closedEmpty && isLocationLegal(x-1,y+1)==1)
				grid[x-1][y+1] = empty;

			if(grid[x][y-1]==closedEmpty && isLocationLegal(x,y-1)==1)
				grid[x][y-1] = empty;

			if(grid[x][y+1]==closedEmpty && isLocationLegal(x,y+1)==1)
				grid[x][y+1] = empty;

			if(grid[x+1][y-1]==closedEmpty && isLocationLegal(x+1,y-1)==1)
				grid[x+1][y-1] = empty;

			if(grid[x+1][y+1]==closedEmpty && isLocationLegal(x+1,y+1)==1)
				grid[x+1][y+1] = empty;
		}

		else if(isCellEmpty(grid, x, y) == 1) /*trying to open an already open cell*/
			printf("This cell is already empty. \n");

	}

	return result;
}

void flagCell(cell grid[][GRIDSIZE], int x, int y)
{
	/*flag operations ->*/
	if(grid[x][y] == closedMined)
		grid[x][y] = flaggedMined;

	else if(grid[x][y] == closedEmpty)
		grid[x][y] = flaggedEmpty;

	/*unflag operations ->*/
	else if(grid[x][y] == flaggedMined)
		grid[x][y] = closedMined;

	else if(grid[x][y] == flaggedEmpty)
		grid[x][y] = closedEmpty;

}

int isCellEmpty(cell grid[][GRIDSIZE], int x, int y)
{
	/*i just didn't get where to use this function??*/
	int result; /*return value is 1 if empty 0 otherwise*/
	if(grid[x][y] == empty)
		result = 1;
	else
		result = 0;
	return result;
}


int asMain()
{
	int openCellReturn;
	char flagOrOpen; /*user will open the cell or flag the cell*/
	int cellX, cellY; /*cell selection (user input)*/
	char continueGame='y'; /*game status, continue game by y, quit by e*/
	int stepCounter=0; /*well the name is clear, counter of the steps*/

	cell board[GRIDSIZE][GRIDSIZE];
	initGrid(board);
	printGrid(board);

	do
	{
		printf("Enter x: ");
		scanf("%d", &cellX);
		printf("Enter y: ");
		scanf("%d", &cellY);

		if(isLocationLegal(cellX,cellY) == 0)
		{
			while(isLocationLegal(cellX,cellY) == 0)
			{
				printf("Location isn't legal please enter X and Y again->\n");
				printf("Enter x: ");
				scanf("%d", &cellX);
				printf("Enter y: ");
				scanf("%d", &cellY);
			}
		}


		printf("Enter operation (flag=f / open=o): ");
		scanf(" %c", &flagOrOpen);

		switch(flagOrOpen)
		{
			case 'f':
				flagCell(board, cellX, cellY); /*call flag function*/
				break;
			case 'o':
				openCellReturn = openCell(board, cellX, cellY); /*call open function*/
				break;
		}

		if(openCellReturn == -1) /*mined cell is open*/
			break;

		printGrid(board); /*print board after opening or flag operation*/
		printf("You want to continue? (y/n): "); /*ask if user wants to continue*/
		scanf(" %c", &continueGame);
		stepCounter++; /*increase step by 1*/

	}while(continueGame == 'y' && isGameFinished(board) == 0);

	if(openCellReturn == -1)
		printf("You lost the game :(\n");

	if(isGameFinished(board) == 1)
		printf("YAY! You finished the game with %d steps!\n", stepCounter);

	return 0;
}

int isGameFinished(cell grid[][GRIDSIZE])
{
	int sumOfEmptyAndFlagged=0; /*when the game is finished there are only flagged empty cells or empty cells so total number of empty+flaggedEmpty = GRIDSIZE^2*/
	int result; /*return value is 0 if not finished, 1 if win*/
	int row, column;
	for(row=0; row<GRIDSIZE; row++)
	{
		for(column=0; column<GRIDSIZE; column++)
		{
			/*condition of increasing the sum by 1*/
			if(isCellEmpty(grid, row, column) == 1 || grid[row][column] == flaggedMined)
				sumOfEmptyAndFlagged++;


		}
	}

	if(sumOfEmptyAndFlagged == (GRIDSIZE*GRIDSIZE))
		result = 1;
	else
		result = 0;

	return result;
}
