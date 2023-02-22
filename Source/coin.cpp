#include <SFML/Graphics.hpp>
#include "coin.h"
#include "global.h"
#include <random>

Coin::Coin() : Sprite3D(2, 1, 420, 140, 10, sf::Color(255, 255, 0)) {}

bool Coin::check_hit(float px, float py) {
	if (px < x + 30 && px > x - 30 && py < y + 30 && py > y - 30) {
		return true;
	}
	else {
		return false;
	}
}

void Coin::respawn(float px, float py) {
	std::random_device rand;
	std::uniform_int_distribution<int> x(1, 7);
	std::uniform_int_distribution<int> y(1, 7);

	int coin_cell_x, coin_cell_y;

	coin_cell_x = x(rand); 
	coin_cell_y = y(rand);
	while (map[coin_cell_y * mapX + coin_cell_x] == 1) {
		coin_cell_x = x(rand);
		coin_cell_y = y(rand);
	}


	this->x = float(coin_cell_x * 64);
	this->y = float(coin_cell_y * 64);
}

void Coin::draw_on_map(sf::RenderWindow& window) {
	sf::RectangleShape sprite_rect;
	sprite_rect.setSize(sf::Vector2f(8, 8));
	sprite_rect.setPosition(sf::Vector2f(float(x / 4), float(y / 4)));
	sprite_rect.setFillColor(color);

	window.draw(sprite_rect);
}