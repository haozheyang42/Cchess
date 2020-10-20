bool move_is_valid(int board[8][8], chess_move &move, string p);
bool move_is_valid_simpler(int board[8][8], chess_move move, string p);
bool is_place_safe(int board[8][8], string p, int place[2]);
void allmove(int board[8][8], string p, vector<chess_move> &moves);
void move_piece_simpler(int board[8][8], chess_move move, string p);


// checking weather a move is valid or not -- 
bool is_place_safe(int board[8][8], string p, int place[2])
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p == "player1") {
                chess_move tmp = {0, i, j, place[0], place[1]};
                if (board[i][j] < 0 && move_is_valid_simpler(board, tmp, "player2"))
                    return false;
            } else {
                chess_move tmp = {0, i, j, place[0], place[1]};
                if (board[i][j] > 0 && move_is_valid_simpler(board, tmp, "player1"))
                    return false;
            }
        }
    }
    return true;
}

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

bool check_castling(int board[8][8], string p, chess_move move)
{
    if (p == "player1" && player1castled) return false;
    if (p == "player2" && player2castled) return false;
    if (move.start_row != move.end_row) return false;
    if (abs(move.start_col-move.end_col) != 2) return false;

    bool rookMovedBefore = false, kingMovedBefore = false;
    fstream fin;
    fin.open("chess.log", ios::in);
    string line;
    while (true) {
        getline(fin, line);
        if (line == "") break;
        line = line.substr(line.size()-5, 5);
        if (p == "player1") {
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
    fin.close();

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

bool check_en_passent(int board[8][8], chess_move &move, string p)
{
    fstream fin;
    string line;
    fin.open("chess.log", ios::in);
    fin.seekg(-7, ios::end);
    getline(fin, line);
    fin.close();

    if (line[2] != ' ') return false;
    int prevStart[2] = {7-(line[0]-'1'), line[1]-'a'};
    int prevEnd[2] = {7-(line[3]-'1'), line[4]-'a'};

    if (p == "player1") {
        bool prevIsPawn = board[prevEnd[0]][prevEnd[1]] == -6;
        bool prevMovedTwoSteps = prevEnd[1] == prevStart[1] && prevEnd[0] == prevStart[0]+2;
        bool moveForwandOneStep = move.start_row == 3 && move.end_row == 2;
        bool moveSideOneStep = abs(move.end_col-move.start_col) == 1;
        bool stepOntoOtherPawn = prevEnd[1] == move.end_col;
        if (prevIsPawn && prevMovedTwoSteps && moveForwandOneStep && moveSideOneStep && stepOntoOtherPawn) {
            move.special_move = 2;
            return true;
        }
    } else {
        bool prevIsPawn = board[prevEnd[0]][prevEnd[1]] == 6;
        bool prevMovedTwoSteps = prevEnd[1] == prevStart[1] && prevEnd[0] == prevStart[0]-2;
        bool moveForwandOneStep = move.start_row == 4 && move.end_row == 5;
        bool moveSideOneStep = abs(move.end_col-move.start_col) == 1;
        bool stepOntoOtherPawn = prevEnd[1] == move.end_col;
        if (prevIsPawn && prevMovedTwoSteps && moveForwandOneStep && moveSideOneStep && stepOntoOtherPawn) {
            move.special_move = 2;
            return true;
        }
    }
    return false;
}

bool pawn_move_is_valid(int board[8][8], chess_move &move, string p)
{
    if (p == "player1") {
        if (move.end_col == move.start_col) {
            if (move.start_row == 6 && (move.end_row == 5 || move.end_row == 4) && board[move.end_row][move.end_col] == 0) {
                if (move.end_row == 4 && board[5][move.start_col] == 0) return true;
                if (move.end_row == 4 && board[5][move.start_col] != 0) return false;
                return true;
            } else if (move.end_row - move.start_row == -1 && board[move.end_row][move.end_col] == 0) {
                if (move.end_row == 0) move.special_move = 7;
                return true;
            } return false;
        } else {
            if (move.start_row-move.end_row == 1 && abs(move.end_col-move.start_col) == 1) {
                if (board[move.end_row][move.end_col] < 0) {
                    if (move.end_row == 0) move.special_move = 7;
                    return true;
                } else if (board[move.end_row][move.end_col] == 0) {
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
                if (move.end_row == 7) move.special_move = 7;
                return true;
            } return false;
        } else {
            if (move.start_row-move.end_row == -1 && abs(move.end_col-move.start_col) == 1) {
                if (board[move.end_row][move.end_col] > 0) {
                    if (move.end_row == 7) move.special_move = 7;
                    return true;
                } else if (board[move.end_row][move.end_col] == 0) {
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

bool move_is_valid(int board[8][8], chess_move &move, string p)
{   
    if (p == "player1") {
        int tmp = board[move.start_row][move.start_col];
        if (tmp <= 0) return false;
        tmp = board[move.end_row][move.end_col];
        if (tmp > 0) return false;
    } else {
        int tmp = board[move.start_row][move.start_col];
        if (tmp >= 0) return false;
        tmp = board[move.end_row][move.end_col];
        if (tmp < 0) return false;
    }

    int valid = false;
    int piece = board[move.start_row][move.start_col];

    // rook
    if (piece == 1 || piece == -1) {
        valid = rook_move_is_valid(board, move);
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
        valid = bishop_move_is_valid(board, move);
    }

    // queen
    else if (piece == 4 || piece == -4) {
        valid = rook_move_is_valid(board, move) || bishop_move_is_valid(board, move);
    }

    // king
    else if (piece == 5 || piece == -5) {
        vector<int> v = {abs(move.end_row - move.start_row), abs(move.end_col - move.start_col)};
        sort(v.begin(), v.end());
        if (v[0] < 2 && v[1] < 2) valid = true;
        else if (v[0] == 0 && v[1] == 2 && move.start_row == move.end_row && move.start_col == 4) {
            if (check_castling(board, p, move)) {
                valid = true;
                move.special_move = 1;
            }
        } else valid = false;
    }

    // pawn
    else {
        valid = pawn_move_is_valid(board, move, p);
    }

    if (!valid) return false;

    int clonedBoard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) clonedBoard[i][j] = board[i][j];
    chess_move clonedMove = move;
    move_piece_simpler(clonedBoard, clonedMove, p);
    int place[2];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p == "player1" && clonedBoard[i][j] == 5) {
                place[0] = i;
                place[1] = j;
            }
            if (p == "player2" && clonedBoard[i][j] == -5) {
                place[0] = i;
                place[1] = j;
            }
        }
    }

    bool safe = false;
    safe = is_place_safe(clonedBoard, p, place);
    if (!safe) return false;
    return true;
}

bool move_is_valid_simpler(int board[8][8], chess_move move, string p)
{
    if (p == "player1") {
        int tmp = board[move.start_row][move.start_col];
        if (tmp <= 0) return false;
        tmp = board[move.end_row][move.end_col];
        if (tmp > 0) return false;
    } else {
        int tmp = board[move.start_row][move.start_col];
        if (tmp >= 0) return false;
        tmp = board[move.end_row][move.end_col];
        if (tmp < 0) return false;
    }

    int valid = false;
    int piece = board[move.start_row][move.start_col];

    // rook
    if (piece == 1 || piece == -1) {
        valid = rook_move_is_valid(board, move);
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
        valid = bishop_move_is_valid(board, move);
    }

    // queen
    else if (piece == 4 || piece == -4) {
        valid = rook_move_is_valid(board, move) || bishop_move_is_valid(board, move);
    }

    // king
    else if (piece == 5 || piece == -5) {
        vector<int> v = {abs(move.end_row - move.start_row), abs(move.end_col - move.start_col)};
        sort(v.begin(), v.end());
        if (v[0] < 2 && v[1] < 2) valid = true;
        else if (v[0] == 0 && v[1] == 2) {
            if (check_castling(board, p, move)) {
                valid = true;
                move.special_move = 1;
            }
        } else valid = false;
    }

    // pawn
    else {
        valid = pawn_move_is_valid(board, move, p);
    }

    if (!valid) return false;
    else return true;
}



// moving piece functions -- done
void check_soldier_endline(int board[8][8], chess_move &move)
{
    const int size = 56;
    sf::Sprite fi[4];
    sf::RenderWindow window(sf::VideoMode(size*4, size), "Choose your piece");
    sf::Texture t3;
    t3.loadFromFile("figures.png");
    for(int j=0;j<4;j++) fi[j].setTexture(t3);
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
                for(int j=0;j<4;j++)
                    if (fi[j].getGlobalBounds().contains(pos.x,pos.y))
                        n = j;
                window.close();
            }
        }
        
        window.clear();
        for(int j=0;j<4;j++) window.draw(fi[j]);
        window.display();
    }
    move.special_move = n+3;

    for (int i = 0; i < 8; i++) if (board[7][i] == -6) {
        board[7][i] = -4;
    }
}

void move_piece(int board[8][8], chess_move move, string p)
{
    fstream fout;
    fout.open("chess.log", ios::app | ios::out);
    fout.seekg(0, ios::end);
    if (p == "player1") fout << "player: ";
    if (p == "player2") fout << "computer: ";
    if (!move.special_move) {
        if (board[move.start_row][move.start_col] == 1 || board[move.start_row][move.start_col] == -1)
            fout << "Rook ";
        if (board[move.start_row][move.start_col] == 2 || board[move.start_row][move.start_col] == -2)
            fout << "Knight ";
        if (board[move.start_row][move.start_col] == 3 || board[move.start_row][move.start_col] == -3)
            fout << "Bishop ";
        if (board[move.start_row][move.start_col] == 4 || board[move.start_row][move.start_col] == -4)
            fout << "Queen ";
        if (board[move.start_row][move.start_col] == 5 || board[move.start_row][move.start_col] == -5)
            fout << "King ";
        if (board[move.start_row][move.start_col] == 6 || board[move.start_row][move.start_col] == -6)
            fout << "Pawn ";
        fout << (char)(7-move.start_row+'1') << (char)(move.start_col+'a') << ' ';
        fout << (char)(7-move.end_row+'1') << (char)(move.end_col+'a') << '\n';
        fout.close();
        board[move.end_row][move.end_col] = board[move.start_row][move.start_col];
        board[move.start_row][move.start_col] = 0;
    } else {
        if (move.special_move == 1) {
            fout << "Castling ";
            board[move.end_row][move.end_col] = board[move.start_row][move.start_col];
            board[move.start_row][move.start_col] = 0;
            if (move.end_col == 2) {
                board[move.start_row][3] = board[move.start_row][0];
                board[move.start_row][0] = 0;
            } if (move.end_col == 6) {
                board[move.start_row][5] = board[move.start_row][7];
                board[move.start_row][7] = 0;
            }
            if (p == "player1") player1castled = true;
            if (p == "player2") player2castled = true;
        }
        if (move.special_move == 2) {
            fout << "En passant ";
            board[move.end_row][move.end_col] = board[move.start_row][move.start_col];
            board[move.start_row][move.start_col] = 0;
            if (p == "player1") {
                board[move.end_row+1][move.end_col] = 0;
            } else {
                board[move.end_row-1][move.end_col] = 0;
            }
        }
        if (move.special_move == 7) {
            check_soldier_endline(board, move);
        }
        if (move.special_move == 3) {
            if (p == "player1") board[move.end_row][move.end_col] = 1;
            if (p == "player2") board[move.end_row][move.end_col] = -1;
            board[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 4) {
            if (p == "player1") board[move.end_row][move.end_col] = 2;
            if (p == "player2") board[move.end_row][move.end_col] = -2;
            board[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 5) {
            if (p == "player1") board[move.end_row][move.end_col] = 3;
            if (p == "player2") board[move.end_row][move.end_col] = -3;
            board[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 6) {
            if (p == "player1") board[move.end_row][move.end_col] = 4;
            if (p == "player2") board[move.end_row][move.end_col] = -4;
            board[move.start_row][move.start_col] = 0;
        }
        fout << (char)(7-move.start_row+'1') << (char)(move.start_col+'a') << ' ';
        fout << (char)(7-move.end_row+'1') << (char)(move.end_col+'a') << '\n';
        fout.close();
    }
}

void move_piece_simpler(int board[8][8], chess_move move, string p)
{
    if (!move.special_move) {
        board[move.end_row][move.end_col] = board[move.start_row][move.start_col];
        board[move.start_row][move.start_col] = 0;
    } else {
        if (move.special_move == 1) {
            board[move.end_row][move.end_col] = board[move.start_row][move.start_col];
            board[move.start_row][move.start_col] = 0;
            if (move.end_col == 2) {
                board[move.start_row][3] = board[move.start_row][0];
                board[move.start_row][0] = 0;
            } if (move.end_col == 6) {
                board[move.start_row][5] = board[move.start_row][7];
                board[move.start_row][7] = 0;
            }
        }
        if (move.special_move == 2) {
            board[move.end_row][move.end_col] = board[move.start_row][move.start_col];
            board[move.start_row][move.start_col] = 0;
            if (p == "player1") {
                board[move.end_row+1][move.end_col] = 0;
            } else {
                board[move.end_row-1][move.end_col] = 0;
            }
        }
        if (move.special_move == 3) {
            if (p == "player1") board[move.end_row][move.end_col] = 1;
            if (p == "player2") board[move.end_row][move.end_col] = -1;
            board[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 4) {
            if (p == "player1") board[move.end_row][move.end_col] = 2;
            if (p == "player2") board[move.end_row][move.end_col] = -2;
            board[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 5) {
            if (p == "player1") board[move.end_row][move.end_col] = 3;
            if (p == "player2") board[move.end_row][move.end_col] = -3;
            board[move.start_row][move.start_col] = 0;
        }
        if (move.special_move == 6) {
            if (p == "player1") board[move.end_row][move.end_col] = 4;
            if (p == "player2") board[move.end_row][move.end_col] = -4;
            board[move.start_row][move.start_col] = 0;
        }
    }
}

void allmove(int board[8][8], string p, vector<chess_move> &moves)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((p == "player1" && board[i][j] > 0) || (p == "player2" && board[i][j] < 0))
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        chess_move move = {0, i, j, x, y};
                        if (move_is_valid(board, move, p)) {
                            moves.push_back(move);
                        }
                    }
                }
        }
    }
    for (chess_move i: moves) {
        if (i.special_move == 7) {
            i.special_move = 6;
            chess_move j = i;
            for (int k = 3; k < 6; k++) {
                j.special_move = k;
                moves.push_back(j);
            }
        }
    }
}



// game ends functions -- done
bool someone_won(int board[8][8], string p)
{
    int tmpboard[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) tmpboard[i][j] = board[i][j];
    vector<chess_move> v;
    allmove(tmpboard, p, v);
    if (v.size() == 0) {
        int kingpos[2];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] == 5 && p == "player1") {
                    kingpos[0] = i;
                    kingpos[1] = j;
                } if (board[i][j] == -5 && p == "player2") {
                    kingpos[0] = i;
                    kingpos[1] = j;
                }
            }
        }
        if (is_place_safe(board, p, kingpos)) {
            fstream fout;
            fout.open("chess.log", ios::app | ios::out);
            fout.seekg(0, ios::end);
            fout << "Tie game!" << endl;
            fout.close();
        } else {
            if (p == "player1") p = "player2";
            else p = "player1";
            fstream fout;
            fout.open("chess.log", ios::app | ios::out);
            fout.seekg(0, ios::end);
            if (p == "player1") fout << "player won!" << endl;
            else fout << "computer won!"<< endl;
            fout.close();
        }
        return true;
    } else {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != 0) count++;
            }
        }
        if (count == 2) return true;
    }
    return false;
}

bool is_it_tie(int board[8][8], string p)
{
    int kingpos[2];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == 5 && p == "player1") {
                kingpos[0] = i;
                kingpos[1] = j;
            } if (board[i][j] == -5 && p == "player2") {
                kingpos[0] = i;
                kingpos[1] = j;
            }
        }
    }
    if (is_place_safe(board, p, kingpos)) return true;
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

void surrender(string &turn)
{
    int ret = MessageBoxA(NULL, "Do you want to surrender to Haozhe Yang's algorithm?", "Surrender?", MB_YESNO);
    if (ret == 6) {
        turn = "nobody";
        losted();
    }
}

void game_ended(int board[8][8], string turn)
{
    if (turn == "player1" && !is_it_tie(board, turn)) losted();
    else if (turn == "player2" && !is_it_tie(board, turn)) won();
    else tied();
}