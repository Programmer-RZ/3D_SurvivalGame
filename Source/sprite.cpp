#include <SFML/Graphics.hpp>
#include <math.h>
#include "global.h"
#include "sprite.h"


Sprite3D::Sprite3D(int type, int state, float x, float y, float z, sf::Color color) 
	: type(type), state(state), x(x), y(y), z(z), color(color)
	{}

void Sprite3D::draw(sf::RenderWindow& window, float px, float py, float pa) {
	if (state == 1) {
		// draw on 2d map
		/*
		sf::RectangleShape sprite_rect;
		sprite_rect.setSize(sf::Vector2f(8, 8));
		sprite_rect.setPosition(sf::Vector2f(float(x / 4), float(y / 4)));
		sprite_rect.setFillColor(color);

		window.draw(sprite_rect);
		*/


		float sx = x - px;
		float sy = y - py;
		float sz = z;

		float CS = cos(degToRad(pa)), SN = sin(degToRad(pa));
		float a = sy * CS + sx * SN;
		float b = sx * CS - sy * SN;

		sx = a;
		sy = b;

		sx = (sx * 108 / sy) + 60;
		sy = (sz * 108 / sy) + 40;

		int scale = int(32 * 40 / b);

		for (int x_count = int(sx - scale / 2); x_count < sx + scale / 2; x_count++) {

			if (state == 1 && sx > 0 && sx < 120 && b < depth[x_count]) {

				sf::RectangleShape line;
				line.setFillColor(color);
				line.setSize(sf::Vector2f(8, float(16 * scale / 2)));
				line.setPosition(sf::Vector2f(float(x_count * 8), sy * 8));

				window.draw(line);
			}
		}
	}
}