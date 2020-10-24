int multi_max(int arr[8][8], int currentMinscore, int depth);
int multi_min(int arr[8][8], int currentMaxscore, int depth);

int eval(int arr2[8][8])
{
    int king_value[8][8] = {
        {-12,-16,-16,-20,-20,-16,-16,-12},
        {-12,-16,-16,-20,-20,-16,-16,-12},
        {-12,-16,-16,-20,-20,-16,-16,-12},
        {-12,-16,-16,-20,-20,-16,-16,-12},
        { -8,-12,-12,-16,-16,-12,-12, -8},
        { -4, -8, -8, -8, -8, -8, -8, -4},
        {  8,  8,  0,  0,  0,  0,  8,  8},
        {  8, 12,  4,  0,  0,  4, 12,  8}
    };

    int queen_value[8][8] = {
        { -8, -4, -4, -2, -2, -4, -4, -8},
        { -4,  0,  0,  0,  0,  0,  0, -4},
        { -4,  0,  2,  2,  2,  2,  0, -4},
        { -2,  0,  2,  2,  2,  2,  0, -2},
        {  0,  0,  2,  2,  2,  2,  0, -2},
        { -4,  2,  2,  2,  2,  2,  0, -4},
        { -4,  0,  2,  0,  0,  0,  0, -4},
        { -8, -4, -4, -2, -2, -4, -4, -8}
    };

    int rook_value[8][8] = {
        {  0,  0,  0,  0,  0,  0,  0,  0},
        {  2,  4,  4,  4,  4,  4,  4,  2},
        { -2,  0,  0,  0,  0,  0,  0, -2},
        { -2,  0,  0,  0,  0,  0,  0, -2},
        { -2,  0,  0,  0,  0,  0,  0, -2},
        { -2,  0,  0,  0,  0,  0,  0, -2},
        { -2,  0,  0,  0,  0,  0,  0, -2},
        {  0,  0,  0,  2,  2,  0,  0,  0}
    };

    int bishop_value[8][8] = {
        { -8, -4, -4, -4, -4, -4, -4, -8},
        { -4,  0,  0,  0,  0,  0,  0, -4},
        { -4,  0,  2,  4,  4,  2,  0, -4},
        { -4,  2,  2,  4,  4,  2,  2, -4},
        { -4,  0,  4,  4,  4,  4,  0, -4},
        { -4,  4,  4,  4,  4,  4,  4, -4},
        { -4,  2,  0,  0,  0,  0,  2, -4},
        { -8, -4, -4, -4, -4, -4, -4, -8}
    };

    int knight_value[8][8] = {
        {-20,-16,-12,-12,-12,-12,-16,-20},
        {-16, -8,  0,  0,  0,  0, -8,-16},
        {-12,  0,  4,  6,  6,  4,  0,-12},
        {-12,  2,  6,  8,  8,  6,  2,-12},
        {-12,  0,  6,  8,  8,  6,  0,-12},
        {-12,  2,  4,  6,  6,  4,  2,-12},
        {-16, -8,  0,  2,  2,  0, -8,-16},
        {-20,-16,-12,-12,-12,-12,-16,-20}
    };

    int pawn_value[8][8] = {
        {  0,  0,  0,  0,  0,  0,  0,  0},
        { 20, 20, 20, 20, 20, 20, 20, 20},
        {  4,  4,  8, 12, 12,  8,  4,  4},
        {  2,  2,  4, 10, 10,  4,  2,  2},
        {  0,  0,  0,  8,  8,  0,  0,  0},
        {  2, -2, -4,  0,  0, -4, -2,  2},
        {  2,  4,  4, -8, -8,  4,  4,  2},
        {  0,  0,  0,  0,  0,  0,  0,  0}
    };
    
    int sc = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (arr2[x][y] == 1) {
                sc-=200;
                sc-=rook_value[x][y];
            } if (arr2[x][y] == 2) {
                sc-=120;
                sc-=knight_value[x][y];
            } if (arr2[x][y] == 3) {
                sc-=120;
                sc-=bishop_value[x][y];
            } if (arr2[x][y] == 4) {
                sc-=360;
                sc-=queen_value[x][y];
            } if (arr2[x][y] == 5) {
                sc-=3600;
                sc-=king_value[x][y];
            } if (arr2[x][y] == 6) {
                sc-=40;
                sc-=pawn_value[x][y];
            } if (arr2[x][y] == -1) {
                sc+=200;
                sc+=rook_value[7-x][7-y];
            } if (arr2[x][y] == -2) {
                sc+=120;
                sc+=knight_value[7-x][7-y];
            } if (arr2[x][y] == -3) {
                sc+=120;
                sc+=bishop_value[7-x][7-y];
            } if (arr2[x][y] == -4) {
                sc+=360;
                sc+=queen_value[7-x][7-y];
            } if (arr2[x][y] == -5) {
                sc+=3600;
                sc+=king_value[7-x][7-y];
            } if (arr2[x][y] == -6) {
                sc+=40;
                sc+=pawn_value[7-x][7-y];
            }
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

chess_move computer_move(int board[8][8], int DEPTH)
{
    // search depth of DEPTH
    int tmpboard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) tmpboard[i][j] = board[i][j];
    vector<chess_move> v, w;
    allmove(tmpboard, "player2", v);

    if (DEPTH >= 2) {
        int maxscore = -99999999;
        for (chess_move i: v) {
            for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) tmpboard[k][j] = board[k][j];
            chess_move tmp = i;
            move_piece_simpler(tmpboard, tmp, "player2");

            int sc;
            if (DEPTH >= 3) sc = multi_min(tmpboard, maxscore, DEPTH-1);
            if (DEPTH == 2) sc = one_min(tmpboard, maxscore);

            if (sc == 1) continue;
            if (sc != maxscore) {
                maxscore = sc;
                w.clear();
            }
            w.push_back(i);
        }
    } else if (DEPTH == 1) {
        int maxscore = -99999999;
        for (chess_move i: v) {
            for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) tmpboard[k][j] = board[k][j];
            chess_move tmp = i;
            move_piece_simpler(tmpboard, tmp, "player2");

            int sc = eval(tmpboard);
            if (sc > maxscore) {
                maxscore = sc;
                w.clear();
            } else if (sc < maxscore) {
                continue;
            }
            w.push_back(i);
        }
    } else {
        for (chess_move i: v) w.push_back(i);
    }

    int SZ = w.size();
    std::srand((unsigned) time(0));
    int result = (rand() % SZ);
    chess_move move = w[result];
    return move;
}