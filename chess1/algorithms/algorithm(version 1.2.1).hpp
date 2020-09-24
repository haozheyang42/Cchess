int one_min(int arr[8][8])
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<int> v;
    allmove(arr2, "player1", v);

    vector<int> score;
    int minscore = 99999999;
    for (int i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        int arrr[2][2] = {(i/1000)%10, (i/100)%10, (i/10)%10, i%10};
        bool spec = false;
        move_is_valid(arr2, arrr, "player1", spec);
        move_piece_simpler(arr2, arrr, "player1", spec);

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
        if (sc < minscore) minscore = sc;
        score.push_back(sc);
    }

    if (score.size() != 0) return minscore;
    else {
        if (!is_it_tie(arr, "player1")) return 1300;
        else return 0;
    }
}

int one_maximin(int arr[8][8])
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<int> v;
    allmove(arr2, "player2", v);

    vector<int> score;
    int maxscore = -99999999;
    for (int i: v) {
        for (int k = 0; k < 8; k++) for (int j = 0; j < 8; j++) arr2[k][j] = arr[k][j];
        int arrr[2][2] = {(i/1000)%10, (i/100)%10, (i/10)%10, i%10};
        bool spec = false;
        move_is_valid(arr2, arrr, "player2", spec);
        move_piece_simpler(arr2, arrr, "player2", spec);

        int sc = one_min(arr2);
        if (sc > maxscore) maxscore = sc;
        score.push_back(sc);
    }

    vector<int> moves;
    for (int j = 0; j < score.size(); j++)
        if (score[j] == maxscore) moves.push_back(j);
    int SZ = moves.size();
    std::srand((unsigned) time(0));
    int result = (rand() % SZ);
    int move = v[moves[result]];
    return move;
}

int computer_move(int arr[8][8], bool &specialMove)
{
    // search depth of 2
    int arr2[8][8];
    int move = one_maximin(arr);
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    int arrr[2][2] = {(move/1000)%10, (move/100)%10, (move/10)%10, move%10};
    bool spec = false;
    move_is_valid(arr2, arrr, "player2", spec);
    specialMove = spec;
    return move;
}