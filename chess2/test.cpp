#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;
    
const int size = 56;
Sprite fi[4];

int f()
{
        RenderWindow window(VideoMode(size*4, size), "Choose your piece");
        Texture t3;
        t3.loadFromFile("figures.png");
        for(int i=0;i<4;i++) fi[i].setTexture(t3);
        fi[0].setTextureRect( IntRect(0,size,size,size) );
        fi[0].setPosition(0,0);
        fi[1].setTextureRect( IntRect(size,size,size,size) );
        fi[1].setPosition(size,0);
        fi[2].setTextureRect( IntRect(2*size,size,size,size) );
        fi[2].setPosition(2*size,0);
        fi[3].setTextureRect( IntRect(3*size,size,size,size) );
        fi[3].setPosition(3*size,0);
        int n;
        while (window.isOpen())
        {
            Vector2i pos = Mouse::getPosition(window);
            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
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
    return n;
}

int main()
{
    std::cout << f() << std::endl;
    std::string k;
    getline(std::cin, k);
    return 0;
}