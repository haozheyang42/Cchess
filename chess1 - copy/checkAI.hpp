bool move_is_valid(int arr[8][8], int arr2[2][2], string p, int &specialMove);
bool move_is_valid_simpler(int arr[8][8], int arr2[2][2], string p);
bool is_place_safe(int arr[8][8], char p, int place[2]);
void allmove(int arr[8][8], string p, vector<int> &moves);
void move_piece_simpler(int arr[8][8], int arr2[2][2], string p, int specialMove);
bool check_special_move(int arr[8][8], int arrr[2][2], int &specialMove);

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

void check_soldier_endline(int arr[8][8], int &specialMove)
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
        specialMove = n+3;
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
                        int arr2[2][2] = {{i, j}, {x, y}};
                        int kkk = 0;
                        if (move_is_valid(arr, arr2, p, kkk)) {
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



bool is_car_move_valid(int arr[8][8], int start[2], int end[2])
{
    int hori = abs(end[0]-start[0]);
    int vert = abs(end[1]-start[1]);
    if ((hori > 0 && vert > 0) || (hori == 0 && vert == 0)) return false;
    if (hori == 0) {
        bool status = true;
        if (start[1] > end[1]) {
            for (int i = end[1]+1; i < start[1]; i++) {
                if (arr[start[0]][i] != 0) status = false;
            }
        } else {
            for (int i = start[1]+1; i < end[1]; i++) {
                if (arr[start[0]][i] != 0) status = false;
            }
        } return status;
    } else {
        bool status = true;
        if (start[0] > end[0]) {
            for (int i = end[0]+1; i < start[0]; i++) {
                if (arr[i][start[1]] != 0) status = false;
            }
        } else {
            for (int i = start[0]+1; i < end[0]; i++) {
                if (arr[i][start[1]] != 0) status = false;
            }
        } return status;
    }
}

bool is_elephant_move_valid(int arr[8][8], int start[2], int end[2])
{
    int hori = abs(end[1]-start[1]);
    int vert = abs(end[0]-start[0]);
    bool status = true;
    if ((hori != vert) || (hori == 0 && vert == 0)) return false;
    if (start[0] > end[0] && start[1] > end[1]) {
        for (int i = 1; i < start[0]-end[0]; i++) {
            if (arr[end[0]+i][end[1]+i] != 0) status = false;
        }
    } else if (start[0] > end[0] && start[1] < end[1]) {
        for (int i = 1; i < start[0]-end[0]; i++) {
            if (arr[end[0]+i][end[1]-i] != 0) status = false;
        }
    } else if (start[0] < end[0] && start[1] > end[1]) {
        for (int i = 1; i < end[0]-start[0]; i++) {
            if (arr[end[0]-i][end[1]+i] != 0) status = false;
        }
    } else {
        for (int i = 1; i < end[0]-start[0]; i++) {
            if (arr[end[0]-i][end[1]-i] != 0) status = false;
        }
    }
    return status;
}

bool check_castling(int arr[8][8], char p, int start[2], int end[2])
{
    if (p == '1' && player1castled) return false;
    if (p == '2' && player2castled) return false;
    if (start[0] != end[0]) return false;
    if (abs(start[1]-end[1]) != 2) return false;

    bool carMovedBefore = false, kingMovedBefore = false;
    fstream fio;
    string line = "1";
    fio.open("chess.log", ios::in | ios::in);
    while (true) {
        getline(fio, line);
        if (line == "") break;
        line = line.substr(line.size()-5, 5);
        if (p == '1') {
            if (end[1] == 6) {
                string a = line.substr(0, 2);
                if (a == "1e") kingMovedBefore = true;
                if (a == "1h") carMovedBefore = true;
            } else if (end[1] == 2) {
                string a = line.substr(0, 2);
                if (a == "1e") kingMovedBefore = true;
                if (a == "1a") carMovedBefore = true;
            } else return false;
        } else {
            if (end[1] == 6) {
                string a = line.substr(0, 2);
                if (a == "8e") kingMovedBefore = true;
                if (a == "8h") carMovedBefore = true;
            } else if (end[1] == 2) {
                string a = line.substr(0, 2);
                if (a == "8e") kingMovedBefore = true;
                if (a == "8a") carMovedBefore = true;
            } else return false;
        }
    }
    fio.close();

    vector<int> v;
    bool nothingInBetween = false;
    if (end[1] == 6) {
        if (arr[start[0]][5] == 0 && arr[start[0]][6] == 0)
            nothingInBetween = true;
        v.push_back(start[0]*10000+4);
        v.push_back(start[0]*10000+5);
        v.push_back(start[0]*10000+6);
        v.push_back(start[0]*10000+7);
    } if (end[1] == 2) {
        if (arr[start[0]][3] == 0 && arr[start[0]][2] == 0 && arr[start[0]][1] == 0)
            nothingInBetween = true;
        v.push_back(start[0]*10000+0);
        v.push_back(start[0]*10000+1);
        v.push_back(start[0]*10000+2);
        v.push_back(start[0]*10000+3);
        v.push_back(start[0]*10000+4);
    }

    bool somewhereBeingAttacked = false;
    for (int i: v) {
        int tmp[2] = {i/10000, i%10000};
        if (!is_place_safe(arr, p, tmp)) somewhereBeingAttacked = true;
    }

    if (!carMovedBefore && !kingMovedBefore && nothingInBetween && !somewhereBeingAttacked)
        return true;
    return false;
}

bool eat_pass_by_soldier(int arr[8][8], int start[2], int end[2], char p)
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
        bool a = arr[prevEnd[0]][prevEnd[1]] == -6 && prevEnd[1] == prevStart[1] && prevEnd[0] == prevStart[0]+2;
        bool b = start[0] == 3 && end[0] == 2 && abs(end[1]-start[1]) == 1 && abs(start[1]-prevEnd[1]) == 1 && prevEnd[1] == end[1];
        if (a && b) return true;
        return false;
    } else {
        bool a = arr[prevEnd[0]][prevEnd[1]] == 6 && prevEnd[1] == prevStart[1] && prevEnd[0] == prevStart[0]-2;
        bool b = start[0] == 4 && end[0] == 5 && abs(end[1]-start[1]) == 1 && abs(start[1]-prevEnd[1]) == 1 && prevEnd[1] == end[1];
        if (a && b) return true;
        return false;
    }
}

