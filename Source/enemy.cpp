#include <SFML/Graphics.hpp>

#include "enemy.h"
#include "global.h"

Enemy::Enemy(int type, int state, float x, float y, float z, float speed)
	: Sprite3D(type, state, x, y, z, sf::Color(255, 0, 0)), speed(speed)
{}

bool Enemy::check_hit(float px, float py) {
	if (px < x + 30 && px > x - 30 && py < y + 30 && py > y - 30) {
		return true;
	}
	else {
		return false;
	}
}

bool Enemy::check_dead(float px, float py) {
	if ((px < x + 50 && px > x - 50 && py < y + 50 && py > y - 50) && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		state = 0;
		return true;
	}
	else {
		return false;
	}
}

void Enemy::move_enemy(float px, float py) {

	if (this->state == 1) {
		int spx = int(x) >> 6, spy = int(y) >> 6;
		int spx_add = (int(x) + 15) >> 6, spy_add = (int(y) + 15) >> 6;
		int spx_sub = (int(x) - 15) >> 6, spy_sub = (int(y) - 15) >> 6;

		if (x > px && map[spy * 8 + spx_sub] == 0) { x -= speed; }
		if (x < px && map[spy * 8 + spx_add] == 0) { x += speed; }
		if (y > py && map[spy_sub * 8 + spx] == 0) { y -= speed; }
		if (y < py && map[spy_add * 8 + spx] == 0) { y += speed; }
	}
}

void Enemy::respawn(float px, float py) {
	int enemy_cell_x, enemy_cell_y;
	int player_cell_x, player_cell_y;

	player_cell_x = int(px / 64) + 1;
	player_cell_y = int(py / 64) + 1;

	if (player_cell_x < 5) { enemy_cell_x = 6; }
	else { enemy_cell_x = 2; }

	if (player_cell_y < 5) { enemy_cell_y = 6; }
	else { enemy_cell_y = 2; }


	x = float(enemy_cell_x * 64);
	y = float(enemy_cell_y * 64);
	state = 1;
}