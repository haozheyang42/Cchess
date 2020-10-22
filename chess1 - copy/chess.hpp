const int sizeOfSprite = 56;

#define display_chess_board(window, board, sprites) \
	window.clear(); \
	window.draw(board); \
	for (sf::Sprite i: sprites) window.draw(i); \
	window.display();

#define display_chess_board_with_avalibles(window, board, sprites, avalibles) \
	window.clear(); \
	window.draw(board); \
	for (sf::Sprite i: sprites) window.draw(i); \
	for (sf::Sprite i: avalibles) window.draw(i); \
	window.display();

bool check(int board[8][8], sf::Vector2f oldPos, sf::Vector2f newPos, string p)
{
    if (oldPos.x/sizeOfSprite < 0 || oldPos.x/sizeOfSprite > 7) return false;
    if (oldPos.y/sizeOfSprite < 0 || oldPos.y/sizeOfSprite > 7) return false;
    if (newPos.x/sizeOfSprite < 0 || newPos.x/sizeOfSprite > 7) return false;
    if (newPos.y/sizeOfSprite < 0 || newPos.y/sizeOfSprite > 7) return false;

    chess_move move = {0, int(oldPos.y/sizeOfSprite), int(oldPos.x/sizeOfSprite), 
                int(newPos.y/sizeOfSprite), int(newPos.x/sizeOfSprite)};

    if (board[move.start_row][move.start_col] == 0) return false;
    if (move_is_valid(board, move, p)) {
        move_piece(board, move, p);
        return true;
    } return false;
}

void load_position(int board[8][8] ,vector<sf::Sprite> &chessFigures)
{
    int k = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            // Get the chess piece. If it is 0 then skip
            int n = board[i][j];
            if (!n) continue;

            // x is the chesspiece without colour
            int x = abs(n) - 1;
            // y is the chesspiece's colour
            int y = n>0 ? 1:0;

            // cut out the piece
            chessFigures[k].setTextureRect(sf::IntRect(sizeOfSprite*x, sizeOfSprite*y, sizeOfSprite, sizeOfSprite));

            // put the piece into the correct location
            chessFigures[k].setPosition(sizeOfSprite*j, sizeOfSprite*i);
            k++;
        }
}

// change the way of space being pressed, if some pieces were eaten.