bool is_soldier_move_valid(int arr[8][8], int start[2], int end[2], char p)
{
    if (p == '1') {
        if (end[1] == start[1]) {
            if (start[0] == 6 && (end[0] == 5 || end[0] == 4) && arr[end[0]][end[1]] == 0) {
                if (end[0] == 4 && arr[5][start[1]] == 0) return true;
                if (end[0] == 4 && arr[5][start[1]] != 0) return false;
                return true;
            } else if (end[0] - start[0] == -1 && arr[end[0]][end[1]] == 0) {
                return true;
            } return false;
        } else {
            if (start[0]-end[0] == 1 && abs(end[1]-start[1]) == 1) {
                if (arr[end[0]][end[1]] < 0)
                    return true;
                else if (arr[end[0]][end[1]] == 0) {
                    if (eat_pass_by_soldier(arr, start, end, p)) {
                        return true;
                    }
                    return false;
                }
            }
            return false;
        }
    } else {
        if (end[1] == start[1]) {
            if (start[0] == 1 && (end[0] == 2 || end[0] == 3) && arr[end[0]][end[1]] == 0)  {
                if (end[0] == 3 && arr[2][start[1]] == 0) return true;
                if (end[0] == 3 && arr[2][start[1]] != 0) return false;
                return true;
            } else if (end[0] - start[0] == 1 && arr[end[0]][end[1]] == 0) {
                return true;
            } return false;
        } else {
            if (start[0]-end[0] == -1 && abs(end[1]-start[1]) == 1) {
                if (arr[end[0]][end[1]] > 0)
                    return true;
                else if (arr[end[0]][end[1]] == 0) {
                    if (eat_pass_by_soldier(arr, start, end, p)) {
                        return true;
                    }
                    return false;
                }
            } return false;
        }
    }
}

bool move_is_valid(int arr[8][8], int arr2[2][2], string p, int &specialMove)
{
    int start[2] = {arr2[0][0], arr2[0][1]};
    int end[2] = {arr2[1][0], arr2[1][1]};
    
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
    if (piece == 1 || piece == -1) {
        valid = is_car_move_valid(arr, start, end);
    }
    else if (piece == 2 || piece == -2) {
        vector<int> v = {abs(end[0]-start[0]), abs(end[1]-start[1])};
        sort(v.begin(), v.end());
        if (v[0] == 1 && v[1] == 2) valid = true;
        else valid = false;
    }
    else if (piece == 3 || piece == -3) {
        valid = is_elephant_move_valid(arr, start, end);
    }
    else if (piece == 4 || piece == -4) {
        valid = is_car_move_valid(arr, start, end) || is_elephant_move_valid(arr, start, end);
    }
    else if (piece == 5 || piece == -5) {
        vector<int> v = {abs(end[0]-start[0]), abs(end[1]-start[1])};
        sort(v.begin(), v.end());
        if (v[0] < 2 && v[1] < 2) valid = true;
        else if (v[0] == 0 && v[1] == 2) {
            if (p[p.size()-1] == '1' && check_castling(arr, '1', start, end)) {
                valid = true;
            }
            if (p[p.size()-1] == '2' && check_castling(arr, '2', start, end)) {
                valid = true;
            }
        } else valid = false;
    }
    else {
        valid = is_soldier_move_valid(arr, start, end, p[p.size()-1]);
    }

    if (!valid) return false;

    int arrrrrr[2][2] = {start[0], start[1], end[0], end[1]};
    check_special_move(arr, arrrrrr, specialMove);

    int mymy[8][8];
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) mymy[i][j] = arr[i][j];
    int arrrr[2][2] = {start[0], start[1], end[0], end[1]};
    move_piece_simpler(mymy, arrrr, p, specialMove);
    int place[2];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (p[p.size()-1] == '1' && mymy[i][j] == 5) {
                place[0] = i;
                place[1] = j;
            }
            if (p[p.size()-1] == '2' && mymy[i][j] == -5) {
                place[0] = i;
                place[1] = j;
            }
        }
    }
    bool safe = false;
    safe = is_place_safe(mymy, p[p.size()-1], place);
    if (!safe) return false;
    
    if (specialMove == 7) check_soldier_endline(arr, specialMove);
    return true;
}

