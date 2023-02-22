#include <SFML/Graphics.hpp>
#include <random>
#include "global.h"
#include "player.h"
#include "enemy.h"
#include "coin.h"


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


void draw_map(sf::RenderWindow& window) {

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

void sky_floor_draw(sf::RenderWindow& window) {
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


int main()
{
    sf::RenderWindow window(sf::VideoMode(960, 640), "Raycasting");
    window.setFramerateLimit(60);

    Player* player = new Player(240.0, 420.0, 0.0, 0.0, 0, 1);

    Enemy* enemy[4];
    for (int i = 0; i < 4; i++) {
        enemy[i] = new Enemy(1, 1, float(64 * (i + 1)), 128.0, 10.0, float(0.2 * (i + 1)));
    }
	
	bool enemy_hit = false;

    Coin* coin = new Coin();
	
	sf::Clock clock;
	float time_interval;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }

        window.clear();

        // draw floor
        sky_floor_draw(window);

        // player
        player->update_position();

        player->draw_rays(window);


        // enemies
		
		if (enemy_hit) {
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

        window.display();
    }

    delete player;
    delete coin;
    
    for (int i = 0; i < 4; i++) {
        delete enemy[i];
    }

    return 0;
}