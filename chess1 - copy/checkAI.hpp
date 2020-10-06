bool move_is_valid(int arr[8][8], chess_move move, string p);
bool move_is_valid_simpler(int arr[8][8], chess_move move, string p);
bool is_place_safe(int arr[8][8], char p, int place[2]);
void allmove(int arr[8][8], string p, vector<int> &moves);
void move_piece_simpler(int arr[8][8], chess_move move, string p, int specialMove);
bool check_special_move(int arr[8][8], chess_move move, int &specialMove);

bool someone_won(int arr[8][8], string p)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    vector<int> v;
    allmove(arr2, p, v);
    if (v.size() == 0) {
        int kingpos[2];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (arr[i][j] == 5 && p[p.size()-1] == '1') {
                    kingpos[0] = i;
                    kingpos[1] = j;
                } if (arr[i][j] == -5 && p[p.size()-1] == '2') {
                    kingpos[0] = i;
                    kingpos[1] = j;
                }
            }
        }
        if (is_place_safe(arr, p[p.size()-1], kingpos)) {
            fstream fio;
            fio.open("chess.log", ios::out | ios::in);
            fio.seekg(0, ios::end);
            fio << "Tie game!" << endl;
            fio.close();
        } else {
            if (p == "player1") p = "player2";
            else p = "player1";
            fstream fio;
            fio.open("chess.log", ios::out | ios::in);
            fio.seekg(0, ios::end);
            if (p == "player1") fio << "player won!" << endl;
            else fio << "computer won!"<< endl;
            fio.close();
        }
        return true;
    } else {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (arr[i][j] != 0) count++;
            }
        }
        if (count == 2) return true;
    }
    return false;
}

void check_soldier_endline(int arr[8][8], chess_move move)
{
    for (int i = 0; i < 8; i++) if (arr[0][i] == 6) {
        const int size = 56;
        sf::Sprite fi[4];
        sf::RenderWindow window(sf::VideoMode(size*4, size), "Choose your piece");
        sf::Texture t3;
        t3.loadFromFile("figures.png");
        for(int i=0;i<4;i++) fi[i].setTexture(t3);
        fi[0].setTextureRect( sf::IntRect(0,size,size,size) );
        fi[0].setPosition(0,0);
        fi[1].setTextureRect( sf::IntRect(size,size,size,size) );
        fi[1].setPosition(size,0);
        fi[2].setTextureRect( sf::IntRect(2*size,size,size,size) );
        fi[2].setPosition(2*size,0);
        fi[3].setTextureRect( sf::IntRect(3*size,size,size,size) );
        fi[3].setPosition(3*size,0);
        int n;
        while (window.isOpen())
        {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                    for(int i=0;i<4;i++)
                        if (fi[i].getGlobalBounds().contains(pos.x,pos.y))
                            n = i;
                    window.close();
                }
            }
            
            window.clear();
            for(int i=0;i<4;i++) window.draw(fi[i]);
            window.display();
        }
        arr[0][i] = n+1;
        move.special_move = n+3;
    }
    for (int i = 0; i < 8; i++) if (arr[7][i] == -6) {
        arr[7][i] = -4;
    }
}

void allmove(int arr[8][8], string p, vector<int> &moves)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((p[p.size()-1] == '1' && arr[i][j] > 0) || (p[p.size()-1] == '2' && arr[i][j] < 0))
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        chess_move move = {0, i, j, x, y};
                        if (move_is_valid(arr, move, p)) {
                            moves.push_back(i*1000+j*100+x*10+y);
                        }
                    }
                }
        }
    }
}

bool is_place_safe(int arr[8][8], char p, int place[2])
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p == '1') {
                int tmp[2][2] = {{i, j}, {place[0], place[1]}};
                if (arr[i][j] < 0 && move_is_valid_simpler(arr, tmp, "player2"))
                    return false;
            } else {
                int tmp[2][2] = {{i, j}, {place[0], place[1]}};
                if (arr[i][j] > 0 && move_is_valid_simpler(arr, tmp, "player1"))
                    return false;
            }
        }
    }
    return true;
}

