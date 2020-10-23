struct chess_move
{
    int special_move;
    // 0 for non special
    // 1 for castling
    // 2 for en passent
    // 3 for pawn becomes rook
    // 4 for pawn becomes knight
    // 5 for pawn becomes bishop
    // 6 for pawn becomes queen
    // 7 for pwan reached endline without becomming aything
    int start_row;
    int start_col;
    int end_row;
    int end_col;
};

bool player1castled = false, player2castled = false;

using namespace std;
#include "include"

int main()
{
    string turn = "player1";
    // // hide console
	// HWND hWnd = GetConsoleWindow();
    // ShowWindow(hWnd, SW_HIDE);

    sf::RenderWindow window(sf::VideoMode(900, 200),"Welcome to Haozhe Yang's chess game");

    sf::Font font;
    font.loadFromFile("sansation.ttf");

    std::string introStr1 = "Welcome to Haozhe Yang's Chess game!\nHere are some special keys you can use:\n    1. Press space to look at what move the computer has made in case if you did not see it.\n    2. Press escape to surrender to Haozhe Yang's algorithm.\n\nPress right key to continue";
    std::string introStr2 = "Please choose who is going first: ";
    std::string introStr3 = "Please choose the diffculty of the AI: ";
    sf::Vector2f position; position.x = 50; position.y = 30;
    
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
        mediumButton.setPosition({300, 80});
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

    sf::Text diffcultButton("easy", font);
    sf::RectangleShape diffcultButtonBackground;
    if (true) {
        diffcultButton.setPosition({500, 80});
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
                } if (playerButtonBackground.getGlobalBounds().contains(pos.x,pos.y)) {
                    turn = "player1";
                    text.setString(introStr3);
                    stage = 3;
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

    cout << turn << endl;
    cout << DEPTH;


    // play chess
	int board[8][8] =
      {-1,-2,-3,-4,-5,-3,-2,-1,
       -6,-6,-6,-6,-6,-6,-6,-6,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        6, 6, 6, 6, 6, 6, 6, 6,
        1, 2, 3, 4, 5, 3, 2, 1};
    
	play_chess(board, turn);

    return 0;
}