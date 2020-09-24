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
            if (arr2[x][y] == 1){
                sc-=200;
                sc-=rook_value[7-x][y];
            } if (arr2[x][y] == 2) {
                sc-=120;
                sc-=knight_value[7-x][y];
            } if (arr2[x][y] == 3) {
                sc-=120;
                sc-=bishop_value[7-x][y];
            } if (arr2[x][y] == 4) {
                sc-=360;
                sc-=queen_value[7-x][y];
            } if (arr2[x][y] == 5) {
                sc-=3600;
                sc-=king_value[7-x][y];
            } if (arr2[x][y] == 6) {
                sc-=40;
                sc-=pawn_value[7-x][y];
            } if (arr2[x][y] == -1) {
                sc+=200;
                sc+=rook_value[x][y];
            } if (arr2[x][y] == -2) {
                sc+=120;
                sc+=knight_value[x][y];
            } if (arr2[x][y] == -3) {
                sc+=120;
                sc+=bishop_value[x][y];
            } if (arr2[x][y] == -4) {
                sc+=360;
                sc+=queen_value[x][y];
            } if (arr2[x][y] == -5) {
                sc+=3600;
                sc+=king_value[x][y];
            } if (arr2[x][y] == -6) {
                sc+=40;
                sc+=pawn_value[x][y];
            }
        }
    }
    return sc;
}

int one_max(int arr[8][8], int currentMinscore)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<int> v;
    allmove(arr2, "player2", v);

    int maxscore = -99999999;
    for (int i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        int arrr[2][2] = {(i/1000)%10, (i/100)%10, (i/10)%10, i%10};
        bool spec = check_special_move(arr2, arrr);
        move_piece_simpler(arr2, arrr, "player2", spec);

        int sc = eval(arr2);
        maxscore = max(sc, maxscore);
        if (maxscore > currentMinscore) return 1;
    }

    if (v.size() != 0) return maxscore;
    else {
        if (!is_it_tie(arr, "player2")) return -5200;
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
    vector<int> v;
    allmove(arr2, "player1", v);

    int minscore = 99999999;
    for (int i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        int arrr[2][2] = {(i/1000)%10, (i/100)%10, (i/10)%10, i%10};
        bool spec = check_special_move(arr2, arrr);
        move_piece_simpler(arr2, arrr, "player1", spec);

        int sc = eval(arr2);
        minscore = min(minscore, sc);
        if (minscore < currentMaxscore) return 1;
    }

    if (v.size() != 0) return minscore;
    else {
        if (!is_it_tie(arr, "player1")) return 5200;
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
    vector<int> v;
    allmove(arr2, "player2", v);

    int maxscore = -99999999;
    for (int i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        int arrr[2][2] = {(i/1000)%10, (i/100)%10, (i/10)%10, i%10};
        bool spec = check_special_move(arr2, arrr);
        move_piece_simpler(arr2, arrr, "player2", spec);
        
        int sc;
        if (depth == 2) sc = one_min(arr2, maxscore);
        if (depth > 2) sc = multi_min(arr2, maxscore, depth-1);

        if (sc == 1) continue;
        maxscore = sc;
        if (maxscore > currentMinscore) return 1;
    }

    if (v.size() != 0) return maxscore;
    else {
        if (!is_it_tie(arr, "player2")) return -5200;
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
    vector<int> v;
    allmove(arr2, "player1", v);

    int minscore = 99999999;
    for (int i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        int arrr[2][2] = {(i/1000)%10, (i/100)%10, (i/10)%10, i%10};
        bool spec = check_special_move(arr2, arrr);
        move_piece_simpler(arr2, arrr, "player1", spec);

        int sc; 
        if (depth == 2) sc = one_max(arr2, minscore);
        if (depth > 2) sc = multi_max(arr2, minscore, depth-1);

        if (sc == 1) continue;
        minscore = sc;
        if (minscore < currentMaxscore) return 1;
    }

    if (v.size() != 0) return minscore;
    else {
        if (!is_it_tie(arr, "player1")) return 5200;
        else {
            if (currentMaxscore > 0) return 1;
            return 0;
        }
    }
}

int computer_move(int arr[8][8], bool &specialMove)
{
    // search depth of DEPTH
    int DEPTH = 3;
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<int> v, w;
    allmove(arr2, "player2", v);

    int maxscore = -99999999;
    for (int i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        int arrr[2][2] = {(i/1000)%10, (i/100)%10, (i/10)%10, i%10};
        bool spec = check_special_move(arr2, arrr);
        move_piece_simpler(arr2, arrr, "player2", spec);

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
    int move = w[result];

    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    int arrr[2][2] = {(move/1000)%10, (move/100)%10, (move/10)%10, move%10};
    bool spec = false;
    move_is_valid(arr2, arrr, "player2", spec);
    specialMove = spec;
    return move;
}