// checking weather a move is valid or not

bool rook_move_is_valid(int board[8][8],  chess_move move)
{
    int hori = abs(move.start_col-move.end_col);
    int vert = abs(move.start_row-move.end_row);
    if ((hori > 0 && vert > 0) || (hori == 0 && vert == 0)) return false;

    bool status = true;
    if (hori == 0) {
        if (move.start_row > move.end_row) {
            for (int i = move.end_row+1; i < move.start_row; i++) {
                if (board[i][move.start_col] != 0) status = false;
            }
        } else {
            for (int i = move.start_row+1; i < move.end_row; i++) {
                if (board[i][move.start_col] != 0) status = false;
            }
        }
    } else {
        if (move.start_col > move.end_col) {
            for (int i = move.end_col+1; i < move.start_col; i++) {
                if (board[move.start_row][i] != 0) status = false;
            }
        } else {
            for (int i = move.start_col+1; i < move.end_col; i++) {
                if (board[move.start_row][i] != 0) status = false;
            }
        }
    }
    return status;
}

bool bishop_move_is_valid(int board[8][8], chess_move move)
{
    int hori = abs(move.end_col-move.start_col);
    int vert = abs(move.end_row-move.start_row);
    if ((hori != vert) || (hori == 0 && vert == 0)) return false;

    bool status = true;
    if (move.start_row > move.end_row && move.start_col > move.end_col) {
        for (int i = 1; i < move.start_row-move.end_row; i++) {
            if (board[move.end_row+i][move.end_col+i] != 0) status = false;
        }
    } else if (move.start_row > move.end_row && move.start_col < move.end_col) {
        for (int i = 1; i < move.start_row-move.end_row; i++) {
            if (board[move.end_row+i][move.end_col-i] != 0) status = false;
        }
    } else if (move.start_row < move.end_row && move.start_col > move.end_col) {
        for (int i = 1; i < move.end_row-move.start_row; i++) {
            if (board[move.end_row-i][move.end_col+i] != 0) status = false;
        }
    } else {
        for (int i = 1; i < move.end_row-move.start_row; i++) {
            if (board[move.end_row-i][move.end_col-i] != 0) status = false;
        }
    }
    return status;
}

bool check_castling(int board[8][8], char p, chess_move move)
{
    if (p == '1' && player1castled) return false;
    if (p == '2' && player2castled) return false;
    if (move.start_row != move.end_row) return false;
    if (abs(move.start_col-move.end_col) != 2) return false;

    bool rookMovedBefore = false, kingMovedBefore = false;
    fstream fio;
    string line = "1";
    fio.open("chess.log", ios::in | ios::in);
    while (true) {
        getline(fio, line);
        if (line == "") break;
        line = line.substr(line.size()-5, 5);
        if (p == '1') {
            if (move.end_col == 6) {
                string a = line.substr(0, 2);
                if (a == "1e") kingMovedBefore = true;
                if (a == "1h") rookMovedBefore = true;
            } else if (move.end_col == 2) {
                string a = line.substr(0, 2);
                if (a == "1e") kingMovedBefore = true;
                if (a == "1a") rookMovedBefore = true;
            } else return false;
        } else {
            if (move.end_col == 6) {
                string a = line.substr(0, 2);
                if (a == "8e") kingMovedBefore = true;
                if (a == "8h") rookMovedBefore = true;
            } else if (move.end_col == 2) {
                string a = line.substr(0, 2);
                if (a == "8e") kingMovedBefore = true;
                if (a == "8a") rookMovedBefore = true;
            } else return false;
        }
    }
    fio.close();

    vector<int> v;
    bool nothingInBetween = false;
    if (move.end_col == 6) {
        if (board[move.start_row][5] == 0 && board[move.start_row][6] == 0)
            nothingInBetween = true;
        v.push_back(move.start_row*10000+4);
        v.push_back(move.start_row*10000+5);
        v.push_back(move.start_row*10000+6);
        v.push_back(move.start_row*10000+7);
    } if (move.end_col == 2) {
        if (board[move.start_row][3] == 0 && board[move.start_row][2] == 0 && board[move.start_row][1] == 0)
            nothingInBetween = true;
        v.push_back(move.start_row*10000+0);
        v.push_back(move.start_row*10000+1);
        v.push_back(move.start_row*10000+2);
        v.push_back(move.start_row*10000+3);
        v.push_back(move.start_row*10000+4);
    }

    bool somewhereBeingAttacked = false;
    for (int i: v) {
        int tmp[2] = {i/10000, i%10000};
        if (!is_place_safe(board, p, tmp)) somewhereBeingAttacked = true;
    }

    if (!rookMovedBefore && !kingMovedBefore && nothingInBetween && !somewhereBeingAttacked)
        return true;
    return false;
}

