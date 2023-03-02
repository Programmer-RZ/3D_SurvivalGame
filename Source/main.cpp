#include <SFML/Graphics.hpp>

#include "game.h"
#include "global.h"


int map[] = 
{
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 1, 0, 0, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

int depth[120];

float FixAng(float a) { if (a > 359) { a -= 360; } if (a < 0) { a += 360; } return a; }

float degToRad(float a) { return float(a * PI / 180.0); }


int main()
{
    sf::RenderWindow window(sf::VideoMode(960, 640), "Raycasting");
    window.setFramerateLimit(60);

    Game* game = new Game();

    game->play_music();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }

        window.clear();

        game->run(window);

        window.display();
    }

    delete game;

    return 0;
}