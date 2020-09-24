int remaining = 32;
bool player1castled = false, player2castled = false;
using namespace std;
#include "include"

const int sizeOfSprite = 56;
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
	s += char(p.x/sizeOfSprite+97);
	s += char(p.y/sizeOfSprite+49);
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


void surrender(string &turn)
{
	int ret = MessageBoxA(NULL, "Do you want to surrender to Haozhe Yang's algorithm?", "Surrender?", MB_YESNO);
    if (ret == 6) {
		turn = "nobody";
		losted();
	}
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
			f[k].setTextureRect( sf::IntRect(sizeOfSprite*x,sizeOfSprite*y,sizeOfSprite,sizeOfSprite) );
			f[k].setPosition(sizeOfSprite*j,sizeOfSprite*i);
			k++;
		}
}

void play_chess()
{
	sf::RenderWindow window(sf::VideoMode(453, 454), "Haozhe(Stephen) Yang's Chess Game!");

    sf::Texture t1,t2,t3;
    t1.loadFromFile("figures.png");
	t2.loadFromFile("board.png");
	t3.loadFromFile("avalible.png");

    for(int i=0;i<32;i++) f[i].setTexture(t1);
    sf::Sprite sBoard(t2);

    loadPosition();

    bool isMove=false;
	float dx=0, dy=0;
    sf::Vector2f oldPos,newPos;
	string str;
    int n=0;
	int Cmove;
	int pieceLastMoved;

	string turn = "player1";
	fstream fio;
	fio.open("chess.log", ios::trunc | ios::in | ios::out);
	fio.close();
	bool clickedCorrectly = true;

	vector<sf::Sprite> avalibles;

    window.setFramerateLimit(60);
	while (window.isOpen())
    {
   	    sf::Vector2i pos = sf::Mouse::getPosition(window);
        sf::Event e;

		// computer makes a move
		if (turn == "player2") {
			bool spec = false;
			Cmove = computer_move(board, spec);
			int arrr[2][2] = {(Cmove/1000)%10, (Cmove/100)%10, (Cmove/10)%10, Cmove%10};

			oldPos = sf::Vector2f(arrr[0][1]*sizeOfSprite,arrr[0][0]*sizeOfSprite);
         	newPos = sf::Vector2f(arrr[1][1]*sizeOfSprite,arrr[1][0]*sizeOfSprite);
			for(int i=0;i<remaining;i++) if (f[i].getPosition()==oldPos) n=i;
			pieceLastMoved = n;
			sf::Vector2f p = newPos - oldPos;
			for(int k=0;k<10;k++)
			{
				f[n].move(p.x/10, p.y/10); 
				window.draw(sBoard);
				for(int i=0;i<remaining;i++) window.draw(f[i]); 
				window.draw(f[n]);
				window.display();
			}
			move_piece(board, arrr, turn, spec);
			turn = "player1";
			if (someone_won(board, turn)) {
				game_ended(board, turn);
				turn = "nobody";
				continue;
			}
		}

        while (turn != "nobody" && window.pollEvent(e)) {
			// last move
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				int arrr[2][2] = {(Cmove/1000)%10, (Cmove/100)%10, (Cmove/10)%10, Cmove%10};
				oldPos = sf::Vector2f(arrr[0][1]*sizeOfSprite,arrr[0][0]*sizeOfSprite);
				newPos = sf::Vector2f(arrr[1][1]*sizeOfSprite,arrr[1][0]*sizeOfSprite);
				for(int i=0;i<remaining;i++) if (f[i].getPosition()==newPos) n=i;
				sf::Vector2f p = oldPos - newPos;
				
				p = oldPos - newPos;
				long double length = 7*sqrt(pow(oldPos.x-newPos.x, 2)+pow(oldPos.y-newPos.y, 2))/sizeOfSprite;
				for(int k=0;k<length;k++)
				{
					f[n].move(p.x/length, p.y/length); 
					window.draw(sBoard);
					for(int i=0;i<remaining;i++) window.draw(f[i]); 
					window.draw(f[n]);
					window.display();
				}
				p = newPos - oldPos;
				for(int k=0;k<length;k++)
				{
					f[n].move(p.x/length, p.y/length); 
					window.draw(sBoard);
					for(int i=0;i<remaining;i++) window.draw(f[i]); 
					window.draw(f[n]);
					window.display();
				}
			}
			
			// drag
			if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
				for(int i=0;i<remaining;i++)
					if (f[i].getGlobalBounds().contains(pos.x,pos.y))
					{
						oldPos  =  f[i].getPosition();
						string st = toChessNote(sf::Vector2f(oldPos.x, oldPos.y));

						int arr2[2] = {st[1]-'1', st[0]-'a'};
						vector<int> v;
						for (int i = 0; i < 8; i++)
							for (int j = 0; j < 8; j++) {
								bool spec = false;
								int arrr[2][2] = {arr2[0], arr2[1], i, j};
								if (move_is_valid(board, arrr, "player1", spec))
									v.push_back(i*10+j);
							}
						for (int i: v) {
							sf::Sprite tmp;
							tmp.setTexture(t3);
							tmp.setPosition(56*(i%10), 56*(i/10));
							avalibles.push_back(tmp);
						}
						for (sf::Sprite i: avalibles) window.draw(i);

						if (turn == "player1" && board[st[1]-'1'][st[0]-'a'] > 0) {
							isMove=true; n=i;
							dx=pos.x - f[i].getPosition().x;
							dy=pos.y - f[i].getPosition().y;
						}
					}
				clickedCorrectly = isMove;
			}

			// drop
            if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
				avalibles.clear();
				isMove=false;
				sf::Vector2f p = f[n].getPosition() + sf::Vector2f(sizeOfSprite/2,sizeOfSprite/2);
				newPos = sf::Vector2f( sizeOfSprite*int(p.x/sizeOfSprite), sizeOfSprite*int(p.y/sizeOfSprite) );
				str = toChessNote(oldPos)+toChessNote(newPos);
				if (check(str, turn) && clickedCorrectly) {
					turn = "player2";
					if (someone_won(board, turn)) {
						game_ended(board, turn);
						turn = "nobody";
						continue;
					}
				}
			}

			// surrender
			if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				surrender(turn);
				continue;
			}
		}

		while (turn == "nobody" && window.pollEvent(e)) {
			// close
			if (e.type == sf::Event::Closed) window.close();

			// last move
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				int arrr[2][2] = {(Cmove/1000)%10, (Cmove/100)%10, (Cmove/10)%10, Cmove%10};
				oldPos = sf::Vector2f(arrr[0][1]*sizeOfSprite,arrr[0][0]*sizeOfSprite);
				newPos = sf::Vector2f(arrr[1][1]*sizeOfSprite,arrr[1][0]*sizeOfSprite);
				for(int i=0;i<remaining;i++) if (f[i].getPosition()==newPos) n=i;
				sf::Vector2f p = oldPos - newPos;
				
				p = oldPos - newPos;
				long double length = sqrt(pow(oldPos.x-newPos.x, 2)+pow(oldPos.y-newPos.y, 2))/sizeOfSprite;
				for(int k=0;k<length;k++)
				{
					f[n].move(p.x/length, p.y/length); 
					window.draw(sBoard);
					for(int i=0;i<remaining;i++) window.draw(f[i]); 
					window.draw(f[n]);
					window.display();
				}
				p = newPos - oldPos;
				for(int k=0;k<length;k++)
				{
					f[n].move(p.x/length, p.y/length); 
					window.draw(sBoard);
					for(int i=0;i<remaining;i++) window.draw(f[i]); 
					window.draw(f[n]);
					window.display();
				}	
			}
		}

		if (isMove) f[n].setPosition(pos.x-dx,pos.y-dy);
		else loadPosition();

		////// draw  ///////
		window.clear();
		window.draw(sBoard);
		for(sf::Sprite i: avalibles) window.draw(i);
		for(int i=0;i<remaining;i++) window.draw(f[i]);
		window.display();
	}
}

int main()
{
	play_chess();
}