bool check_en_passent(int board[8][8], chess_move move, char p)
{
    fstream fio;
    string line;
    fio.open("chess.log", ios::in);
    fio.seekg(-7, ios::end);
    getline(fio, line);
    fio.close();

    if (line[2] != ' ') return false;
    int prevStart[2] = {7-(line[0]-'1'), line[1]-'a'};
    int prevEnd[2] = {7-(line[3]-'1'), line[4]-'a'};

    if (p == '1') {
        bool prevIsPawn = board[prevEnd[0]][prevEnd[1]] == -6;
        bool prevMovedTwoSteps = prevEnd[1] == prevStart[1] && prevEnd[0] == prevStart[0]+2;
        bool moveForwandOneStep = move.start_row == 3 && move.end_row == 2;
        bool moveSideOneStep = abs(move.end_col-move.start_col) == 1;
        bool stepOntoOtherPawn = prevEnd[1] == move.end_col;
        if (prevIsPawn && prevMovedTwoSteps && moveForwandOneStep && moveSideOneStep && stepOntoOtherPawn)
            return true;
    } else {
        bool prevIsPawn = board[prevEnd[0]][prevEnd[1]] == 6;
        bool prevMovedTwoSteps = prevEnd[1] == prevStart[1] && prevEnd[0] == prevStart[0]-2;
        bool moveForwandOneStep = move.start_row == 4 && move.end_row == 5;
        bool moveSideOneStep = abs(move.end_col-move.start_col) == 1;
        bool stepOntoOtherPawn = prevEnd[1] == move.end_col;
        if (prevIsPawn && prevMovedTwoSteps && moveForwandOneStep && moveSideOneStep && stepOntoOtherPawn)
            return true;
    }
    return false;
}

bool pawn_move_is_valid(int board[8][8], chess_move &move, char p)
{
    if (p == '1') {
        if (move.end_col == move.start_col) {
            if (move.start_row == 6 && (move.end_row == 5 || move.end_row == 4) && board[move.end_row][move.end_col] == 0) {
                if (move.end_row == 4 && board[5][move.start_col] == 0) return true;
                if (move.end_row == 4 && board[5][move.start_col] != 0) return false;
                return true;
            } else if (move.end_row - move.start_row == -1 && board[move.end_row][move.end_col] == 0) {
                return true;
            } return false;
        } else {
            if (move.start_row-move.end_row == 1 && abs(move.end_col-move.start_col) == 1) {
                if (board[move.end_row][move.end_col] < 0)
                    return true;
                else if (board[move.end_row][move.end_col] == 0) {
                    if (check_en_passent(board, move, p)) {
                        move.special_move = 2;
                        return true;
                    }
                    return false;
                }
            }
            return false;
        }
    } else {
        if (move.end_col == move.start_col) {
            if (move.start_row == 1 && (move.end_row == 2 || move.end_row == 3) && board[move.end_row][move.end_col] == 0)  {
                if (move.end_row == 3 && board[2][move.start_col] == 0) return true;
                if (move.end_row == 3 && board[2][move.start_col] != 0) return false;
                return true;
            } else if (move.end_row - move.start_row == 1 && board[move.end_row][move.end_col] == 0) {
                return true;
            } return false;
        } else {
            if (move.start_row-move.end_row == -1 && abs(move.end_col-move.start_col) == 1) {
                if (board[move.end_row][move.end_col] > 0)
                    return true;
                else if (board[move.end_row][move.end_col] == 0) {
                    if (check_en_passent(board, move, p)) {
                        move.special_move = 2;
                        return true;
                    }
                    return false;
                }
            } return false;
        }
    }
}