bool move_is_valid_simpler(int arr[8][8], int arr2[2][2], string p)
{
    int start[2] = {arr2[0][0], arr2[0][1]};
    int end[2] = {arr2[1][0], arr2[1][1]};
    
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
    if (piece == 1 || piece == -1) {
        valid = is_car_move_valid(arr, start, end);
    }
    else if (piece == 2 || piece == -2) {
        vector<int> v = {abs(end[0]-start[0]), abs(end[1]-start[1])};
        sort(v.begin(), v.end());
        if (v[0] == 1 && v[1] == 2) valid = true;
        else valid = false;
    }
    else if (piece == 3 || piece == -3) {
        valid = is_elephant_move_valid(arr, start, end);
    }
    else if (piece == 4 || piece == -4) {
        valid = is_car_move_valid(arr, start, end) || is_elephant_move_valid(arr, start, end);
    }
    else if (piece == 5 || piece == -5) {
        vector<int> v = {abs(end[0]-start[0]), abs(end[1]-start[1])};
        sort(v.begin(), v.end());
        if (v[0] < 2 && v[1] < 2) valid = true;
        else if (v[0] == 0 && v[1] == 2) {
            if (p[p.size()-1] == '1' && check_castling(arr, '1', start, end)) {
                valid = true;
            }
            if (p[p.size()-1] == '2' && check_castling(arr, '2', start, end)) {
                valid = true;
            }
        } else valid = false;
    }
    else {
        valid = is_soldier_move_valid(arr, start, end, p[p.size()-1]);
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

void move_piece(int arr[8][8], int arr2[2][2], string p, int specialMove)
{
    fstream fio;
    fio.open("chess.log", ios::out | ios::in);
    fio.seekg(0, ios::end);
    if (p == "player1") fio << "player: ";
    if (p == "player2") fio << "computer: ";
    if (!specialMove) {
        if (arr[arr2[0][0]][arr2[0][1]] == 1 || arr[arr2[0][0]][arr2[0][1]] == -1)
            fio << "Rook ";
        if (arr[arr2[0][0]][arr2[0][1]] == 2 || arr[arr2[0][0]][arr2[0][1]] == -2)
            fio << "Knight ";
        if (arr[arr2[0][0]][arr2[0][1]] == 3 || arr[arr2[0][0]][arr2[0][1]] == -3)
            fio << "Bishop ";
        if (arr[arr2[0][0]][arr2[0][1]] == 4 || arr[arr2[0][0]][arr2[0][1]] == -4)
            fio << "Queen ";
        if (arr[arr2[0][0]][arr2[0][1]] == 5 || arr[arr2[0][0]][arr2[0][1]] == -5)
            fio << "King ";
        if (arr[arr2[0][0]][arr2[0][1]] == 6 || arr[arr2[0][0]][arr2[0][1]] == -6)
            fio << "Pawn ";
        fio << (char)(7-arr2[0][0]+'1') << (char)(arr2[0][1]+'a') << ' ';
        fio << (char)(7-arr2[1][0]+'1') << (char)(arr2[1][1]+'a') << '\n';
        fio.close();
        if (arr[arr2[1][0]][arr2[1][1]]) remaining--;
        arr[arr2[1][0]][arr2[1][1]] = arr[arr2[0][0]][arr2[0][1]];
        arr[arr2[0][0]][arr2[0][1]] = 0;
    } else {
        if (specialMove == 2) {
            fio << "Castling ";
            arr[arr2[1][0]][arr2[1][1]] = arr[arr2[0][0]][arr2[0][1]];
            arr[arr2[0][0]][arr2[0][1]] = 0;
            if (arr2[1][1] == 2) {
                arr[arr2[0][0]][3] = arr[arr2[0][0]][0];
                arr[arr2[0][0]][0] = 0;
            } if (arr2[1][1] == 6) {
                arr[arr2[0][0]][5] = arr[arr2[0][0]][7];
                arr[arr2[0][0]][7] = 0;
            }
            if (p[p.size()-1] == '1') player1castled = true;
            if (p[p.size()-1] == '2') player2castled = true;
        }
        if (specialMove == 1) {
            fio << "En passant ";
            arr[arr2[1][0]][arr2[1][1]] = arr[arr2[0][0]][arr2[0][1]];
            arr[arr2[0][0]][arr2[0][1]] = 0;
            if (p[p.size()-1] == '1') {
                arr[arr2[1][0]+1][arr2[1][1]] = 0;
            } else {
                arr[arr2[1][0]-1][arr2[1][1]] = 0;
            }
            remaining--;
        }
        if (specialMove == 3) {
            if (p[p.size()-1] == '1') arr[arr2[1][0]][arr2[1][1]] = 1;
            if (p[p.size()-1] == '2') arr[arr2[1][0]][arr2[1][1]] = -1;
            arr[arr2[0][0]][arr2[0][1]] = 0;
        }
        if (specialMove == 4) {
            if (p[p.size()-1] == '1') arr[arr2[1][0]][arr2[1][1]] = 2;
            if (p[p.size()-1] == '2') arr[arr2[1][0]][arr2[1][1]] = -2;
            arr[arr2[0][0]][arr2[0][1]] = 0;
        }
        if (specialMove == 5) {
            if (p[p.size()-1] == '1') arr[arr2[1][0]][arr2[1][1]] = 3;
            if (p[p.size()-1] == '2') arr[arr2[1][0]][arr2[1][1]] = -3;
            arr[arr2[0][0]][arr2[0][1]] = 0;
        }
        if (specialMove == 6) {
            if (p[p.size()-1] == '1') arr[arr2[1][0]][arr2[1][1]] = 4;
            if (p[p.size()-1] == '2') arr[arr2[1][0]][arr2[1][1]] = -4;
            arr[arr2[0][0]][arr2[0][1]] = 0;
        }
        fio << (char)(7-arr2[0][0]+'1') << (char)(arr2[0][1]+'a') << ' ';
        fio << (char)(7-arr2[1][0]+'1') << (char)(arr2[1][1]+'a') << '\n';
        fio.close();
    }
}

void move_piece_simpler(int arr[8][8], int arr2[2][2], string p, int specialMove)
{
    if (!specialMove) {
        arr[arr2[1][0]][arr2[1][1]] = arr[arr2[0][0]][arr2[0][1]];
        arr[arr2[0][0]][arr2[0][1]] = 0;
    } else {
        if (specialMove == 2) {
            arr[arr2[1][0]][arr2[1][1]] = arr[arr2[0][0]][arr2[0][1]];
            arr[arr2[0][0]][arr2[0][1]] = 0;
            if (arr2[1][1] == 2) {
                arr[arr2[0][0]][3] = arr[arr2[0][0]][0];
                arr[arr2[0][0]][0] = 0;
            } if (arr2[1][1] == 6) {
                arr[arr2[0][0]][5] = arr[arr2[0][0]][7];
                arr[arr2[0][0]][7] = 0;
            }
        }
        if (specialMove == 1) {
            arr[arr2[1][0]][arr2[1][1]] = arr[arr2[0][0]][arr2[0][1]];
            arr[arr2[0][0]][arr2[0][1]] = 0;
            if (p[p.size()-1] == '1') {
                arr[arr2[1][0]+1][arr2[1][1]] = 0;
            } else {
                arr[arr2[1][0]-1][arr2[1][1]] = 0;
            }
        }
        if (specialMove == 7) {
            arr[arr2[1][0]][arr2[1][1]] = arr[arr2[0][0]][arr2[0][1]];
            arr[arr2[0][0]][arr2[0][1]] = 0;
        }
    }
}

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


void losted()
{
	MessageBoxA(NULL, "The Computer Won!! Good Luck Next Time!", "You Lost!", MB_OK);
}

void won()
{
	MessageBoxA(NULL, "Congratulations You Won!!", "You Won!", MB_OK);
}

void tied()
{
	MessageBoxA(NULL, "Ooops, Tie Game!!", "Tie Game!", MB_OK);
}

void game_ended(int arr[8][8], string turn)
{
    if (turn == "player1" && !is_it_tie(arr, turn)) losted();
    else if (turn == "player2" && !is_it_tie(arr, turn)) won();
    else tied();
}