void play_chess(int board[8][8], string turn)
{
    
    // render window
    sf::RenderWindow window(sf::VideoMode(sizeOfSprite*8, sizeOfSprite*8), "Haozhe Yang's Chess Game!");

    // import texture
    sf::Texture t1,t2,t3;
    t1.loadFromFile("figures.png");
    t2.loadFromFile("board.png");    
    t3.loadFromFile("avalible.png");

    // load in the chess figures
    vector<sf::Sprite> chessFigures(32);
    for(int i=0;i<32;i++) chessFigures[i].setTexture(t1);
    load_position(board, chessFigures);

    // load in board
    sf::Sprite chessBoard(t2);

    // load in the blue dot for position that you are able to move to
    sf::Sprite blueDot(t3);
    vector<sf::Sprite> avalibles;


    // varibles req change
    bool isMove=false;
    float dx=0, dy=0;
    sf::Vector2f oldPos,newPos;
    string str;
    int n=0;

    int Cmove;
    int pieceLastMoved;
    
    chess_move computerMove;


    // clear everything in chess.log
    fstream fout;
    fout.open("chess.log", ios::trunc | ios::out);
    fout.close();

    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        sf::Event e;

        // computer makes a move
        if (turn == "player2") {
            // get computer's move
            // try {
            //     computerMove = computer_move(board);
            // } catch (...) {
            //     computerMove = {0, 0, 0, 0, 0};
            //     while (!move_is_valid(board, computerMove, "player2")) {
            //         cout << "The computer AI has crashed! Please enter an input for the computer\n";
            //         cin >> computerMove.start_row >> computerMove.start_col >> computerMove.end_row >> computerMove.end_col;
            //     }
            // }
            computerMove = computer_move(board);
            // cout << computerMove.special_move << endl;
            // cout << computerMove.start_row << ' ' << computerMove.start_col << ' ';
            // cout << computerMove.end_row << ' ' << computerMove.end_col << ' ';
            
            oldPos = sf::Vector2f(computerMove.start_col*sizeOfSprite, computerMove.start_row*sizeOfSprite);
            newPos = sf::Vector2f(computerMove.end_col*sizeOfSprite, computerMove.end_row*sizeOfSprite);

            // move piece
            for (int i = 0; i < chessFigures.size(); i++)
                if (chessFigures[i].getPosition() == oldPos) n=i;
            sf::Vector2f p = newPos - oldPos;
            for (int i = 0; i < 10; i++) {
                chessFigures[n].move(p.x/10, p.y/10);
                display_chess_board(window, chessBoard, chessFigures);
            }
            move_piece(board, computerMove, turn);

            // check win, if not move on to player
            turn = "player1";
            if (someone_won(board, turn)) {
                game_ended(board, turn);
                turn = "nobody";
                continue;
            }
        }

        // game on going
        while (turn == "player1" && window.pollEvent(e)) {
            // previous move req change into function
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                // get the preious move
                oldPos = sf::Vector2f(computerMove.start_col*sizeOfSprite, computerMove.start_row*sizeOfSprite);
                newPos = sf::Vector2f(computerMove.end_col*sizeOfSprite, computerMove.end_row*sizeOfSprite);
                for (int i = 0; i < chessFigures.size(); i++)
                    if (chessFigures[i].getPosition() == newPos) n = i;

                // move the piece
                sf::Vector2f p = oldPos - newPos;
                
                for (int k = 0; k < 10; k++) {
                    chessFigures[n].move(p.x/10, p.y/10); 
                    display_chess_board(window, chessBoard, chessFigures);
                }
                p = newPos - oldPos;
                for (int k = 0; k < 10; k++) {
                    chessFigures[n].move(p.x/10, p.y/10); 
                    display_chess_board(window, chessBoard, chessFigures);
                }
            }
            
            // drag
            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                for (int i = 0; i < chessFigures.size(); i++)
                    if (chessFigures[i].getGlobalBounds().contains(pos.x,pos.y)) {
                        // get original position
                        oldPos = chessFigures[i].getPosition();

                        // display places that the piece can move to
                        int arr[2] = {int(oldPos.y)/sizeOfSprite, int(oldPos.x)/sizeOfSprite};
                        vector<int> v;
                        for (int i = 0; i < 8; i++)
                            for (int j = 0; j < 8; j++) {
                                chess_move move = {0, arr[0], arr[1], i, j};
                                if (move_is_valid(board, move, "player1"))
                                    v.push_back(i*10+j);
                            }
                        for (int i: v) {
                            sf::Sprite tmp = blueDot;
                            tmp.setPosition(sizeOfSprite*(i%10), sizeOfSprite*(i/10));
                            avalibles.push_back(tmp);
                        }

                        if (board[arr[0]][arr[1]] > 0) {
                            isMove = true; n =i;
                            dx=pos.x - chessFigures[i].getPosition().x;
                            dy=pos.y - chessFigures[i].getPosition().y;
                        }
                    }
            }

            // drop
            if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
                avalibles.clear();
                // center of sprite
                sf::Vector2f p = chessFigures[n].getPosition() + sf::Vector2f(sizeOfSprite/2, sizeOfSprite/2);
                // put sprite into grid
                newPos = sf::Vector2f(sizeOfSprite*int(p.x/sizeOfSprite), sizeOfSprite*int(p.y/sizeOfSprite));
                if (check(board, oldPos, newPos, turn) && isMove) {
                    isMove = false;
                    turn = "player2";
                    if (someone_won(board, turn)) {
                        game_ended(board, turn);
                        turn = "nobody";
                        continue;
                    }
                }
                isMove=false;
            }

            // surrender
            if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                surrender(turn);
                continue;
            }
        }

        // game ended
        while (turn == "nobody" && window.pollEvent(e)) {
            // close
            if (e.type == sf::Event::Closed) window.close();

            // last move
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                // get the preious move
                oldPos = sf::Vector2f(computerMove.start_col*sizeOfSprite, computerMove.start_row*sizeOfSprite);
                newPos = sf::Vector2f(computerMove.end_col*sizeOfSprite, computerMove.end_row*sizeOfSprite);
                for (int i = 0; i < chessFigures.size(); i++)
                    if (chessFigures[i].getPosition() == newPos) n = i;

                // move the piece
                sf::Vector2f p = oldPos - newPos;
                
                for (int k = 0; k < 10; k++) {
                    chessFigures[n].move(p.x/10, p.y/10); 
                    display_chess_board(window, chessBoard, chessFigures);
                }
                p = newPos - oldPos;
                for (int k = 0; k < 10; k++) {
                    chessFigures[n].move(p.x/10, p.y/10); 
                    display_chess_board(window, chessBoard, chessFigures);
                }
            }
        }

        if (isMove) chessFigures[n].setPosition(pos.x-dx,pos.y-dy);
        else {
            load_position(board, chessFigures);
            int counter = 0;
            for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) if (board[i][j]) counter++;
            if (counter < chessFigures.size()) chessFigures.pop_back();
        }

        // draw
        display_chess_board_with_avalibles(window, chessBoard, chessFigures, avalibles);
    }
}