bool move_is_valid(int arr[8][8], chess_move move, string p, int &specialMove)
{   
    if (p[p.size()-1] == '1') {
        int tmp = arr[move.start_row][move.start_col];
        if (tmp <= 0) return false;
        tmp = arr[move.end_row][move.end_col];
        if (tmp > 0) return false;
    } else {
        int tmp = arr[move.start_row][move.start_col];
        if (tmp >= 0) return false;
        tmp = arr[move.end_row][move.end_col];
        if (tmp < 0) return false;
    }

    int valid = false;
    int piece = arr[move.start_row][move.start_col];

    // rook
    if (piece == 1 || piece == -1) {
        valid = rook_move_is_valid(arr, move);
    }

    // knight
    else if (piece == 2 || piece == -2) {
        vector<int> v = {abs(move.end_row - move.start_row), abs(move.end_col - move.start_col)};
        sort(v.begin(), v.end());
        if (v[0] == 1 && v[1] == 2) valid = true;
        else valid = false;
    }

    // bishop
    else if (piece == 3 || piece == -3) {
        valid = bishop_move_is_valid(arr, move);
    }

    // queen
    else if (piece == 4 || piece == -4) {
        valid = rook_move_is_valid(arr, move) || bishop_move_is_valid(arr, move);
    }

    // king
    else if (piece == 5 || piece == -5) {
        vector<int> v = {abs(move.end_row - move.start_row), abs(move.end_col - move.start_col)};
        sort(v.begin(), v.end());
        if (v[0] < 2 && v[1] < 2) valid = true;
        else if (v[0] == 0 && v[1] == 2) {
            if (p[p.size()-1] == '1' && check_castling(arr, '1', move)) {
                valid = true;
                move.special_move = 1;
            }
            if (p[p.size()-1] == '2' && check_castling(arr, '2', move)) {
                valid = true;
                move.special_move = 1;
            }
        } else valid = false;
    }

    // pawn
    else {
        valid = pawn_move_is_valid(arr, move, p[p.size()-1]);
    }

    if (!valid) return false;

    int clonedBoard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) clonedBoard[i][j] = arr[i][j];
    chess_move clonedMove = move;
    move_piece_simpler(clonedBoard, clonedMove, p);
    int place[2];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p[p.size()-1] == '1' && clonedBoard[i][j] == 5) {
                place[0] = i;
                place[1] = j;
            }
            if (p[p.size()-1] == '2' && clonedBoard[i][j] == -5) {
                place[0] = i;
                place[1] = j;
            }
        }
    }
    bool safe = false;
    safe = is_place_safe(clonedBoard, p[p.size()-1], place);
    if (!safe) return false;
    
    if (move.special_move == 2) check_soldier_endline(arr, move);
    return true;
}





