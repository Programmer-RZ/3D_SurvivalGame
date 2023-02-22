#pragma once

#include "sprite.h"

class Enemy :public Sprite3D {
private:
	float speed;
public:
	Enemy(int type, int state, float x, float y, float z, float speed);

	bool check_hit(float px, float py);

	bool check_dead(float px, float py);

	void move_enemy(float px, float py);

	void respawn(float px, float py);
};
