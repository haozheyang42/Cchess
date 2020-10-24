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

void play_chess(int board[8][8], string turn, int DEPTH)
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
            computerMove = computer_move(board, DEPTH);
            
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

// welcome to my chess program
void introduction(string &turn, int &DEPTH)
{
    string introStr1 = "Welcome to Haozhe Yang's Chess game!\nHere are some special keys you can use:\n    1. Press space to look at what move the computer has made in case if you did not see it.\n    2. Press escape to surrender to Haozhe Yang's algorithm.\n\nPress right key to continue";
    string introStr2 = "Please choose who is going first: ";
    string introStr3 = "Please choose the diffculty of the AI: ";
    sf::Vector2f position; position.x = 50; position.y = 30;

    sf::Font font;
    font.loadFromFile("sansation.ttf");
    
    sf::Text text(introStr1, font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition({50, 30});

    sf::RectangleShape background;
    background.setSize({900, 200});
    background.setFillColor(sf::Color(209, 209, 209));

    sf::Text computerButton("computer", font);
    sf::RectangleShape computerButtonBackground;
    if (true) {
        computerButton.setPosition({100, 80});
        computerButton.setFillColor(sf::Color::Black);
        sf::FloatRect textBounds = computerButton.getGlobalBounds();
        sf::Vector2f recSize(textBounds.width+10, textBounds.height+10);
        computerButtonBackground.setSize(recSize);
        computerButtonBackground.setFillColor(sf::Color(209, 209, 209));
        computerButtonBackground.setPosition(textBounds.left, textBounds.top);
        computerButtonBackground.move({-5, -5});
        computerButtonBackground.setOutlineColor(sf::Color::Black);
        computerButtonBackground.setOutlineThickness(2);
    }

    sf::Text playerButton("player", font);
    sf::RectangleShape playerButtonBackground;
    if (true) {
        playerButton.setPosition({400, 83});
        playerButton.setFillColor(sf::Color::Black);
        sf::FloatRect textBounds = playerButton.getGlobalBounds();
        sf::Vector2f recSize(textBounds.width+10, textBounds.height+10);
        playerButtonBackground.setSize(recSize);
        playerButtonBackground.setFillColor(sf::Color(209, 209, 209));
        playerButtonBackground.setPosition(textBounds.left, textBounds.top);
        playerButtonBackground.move({-5, -5});
        playerButtonBackground.setOutlineColor(sf::Color::Black);
        playerButtonBackground.setOutlineThickness(2);
    }

    sf::Text easyButton("easy", font);
    sf::RectangleShape easyButtonBackground;
    if (true) {
        easyButton.setPosition({100, 80});
        easyButton.setFillColor(sf::Color::Black);
        sf::FloatRect textBounds = easyButton.getGlobalBounds();
        sf::Vector2f recSize(textBounds.width+10, textBounds.height+10);
        easyButtonBackground.setSize(recSize);
        easyButtonBackground.setFillColor(sf::Color(209, 209, 209));
        easyButtonBackground.setPosition(textBounds.left, textBounds.top);
        easyButtonBackground.move({-5, -5});
        easyButtonBackground.setOutlineColor(sf::Color::Black);
        easyButtonBackground.setOutlineThickness(2);
    }

    sf::Text mediumButton("medium", font);
    sf::RectangleShape mediumButtonBackground;
    if (true) {
        mediumButton.setPosition({300, 86});
        mediumButton.setFillColor(sf::Color::Black);
        sf::FloatRect textBounds = mediumButton.getGlobalBounds();
        sf::Vector2f recSize(textBounds.width+10, textBounds.height+10);
        mediumButtonBackground.setSize(recSize);
        mediumButtonBackground.setFillColor(sf::Color(209, 209, 209));
        mediumButtonBackground.setPosition(textBounds.left, textBounds.top);
        mediumButtonBackground.move({-5, -5});
        mediumButtonBackground.setOutlineColor(sf::Color::Black);
        mediumButtonBackground.setOutlineThickness(2);
    }

    sf::Text diffcultButton("diffcult", font);
    sf::RectangleShape diffcultButtonBackground;
    if (true) {
        diffcultButton.setPosition({550, 87});
        diffcultButton.setFillColor(sf::Color::Black);
        sf::FloatRect textBounds = diffcultButton.getGlobalBounds();
        sf::Vector2f recSize(textBounds.width+10, textBounds.height+10);
        diffcultButtonBackground.setSize(recSize);
        diffcultButtonBackground.setFillColor(sf::Color(209, 209, 209));
        diffcultButtonBackground.setPosition(textBounds.left, textBounds.top);
        diffcultButtonBackground.move({-5, -5});
        diffcultButtonBackground.setOutlineColor(sf::Color::Black);
        diffcultButtonBackground.setOutlineThickness(2);
    }

    int stage = 1;

    sf::RenderWindow window(sf::VideoMode(900, 200), "Welcome to Haozhe Yang's Chess Game!");
    window.setFramerateLimit(60);

    while(window.isOpen())
    {
        sf::Event event;
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                text.setString(introStr2);
                stage = 2;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && stage == 2) {
                if (computerButtonBackground.getGlobalBounds().contains(pos.x,pos.y)) {
                    turn = "player2";
                    text.setString(introStr3);
                    stage = 3;
                    this_thread::sleep_for (chrono::milliseconds(200));
                } if (playerButtonBackground.getGlobalBounds().contains(pos.x,pos.y)) {
                    turn = "player1";
                    text.setString(introStr3);
                    stage = 3;
                    this_thread::sleep_for (chrono::milliseconds(200));
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && stage == 3) {
                if (easyButtonBackground.getGlobalBounds().contains(pos.x,pos.y)) {
                    DEPTH = 0;
                    stage = 4;
                } if (mediumButtonBackground.getGlobalBounds().contains(pos.x,pos.y)) {
                    DEPTH = 1;
                    stage = 4;
                } if (diffcultButtonBackground.getGlobalBounds().contains(pos.x,pos.y)) {
                    DEPTH = 3;
                    stage = 4;
                }
            }

            if (stage == 4) window.close();
        }

        window.clear();
        window.draw(background);
        window.draw(text);
        if (stage == 2) {
            window.draw(computerButtonBackground);
            window.draw(computerButton);
            window.draw(playerButtonBackground);
            window.draw(playerButton);
        } if (stage == 3) {
            window.draw(easyButtonBackground);
            window.draw(easyButton);
            window.draw(mediumButtonBackground);
            window.draw(mediumButton);
            window.draw(diffcultButtonBackground);
            window.draw(diffcultButton);
        }
        window.display();
    }
}