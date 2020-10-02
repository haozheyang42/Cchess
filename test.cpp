#include <SFML/Graphics.hpp>

void move_bule_dot(sf::Sprite &blueDot) //, sf::RenderWindow window)
{
    for (int i = 0; i < 100; i++) {
        blueDot.setPosition(i, i);
        // window.clear();
        // window.draw(blueDot);
        // window.display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::Texture t3;
    t3.loadFromFile("avalible.png");
    sf::Sprite blueDot(t3);
    blueDot.setPosition(0, 0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                move_bule_dot(blueDot);
            }
        }

        window.clear();
        window.draw(blueDot);
        window.display();
    }

    return 0;
}