#pragma once

class Player {
private:
    sf::RectangleShape player;
    float speed;

    sf::Font font;

public:
    float x, y, dx, dy, a;
    int health;

    int coins;

    int enemies_killed;

    Player(float x, float y, float dx, float dy, float a, float speed);

    void update_position();

    void draw(sf::RenderWindow& window);

    void draw_rays(sf::RenderWindow& window);

    void draw_health(sf::RenderWindow& window);

    void draw_stats(sf::RenderWindow& window);
};

