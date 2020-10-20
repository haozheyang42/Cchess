int multi_max(int board[8][8], int currentMinscore, int depth);
int multi_min(int board[8][8], int currentMaxscore, int depth);

int eval(int board[8][8])
{
    int sc = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board[x][y] == 1) sc-=50;
            if (board[x][y] == 2) sc-=30;
            if (board[x][y] == 3) sc-=30;
            if (board[x][y] == 4) sc-=90;
            if (board[x][y] == 5) sc-=900;
            if (board[x][y] == 6) sc-=10;
            if (board[x][y] == -1) sc+=50;
            if (board[x][y] == -2) sc+=30;
            if (board[x][y] == -3) sc+=30;
            if (board[x][y] == -4) sc+=90;
            if (board[x][y] == -5) sc+=900;
            if (board[x][y] == -6) sc+=10;
        }
    }
    return sc;
}

int one_max(int board[8][8], int currentMinscore)
{
    int tmpboard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) tmpboard[i][j] = board[i][j];
    vector<chess_move> v;
    allmove(tmpboard, "player2", v);

    int maxscore = -99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) tmpboard[k][j] = board[k][j];
        chess_move tmp = i;
        move_piece_simpler(tmpboard, tmp, "player2");

        int sc = eval(tmpboard);
        maxscore = max(sc, maxscore);
        if (maxscore > currentMinscore) return 1;
    }

    if (v.size() != 0) return maxscore;
    else {
        if (!is_it_tie(board, "player2")) return -1300;
        else {
            if (currentMinscore < 0) return 1;
            return 0;
        }
    }
}

int one_min(int board[8][8], int currentMaxscore)
{
    int tmpboard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) tmpboard[i][j] = board[i][j];
    vector<chess_move> v;
    allmove(tmpboard, "player1", v);

    int minscore = 99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) tmpboard[k][j] = board[k][j];
        chess_move tmp = i;
        move_piece_simpler(tmpboard, tmp, "player1");

        int sc = eval(tmpboard);
        minscore = min(minscore, sc);
        if (minscore < currentMaxscore) return 1;
    }

    if (v.size() != 0) return minscore;
    else {
        if (!is_it_tie(board, "player1")) return 1300;
        else {
            if (currentMaxscore > 0) return 1;
            return 0;
        }
    }
}

int multi_max(int board[8][8], int currentMinscore, int depth)
{
    int tmpboard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) tmpboard[i][j] = board[i][j];
    vector<chess_move> v;
    allmove(tmpboard, "player2", v);

    int maxscore = -99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) tmpboard[k][j] = board[k][j];
        chess_move tmp = i;
        move_piece_simpler(tmpboard, tmp, "player2");
        
        int sc;
        if (depth == 2) sc = one_min(tmpboard, maxscore);
        if (depth > 2) sc = multi_min(tmpboard, maxscore, depth-1);

        if (sc == 1) continue;
        maxscore = sc;
        if (maxscore > currentMinscore) return 1;
    }

    if (v.size() != 0) return maxscore;
    else {
        if (!is_it_tie(board, "player2")) return -1300;
        else {
            if (currentMinscore < 0) return 1;
            return 0;
        }
    }
}

int multi_min(int board[8][8], int currentMaxscore, int depth)
{
    int tmpboard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) tmpboard[i][j] = board[i][j];
    vector<chess_move> v;
    allmove(tmpboard, "player1", v);

    int minscore = 99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) tmpboard[k][j] = board[k][j];
        chess_move tmp = i;
        move_piece_simpler(tmpboard, tmp, "player1");

        int sc; 
        if (depth == 2) sc = one_max(tmpboard, minscore);
        if (depth > 2) sc = multi_max(tmpboard, minscore, depth-1);

        if (sc == 1) continue;
        minscore = sc;
        if (minscore < currentMaxscore) return 1;
    }

    if (v.size() != 0) return minscore;
    else {
        if (!is_it_tie(board, "player1")) return 1300;
        else {
            if (currentMaxscore > 0) return 1;
            return 0;
        }
    }
}

chess_move computer_move(int board[8][8]) throw (char*)
{
    // search depth of DEPTH
    int DEPTH = 3;
    int tmpboard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) tmpboard[i][j] = board[i][j];
    vector<chess_move> v, w;
    allmove(tmpboard, "player2", v);

    int maxscore = -99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) tmpboard[k][j] = board[k][j];
        chess_move tmp = i;
        move_piece_simpler(tmpboard, tmp, "player2");

        int sc = multi_min(tmpboard, maxscore, DEPTH-1);
        if (sc == 1) continue;
        if (sc != maxscore) {
            maxscore = sc;
            w.clear();
        }
        w.push_back(i);
    }

    int SZ = w.size();
    std::srand((unsigned) time(0));
    int result = (rand() % SZ);
    chess_move move = w[result];
    return move;
}