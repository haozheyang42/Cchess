int remaining = 32;
bool player1castled = false, player2castled = false;
#include "checkAI.hpp"

const int size = 56;
sf::Sprite f[32]; //figures

int board[8][8] =
    {-1,-2,-3,-4,-5,-3,-2,-1,
	 -6,-6,-6,-6,-6,-6,-6,-6,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0,
	  6, 6, 6, 6, 6, 6, 6, 6,
	  1, 2, 3, 4, 5, 3, 2, 1};

string toChessNote(sf::Vector2f p)
{
	string s = "";
	s += char(p.x/size+97);
	s += char(p.y/size+49);
	return s;
}

bool check(string str, string p)
{
	if (str.size() != 4) return false;
	if (str[0] < 'a' || str[0] > 'h' || str[2] < 'a' || str[2] > 'h') return false;
	if (str[1] < '1' || str[1] > '8' || str[3] < '1' || str[3] > '8') return false;
	int arr[2][2] = {{str[1]-'1', str[0]-'a'}, {str[3]-'1', str[2]-'a'}};
	if (board[arr[0][0]][arr[0][1]] == 0) return false;

	bool specialMove = false;
	if (move_is_valid(board, arr, p, specialMove)) {
		move_piece(board, arr, p, specialMove);
		return true;
	} return false;
}

void loadPosition()
{
	int k=0;
  	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
		{
			int n = board[i][j];
			if (!n) continue;
			int x = abs(n)-1;
			int y = n>0?1:0;
			f[k].setTextureRect( sf::IntRect(size*x,size*y,size,size) );
			f[k].setPosition(size*j,size*i);
			k++;
		}
}

void play_chess()
{
	sf::RenderWindow window(sf::VideoMode(453, 454), "Haozhe Yang's Chess Game!");

    sf::Texture t1,t2;
    t1.loadFromFile("figures.png");
	t2.loadFromFile("board.png");

    for(int i=0;i<32;i++) f[i].setTexture(t1);

    sf::Sprite s(t1);
    sf::Sprite sBoard(t2);

    loadPosition();

    bool isMove=false, gameEnded = false;
	float dx=0, dy=0;
    sf::Vector2f oldPos,newPos;
	string str;
    int n=0;

	string turn = "player1";
	fstream fio;
	fio.open("chess.log", ios::trunc | ios::in | ios::out);
	fio.close();

    while (window.isOpen())
    {
   	    sf::Vector2i pos = sf::Mouse::getPosition(window);

        sf::Event e;
        while (window.pollEvent(e))
        {
			if (e.type == sf::Event::Closed)
                window.close();

			bool clickedCorrectly = true;
			/////drag and drop///////
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
				for(int i=0;i<remaining;i++)
					if (f[i].getGlobalBounds().contains(pos.x,pos.y))
					{
						oldPos  =  f[i].getPosition();
						string st = toChessNote(sf::Vector2f(oldPos.x, oldPos.y));
						if (turn == "player1" && board[st[1]-'1'][st[0]-'a'] > 0) {
							isMove=true; n=i;
							dx=pos.x - f[i].getPosition().x;
							dy=pos.y - f[i].getPosition().y;
						} if (turn == "player2" && board[st[1]-'1'][st[0]-'a'] < 0) {
							isMove=true; n=i;
							dx=pos.x - f[i].getPosition().x;
							dy=pos.y - f[i].getPosition().y;
						}
					}
				clickedCorrectly = isMove;
			}

            if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
			{
				isMove=false;
				sf::Vector2f p = f[n].getPosition() + sf::Vector2f(size/2,size/2);
				newPos = sf::Vector2f( size*int(p.x/size), size*int(p.y/size) );
				str = toChessNote(oldPos)+toChessNote(newPos);
				if (check(str, turn) && clickedCorrectly) {
					if (turn == "player1") turn = "player2";
					else turn = "player1";
					if (someone_won(board, turn)) gameEnded = true;
				}
			}
		}

		if (isMove) f[n].setPosition(pos.x-dx,pos.y-dy);
		else loadPosition();

		////// draw  ///////
		window.clear();
		window.draw(sBoard);
		for(int i=0;i<remaining;i++) window.draw(f[i]);
		window.display();

		while (gameEnded && window.pollEvent(e)) {
			if (e.key.code == sf::Keyboard::Space) window.close();
			if (e.type == sf::Event::Closed) window.close();
		}
	}
}

int main()
{
	play_chess();
	cout << "press enter to continue";
	string turn;
	getline(cin, turn);
}