#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "global.h"
#include "player.h"
#include "enemy.h"
#include "coin.h"

#include "game.h"


void Game::draw_map(sf::RenderWindow& window) {

    sf::RectangleShape cell;
    cell.setSize(sf::Vector2f(12, 12));
    cell.setFillColor(sf::Color(255, 255, 255));

    for (int y = 0; y < mapY; y++) {
        for (int x = 0; x < mapX; x++) {
            if (map[y * mapY + x] == 1) {
                float cx = float(x * mapS / 4 + 4);
                float cy = float(y * mapS / 4 + 4);
                cell.setPosition(sf::Vector2f(cx, cy));
                window.draw(cell);
            }
        }
    }
}

void Game::sky_floor_draw(sf::RenderWindow& window) {
    // sky
    sf::RectangleShape sky_floor;
    sky_floor.setFillColor(sf::Color(106, 206, 235));
    sky_floor.setSize(sf::Vector2f(960, 320));
    sky_floor.setPosition(sf::Vector2f(0, 0));

    window.draw(sky_floor);

    // floor
    sky_floor.setFillColor(sf::Color(211, 211, 211));
    sky_floor.setPosition(sf::Vector2f(0, 320));

    window.draw(sky_floor);
}


Game::Game() 
    : time_interval(0)
{

    player = new Player(240.0, 420.0, 0.0, 0.0, 0, 1);
    coin = new Coin();

    music.openFromFile("Boss Theme.wav");
    music.setVolume(2.5f);
    music.setLoop(true);

    for (int i = 0; i < 4; i++) {
        enemy[i] = new Enemy(1, 1, float(64 * (i + 1)), 128.0, 10.0, float(0.2 * (i + 1)));
    }

    warning.setFillColor(sf::Color(255, 0, 0, 100));
    warning.setSize(sf::Vector2f(960, 640));
    warning.setPosition(sf::Vector2f(0, 0));
}

Game::~Game() {
    delete player;
    delete coin;

    for (int i = 0; i < 4; i++) {
        delete enemy[i];
    }
}

void Game::play_music() {
    music.play();
}

void Game::run(sf::RenderWindow& window) {
    // draw floor
    sky_floor_draw(window);

    // player
    player->update_position();

    player->draw_rays(window);


    // enemies

    if (enemy_hit) {

        window.draw(warning);

        time_interval = clock.getElapsedTime().asSeconds();
        if (time_interval > 1) {
            enemy_hit = false;
            clock.restart();
        }
    }

    for (int i = 0; i < 4; i++) {

        enemy[i]->move_enemy(player->x, player->y);


        if (!enemy_hit) {
            if (enemy[i]->check_hit(player->x, player->y)) {
                player->health -= 5;
                if (player->health <= 0) { window.close(); }
                enemy_hit = true;

            }
        }

        if (enemy[i]->check_dead(player->x, player->y)) {
            enemy[i]->respawn(player->x, player->y);
            player->enemies_killed += 1;
            enemy_hit = false;
        }

        enemy[i]->draw(window, player->x, player->y, player->a);
    }

    // coin
    if (coin->check_hit(player->x, player->y)) {
        player->coins += 1;
        player->health += 10;
        if (player->health > 100) { player->health = 100; }
        coin->respawn(player->x, player->y);
    }
    coin->draw(window, player->x, player->y, player->a);

    // draw map
    draw_map(window);
    coin->draw_on_map(window);
    player->draw(window);

    // draw player stats
    player->draw_health(window);
    player->draw_stats(window);
}
