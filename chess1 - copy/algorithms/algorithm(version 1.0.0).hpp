int computer_move(int arr[8][8], bool &specialMove)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<int> v;
    allmove(arr2, "player2", v);

    int SZ = v.size();
    std::srand((unsigned) time(0));
    int result = (rand() % SZ);
    int move = v[result];
    
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    int arrr[2][2] = {(move/1000)%10, (move/100)%10, (move/10)%10, move%10};
    bool spec = false;
    move_is_valid(arr2, arrr, "player2");
    specialMove = spec;
    return move;
}