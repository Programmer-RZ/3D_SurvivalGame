#pragma once

class Sprite3D {
protected:
	int type;
	float x, y, z;

	sf::Color color;

public:
	int state;

	Sprite3D(int type, int state, float x, float y, float z, sf::Color color);

	void draw(sf::RenderWindow& window, float px, float py, float pa);
};