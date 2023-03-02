#pragma once

#include <SFML/Audio.hpp>
#include "player.h"
#include "enemy.h"
#include "coin.h"

class Game {
private:

    Player* player;

    Enemy* enemy[4];

    bool enemy_hit = false;

    sf::RectangleShape warning;

    Coin* coin;

    sf::Clock clock;
    float time_interval;

    // music
    sf::Music music;

    // private functions
    void draw_map(sf::RenderWindow& window);

    void sky_floor_draw(sf::RenderWindow& window);

public:
    Game();

    ~Game();

    void play_music();

    void run(sf::RenderWindow& window);

};
