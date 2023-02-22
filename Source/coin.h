#pragma once

#include "sprite.h"

class Coin :public Sprite3D {

public:
	Coin();

	bool check_hit(float px, float py);

	void respawn(float px, float py);

	void draw_on_map(sf::RenderWindow& window);

};