bool move_is_valid_simpler(int arr[8][8], int arr2[2][2], string p)
{
    int start[2] = {arr2[0][0], arr2[0][1]};
    int end[2] = {arr2[1][0], arr2[1][1]};
    chess_move move = {0, arr2[0][0], arr2[0][1], arr2[1][0], arr2[1][1]};
    // cout << start[0] << ' ' << start[1] << endl;
    
    if (p[p.size()-1] == '1') {
        int tmp = arr[start[0]][start[1]];
        if (tmp <= 0) return false;
        tmp = arr[end[0]][end[1]];
        if (tmp > 0) return false;
    } else {
        int tmp = arr[start[0]][start[1]];
        if (tmp >= 0) return false;
        tmp = arr[end[0]][end[1]];
        if (tmp < 0) return false;
    }

    int valid = false;
    int piece = arr[start[0]][start[1]];

    // rook
    if (piece == 1 || piece == -1) {
        valid = rook_move_is_valid(arr, move);
    }

    // knight
    else if (piece == 2 || piece == -2) {
        vector<int> v = {abs(move.end_row - move.start_row), abs(move.end_col - move.start_col)};
        sort(v.begin(), v.end());
        if (v[0] == 1 && v[1] == 2) valid = true;
        else valid = false;
    }

    // bishop
    else if (piece == 3 || piece == -3) {
        valid = bishop_move_is_valid(arr, move);
    }

    // queen
    else if (piece == 4 || piece == -4) {
        valid = rook_move_is_valid(arr, move) || bishop_move_is_valid(arr, move);
    }

    // king
    else if (piece == 5 || piece == -5) {
        vector<int> v = {abs(move.end_row - move.start_row), abs(move.end_col - move.start_col)};
        sort(v.begin(), v.end());
        if (v[0] < 2 && v[1] < 2) valid = true;
        else if (v[0] == 0 && v[1] == 2) {
            if (p[p.size()-1] == '1' && check_castling(arr, '1', move)) {
                valid = true;
                move.special_move = 1;
            }
            if (p[p.size()-1] == '2' && check_castling(arr, '2', move)) {
                valid = true;
                move.special_move = 1;
            }
        } else valid = false;
    }

    // pawn
    else {
        valid = pawn_move_is_valid(arr, move, p[p.size()-1]);
    }

    if (!valid) return false;
    else return true;
}

bool check_special_move(int arr[8][8], int arrr[2][2], int &specialMove)
{
    int piece = arr[arrr[0][0]][arrr[0][1]];
    if (piece != 5 && piece != 6) specialMove = 0;
    else if (piece == 5) {
        if (abs(arrr[1][0]-arrr[0][0]) == 2 || abs(arrr[1][1]-arrr[0][1]) == 2) specialMove = 2;
        else specialMove = 0;
    } else {
        if (arrr[0][1] != arrr[1][1] && arr[arrr[1][0]][arrr[1][1]] == 0) specialMove = 1;
        else {
            if (arrr[1][0] == 0 or arrr[1][0] == 7) specialMove = 7;
            else specialMove = 0;
        }
    }
}

void move_piece(int arr[8][8], chess_move move, string p)
{
    fstream fio;
    fio.open("chess.log", ios::out | ios::in);
    fio.seekg(0, ios::end);
    if (p == "player1") fio << "player: ";
    if (p == "player2") fio << "computer: ";
    if (!move.special_move) {
        if (arr[move.start_row][move.start_col] == 1 || arr[move.start_row][move.start_col] == -1)
            fio << "Rook ";
        if (arr[move.start_row][move.start_col] == 2 || arr[move.start_row][move.start_col] == -2)
            fio << "Knight ";
        if (arr[move.start_row][move.start_col] == 3 || arr[move.start_row][move.start_col] == -3)
            fio << "Bishop ";
        if (arr[move.start_row][move.start_col] == 4 || arr[move.start_row][move.start_col] == -4)
            fio << "Queen ";
        if (arr[move.start_row][move.start_col] == 5 || arr[move.start_row][move.start_col] == -5)
            fio << "King ";
        if (arr[move.start_row][move.start_col] == 6 || arr[move.start_row][move.start_col] == -6)
            fio << "Pawn ";
        fio << (char)(7-move.start_row+'1') << (char)(move.start_col+'a') << ' ';
        fio << (char)(7-move.end_row+'1') << (char)(move.end_col+'a') << '\n';
        fio.close();
        arr[move.end_row][move.end_col] = arr[move.start_row][move.start_col];
        arr[move.start_row][move.start_col] = 0;
    } else {
        if (move.special_move == 1) {
            fio << "Castling ";
            arr[move.end_row][move.end_col] = arr[move.start_row][move.start_col];
            arr[move.start_row][move.start_col] = 0;
            if (move.end_col == 2) {
                arr[move.start_row][3] = arr[move.start_row][0];
                arr[move.start_row][0] = 0;
            } if (move.end_col == 6) {
                arr[move.start_row][5] = arr[move.start_row][7];
                arr[move.start_row][7] = 0;
            }
            if (p[p.size()-1] == '1') player1castled = true;
            if (p[p.size()-1] == '2') player2castled = true;
        }
        if (move.special_move == 2) {
            fio << "En passant ";
            arr[move.end_row][move.end_col] = arr[move.start_row][move.start_col];
            arr[move.start_row][move.start_col] = 0;
            if (p[p.size()-1] == '1') {
                arr[move.end_row+1][move.end_col] = 0;
            } else {
                arr[move.end_row-1][move.end_col] = 0;
            }
        }
        if (move.special_move == 3) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 1;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -1;
            arr[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 4) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 2;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -2;
            arr[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 5) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 3;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -3;
            arr[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 6) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 4;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -4;
            arr[move.start_row][move.start_col] = 0;
        }
        fio << (char)(7-move.start_row+'1') << (char)(move.start_col+'a') << ' ';
        fio << (char)(7-move.end_row+'1') << (char)(move.end_col+'a') << '\n';
        fio.close();
    }
}

