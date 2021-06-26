struct chess_move
{
    int special_move;
    /* 0 for non special
     * 1 for castling
     * 2 for en passent
     * 3 for pawn becomes rook
     * 4 for pawn becomes knight
     * 5 for pawn becomes bishop
     * 6 for pawn becomes queen
     * 7 for pwan reached endline without becomming amything
     */
    int start_row;
    int start_col;
    int end_row;
    int end_col;
};

using namespace std;
#include <bits/stdc++.h>
#include <sfml/Graphics.hpp>
#include <windows.h>
#include "checkAI.hpp"
#include "algorithm(generation 1).hpp"
#include "chess.hpp"

int main()
{
    // hide console
	HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    string turn = "player1";
    int DEPTH = 3;
    introduction(turn, DEPTH);

    // play chess
	int board[8][8] =
      {-1,-2,-3,-4,-5,-3,-2,-1,
       -6,-6,-6,-6,-6,-6,-6,-6,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        6, 6, 6, 6, 6, 6, 6, 6,
        1, 2, 3, 4, 5, 3, 2, 1};

    
	play_chess(board, turn, DEPTH);

    return 0;
}