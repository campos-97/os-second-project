#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COMPUTER 1 
#define HUMAN 2 
  
#define SIDE 3 // Length of the board 
  
// Computer will move with 'O' 
// and human with 'X' 
#define COMPUTERMOVE 'O' 
#define HUMANMOVE 'X' 
 
// A function to initialise the game  
void initialise(int board[][SIDE]);
// A function that returns 1 if any of the row 
// is crossed with the same player's move 
int rowCrossed(int board[][SIDE]);
// A function that returns 1 if any of the column 
// is crossed with the same player's move 
int columnCrossed(int board[][SIDE]);
// A function that returns 1 if any of the diagonal 
// is crossed with the same player's move 
int diagonalCrossed(int board[][SIDE]);
// A function that returns 1 if the game is over 
// else it returns a 0 
int gameOver(int board[][SIDE]);