void move_piece_simpler(int arr[8][8], chess_move move, string p)
{
    if (!move.special_move) {
        arr[move.end_row][move.end_col] = arr[move.start_row][move.start_col];
        arr[move.start_row][move.start_col] = 0;
    } else {
        if (move.special_move == 1) {
            arr[move.end_row][move.end_col] = arr[move.start_row][move.start_col];
            arr[move.start_row][move.start_col] = 0;
            if (move.end_col == 2) {
                arr[move.start_row][3] = arr[move.start_row][0];
                arr[move.start_row][0] = 0;
            } if (move.end_col == 6) {
                arr[move.start_row][5] = arr[move.start_row][7];
                arr[move.start_row][7] = 0;
            }
        }
        if (move.special_move == 2) {
            arr[move.end_row][move.end_col] = arr[move.start_row][move.start_col];
            arr[move.start_row][move.start_col] = 0;
            if (p[p.size()-1] == '1') {
                arr[move.end_row+1][move.end_col] = 0;
            } else {
                arr[move.end_row-1][move.end_col] = 0;
            }
        }
        if (move.special_move == 3) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 1;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -1;
            arr[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 4) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 2;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -2;
            arr[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 5) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 3;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -3;
            arr[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 6) {
            if (p[p.size()-1] == '1') arr[move.end_row][move.end_col] = 4;
            if (p[p.size()-1] == '2') arr[move.end_row][move.end_col] = -4;
            arr[move.start_row][move.start_col] = 0;
        }
    }
}




// game ends functions
bool is_it_tie(int arr[8][8], string p)
{
    int arr2[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) arr2[i][j] = arr[i][j];
    
    int kingpos[2];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (arr[i][j] == 5 && p[p.size()-1] == '1') {
                kingpos[0] = i;
                kingpos[1] = j;
            } if (arr[i][j] == -5 && p[p.size()-1] == '2') {
                kingpos[0] = i;
                kingpos[1] = j;
            }
        }
    }
    if (is_place_safe(arr, p[p.size()-1], kingpos)) return true;
    else return false;
}

void losted() {
	MessageBoxA(NULL, "The Computer Won!! Good Luck Next Time!", "You Lost!", MB_OK);
}

void won() {
	MessageBoxA(NULL, "Congratulations You Won!!", "You Won!", MB_OK);
}

void tied() {
	MessageBoxA(NULL, "Ooops, Tie Game!!", "Tie Game!", MB_OK);
}

void game_ended(int arr[8][8], string turn)
{
    if (turn == "player1" && !is_it_tie(arr, turn)) losted();
    else if (turn == "player2" && !is_it_tie(arr, turn)) won();
    else tied();
}