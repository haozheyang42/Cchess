// struct chess_move
// {
//     int special_move;
//     // 0 for non special
//     // 1 for castling
//     // 2 for en passent
//     // 3 for pawn becomes rook
//     // 4 for pawn becomes knight
//     // 5 for pawn becomes bishop
//     // 6 for pawn becomes queen
//     // 7 for pwan reached endline without becomming aything
//     int start_row;
//     int start_col;
//     int end_row;
//     int end_col;
// };

// bool player1castled = false, player2castled = false;

// using namespace std;
// #include "include"

// int main()
// {
//     // hide console
// 	HWND hWnd = GetConsoleWindow();
//     ShowWindow(hWnd, SW_HIDE);

//     sf::RenderWindow window(sf::VideoMode(1000, 1000), "Welcome to Haozhe Yang's Chess Game!");

//     sf::Font font;
//     font.loadFromFile("sansation.tff");

//     sf::Text text;
//     text.setFont(font);
//     text.setString("abcd");
//     text.setPosition(500, 500);
//     text.setCharacterSize(100);
//     text.setFillColor(sf::Color::White);
//     // text.setStyle(sf::Text::Bold | sf::Text::Underlined);

//     window.setFramerateLimit(60);
//     while (window.isOpen()) {
//         sf::Vector2i pos = sf::Mouse::getPosition(window);
//         sf::Event e;

//         while (window.pollEvent(e)) {
//             if (e.type == sf::Event::Closed) window.close();
//         }
//         window.clear();
//         window.draw(text);
//         window.display();
//     }



//     // play chess
// 	int board[8][8] =
//       {-1,-2,-3,-4,-5,-3,-2,-1,
//        -6,-6,-6,-6,-6,-6,-6,-6,
//         0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 0,
//         6, 6, 6, 6, 6, 6, 6, 6,
//         1, 2, 3, 4, 5, 3, 2, 1};

//     // int board[8][8] =
//     //   { 0, 0,-1, 0, 0, 0, 0, 0,
//     //     0, 6, 0, 0,-5, 0, 0,-6,
//     //     0, 0, 0,-6, 0,-6, 0, 0,
//     //     0, 0, 0, 0, 0,-2, 0, 6,
//     //     0, 0, 0, 0, 3, 0, 0, 0,
//     //     0, 0, 0, 1, 0, 0, 0, 0,
//     //    -1, 3, 5, 0, 0, 6, 0, 0,
//     //     0, 0, 0, 0, 0, 0, 0, 0};
//     string turn = "player1";
// 	// play_chess(board, turn);
// }

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{

    sf::RenderWindow window(sf::VideoMode(900,200),"TEXT");

    sf::Font font;
    font.loadFromFile("sansation.ttf");

    std::string introStr1 = "Welcome to Haozhe Yang's Chess game!\nHere are some special keys you can use:\n    1. Press space to look at what move the computer has made in case if you did not see it.\n    2. Press escape to surrender to Haozhe Yang's algorithm.\nPress left key to continue";
    std::string introStr2 = "Please choose who is going first: ";
    std::string introStr3 = "Please choose the diffculty of the AI: ";
    sf::Vector2f position3; position3.x = 50; position3.y = 30;
    
    sf::Text text(introStr1, font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(position3);


    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}