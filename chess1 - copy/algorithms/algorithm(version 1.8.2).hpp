int multi_max(int arr[8][8], int currentMinscore, int depth);
int multi_min(int arr[8][8], int currentMaxscore, int depth);

int eval(int arr2[8][8])
{
    int sc = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (arr2[x][y] == 1) sc-=50;
            if (arr2[x][y] == 2) sc-=30;
            if (arr2[x][y] == 3) sc-=30;
            if (arr2[x][y] == 4) sc-=90;
            if (arr2[x][y] == 5) sc-=900;
            if (arr2[x][y] == 6) sc-=10;
            if (arr2[x][y] == -1) sc+=50;
            if (arr2[x][y] == -2) sc+=30;
            if (arr2[x][y] == -3) sc+=30;
            if (arr2[x][y] == -4) sc+=90;
            if (arr2[x][y] == -5) sc+=900;
            if (arr2[x][y] == -6) sc+=10;
        }
    }
    return sc;
}

int one_max(int arr[8][8], int currentMinscore)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<chess_move> v;
    allmove(arr2, "player2", v);

    int maxscore = -99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        chess_move tmp = i;
        move_piece_simpler(arr2, tmp, "player2");

        int sc = eval(arr2);
        maxscore = max(sc, maxscore);
        if (maxscore > currentMinscore) return 1;
    }

    if (v.size() != 0) return maxscore;
    else {
        if (!is_it_tie(arr, "player2")) return -1300;
        else {
            if (currentMinscore < 0) return 1;
            return 0;
        }
    }
}

int one_min(int arr[8][8], int currentMaxscore)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<chess_move> v;
    allmove(arr2, "player1", v);

    int minscore = 99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        chess_move tmp = i;
        move_piece_simpler(arr2, tmp, "player1");

        int sc = eval(arr2);
        minscore = min(minscore, sc);
        if (minscore < currentMaxscore) return 1;
    }

    if (v.size() != 0) return minscore;
    else {
        if (!is_it_tie(arr, "player1")) return 1300;
        else {
            if (currentMaxscore > 0) return 1;
            return 0;
        }
    }
}

int multi_max(int arr[8][8], int currentMinscore, int depth)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<chess_move> v;
    allmove(arr2, "player2", v);

    int maxscore = -99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        chess_move tmp = i;
        move_piece_simpler(arr2, tmp, "player2");
        
        int sc;
        if (depth == 2) sc = one_min(arr2, maxscore);
        if (depth > 2) sc = multi_min(arr2, maxscore, depth-1);

        if (sc == 1) continue;
        maxscore = sc;
        if (maxscore > currentMinscore) return 1;
    }

    if (v.size() != 0) return maxscore;
    else {
        if (!is_it_tie(arr, "player2")) return -1300;
        else {
            if (currentMinscore < 0) return 1;
            return 0;
        }
    }
}

int multi_min(int arr[8][8], int currentMaxscore, int depth)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<chess_move> v;
    allmove(arr2, "player1", v);

    int minscore = 99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        chess_move tmp = i;
        move_piece_simpler(arr2, tmp, "player1");

        int sc; 
        if (depth == 2) sc = one_max(arr2, minscore);
        if (depth > 2) sc = multi_max(arr2, minscore, depth-1);

        if (sc == 1) continue;
        minscore = sc;
        if (minscore < currentMaxscore) return 1;
    }

    if (v.size() != 0) return minscore;
    else {
        if (!is_it_tie(arr, "player1")) return 1300;
        else {
            if (currentMaxscore > 0) return 1;
            return 0;
        }
    }
}

chess_move computer_move(int arr[8][8], bool &specialMove)
{
    // search depth of DEPTH
    int DEPTH = 3;
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<chess_move> v, w;
    allmove(arr2, "player2", v);

    int maxscore = -99999999;
    for (chess_move i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        chess_move tmp = i;
        move_piece_simpler(arr2, tmp, "player2");

        int sc = multi_min(arr2, maxscore, DEPTH-1);
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