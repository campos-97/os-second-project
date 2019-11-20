#include "tictactoe.h"
  
// A function to initialise the game  
void initialise(int board[][SIDE]) { 
    // Initially the board is empty 
    for (int i=0; i<SIDE; i++) 
    { 
        for (int j=0; j<SIDE; j++) 
            board[i][j] = -1; 
    }
    return; 
} 
  
// A function that returns 1 if any of the row 
// is crossed with the same player's move 
int rowCrossed(int board[][SIDE]) 
{ 
    for (int i=0; i<SIDE; i++) 
    { 
        if (board[i][0] == board[i][1] && 
            board[i][1] == board[i][2] &&  
            board[i][0] != -1) 
            return i; 
    } 
    return -1; 
} 
  
// A function that returns 1 if any of the column 
// is crossed with the same player's move 
int columnCrossed(int board[][SIDE]) 
{ 
    for (int i=0; i<SIDE; i++) 
    { 
        if (board[0][i] == board[1][i] && 
            board[1][i] == board[2][i] &&  
            board[0][i] != -1) 
            return i+3; 
    } 
    return -1; 
} 
  
// A function that returns 1 if any of the diagonal 
// is crossed with the same player's move 
int diagonalCrossed(int board[][SIDE]) 
{ 
    if (board[0][0] == board[1][1] && 
        board[1][1] == board[2][2] &&  
        board[0][0] != -1) 
        return 6;
          
    if (board[0][2] == board[1][1] && 
        board[1][1] == board[2][0] && 
         board[0][2] != -1) 
        return 7; 
  
    return -1; 
}

int boardFull(int board[][SIDE]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == -1) return 0;
        }
    }
    return 1;
}
  
// A function that returns 1 if the game is over 
// else it returns a 0 
int gameOver(int board[][SIDE]) 
{ 
    int result = rowCrossed(board);
    if(result != -1)return result;
    result = columnCrossed(board);
    if(result != -1)return result;
    result = diagonalCrossed(board);
    if(result != -1)return result;
    return -1;
} 