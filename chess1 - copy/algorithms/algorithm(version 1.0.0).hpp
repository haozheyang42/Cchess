chess_move computer_move(int arr[8][8])
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<chess_move> v;
    allmove(arr2, "player2", v);

    int SZ = v.size();
    std::srand((unsigned) time(0));
    int result = (rand() % SZ);
    return v[result];
}