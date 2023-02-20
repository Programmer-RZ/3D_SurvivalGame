#include <SFML/Graphics.hpp>
#include <math.h>
#include "string"
#include "global.h"
#include "player.h"


Player::Player(float x, float y, float dx, float dy, float a, float speed)
    :x(x), y(y), dx(dx), dy(dy), a(a), speed(speed), health(100), coins(0), enemies_killed(0)
{
    player.setSize(sf::Vector2f(8, 8));
    
    this->dx = cos(this->a) * 5;
    this->dy = sin(this->a) * 5;

    font.loadFromFile("Fonts/Montserrat-Regular.ttf");
}

void Player::update_position() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        a += float(speed);
        a = FixAng(a);
        dx = cos(degToRad(a));
        dy = -sin(degToRad(a));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        a -= float(speed);
        a = FixAng(a);
        dx = cos(degToRad(a));
        dy = -sin(degToRad(a));
    }

    // check collision
    int x_offset = 0; if (dx < 0) { x_offset = -20; } else { x_offset = 20; }
    int y_offset = 0; if (dy < 0) { y_offset = -20; } else { y_offset = 20; }
    int ipx = int(x / 64.0), ipx_add_xo = int((x + x_offset) / 64.0), ipx_sub_xo = int((x - x_offset) / 64.0);
    int ipy = int(y / 64.0), ipy_add_yo = int((y + y_offset) / 64.0), ipy_sub_yo = int((y - y_offset) / 64.0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (map[ipy * mapX + ipx_add_xo] != 1) { x += dx * speed; }
        if (map[ipy_add_yo * mapX + ipx] != 1) { y += dy * speed; }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (map[ipy * mapX + ipx_sub_xo] != 1) { x -= dx * speed; }
        if (map[ipy_sub_yo * mapX + ipx] != 1) { y -= dy * speed; }
    }

    player.setPosition(sf::Vector2f(x, y));
}

void Player::draw(sf::RenderWindow& window) {

    sf::RectangleShape scaled_p = player;
    scaled_p.setPosition(sf::Vector2f(x / 4, y / 4));
    scaled_p.setFillColor(sf::Color(0, 200, 0));
    window.draw(scaled_p);
}

void Player::draw_rays(sf::RenderWindow& window) {
    int r, mx, my, mp, dof, side; 
    float vx, vy, rx, ry, ra, xo, yo, disV, disH, disT = 0.0;

    sf::Color wallcolor;

    ra = FixAng(a + 30);                                                              //ray set back 30 degrees

    for (r = 0; r < 120; r++)
    {
        //---Vertical--- 
        dof = 0; side = 0; disV = 100000;
        float Tan = tan(degToRad(ra));
        if (cos(degToRad(ra)) > 0.001) { rx = float((((int)x >> 6) << 6) + 64); ry = (x - rx) * Tan + y; xo = 64; yo = -xo * Tan; }//looking left
        else if (cos(degToRad(ra)) < -0.001) { rx = float(((static_cast<unsigned __int64>(x) >> 6) << 6) - 0.0001); ry = (x - rx) * Tan + y; xo = -64; yo = -xo * Tan; }//looking right
        else { rx = x; ry = y; dof = 8; }                                                  //looking up or down. no hit  

        while (dof < 8)
        {
            mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) { dof = 8; disV = cos(degToRad(ra)) * (rx - x) - sin(degToRad(ra)) * (ry - y); }//hit         
            else { rx += xo; ry += yo; dof += 1; }                                               //check next horizontal
        }
        vx = rx; vy = ry;

        //---Horizontal---
        dof = 0; disH = 100000;
        Tan = float(1.0 / Tan);
        if (sin(degToRad(ra)) > 0.001) { ry = float(((static_cast<unsigned __int64>(y) >> 6) << 6) - 0.0001); rx = (y - ry) * Tan + x; yo = -64; xo = -yo * Tan; }//looking up 
        else if (sin(degToRad(ra)) < -0.001) { ry = float((((int)y >> 6) << 6) + 64);      rx = (y - ry) * Tan + x; yo = 64; xo = -yo * Tan; }//looking down
        else { rx = x; ry = y; dof = 8; }                                                   //looking straight left or right

        while (dof < 8)
        {
            mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) { dof = 8; disH = cos(degToRad(ra)) * (rx - x) - sin(degToRad(ra)) * (ry - y); }//hit         
            else { rx += xo; ry += yo; dof += 1; }                                               //check next horizontal
        }

        if (disV < disH) { rx = vx; ry = vy; disT = disV; wallcolor = sf::Color(140, 140, 140); }
        if (disV > disH) { disT = disH; wallcolor = sf::Color(90, 90, 90); }

        float ca = FixAng(a - ra); disT = disT * cos(degToRad(ca));                            //fix fisheye 
        float lineH = (mapS * 640) / (disT); if (lineH > 640) { lineH = 640; }                     //line height and limit
        float lineOff = float(320 - (int(lineH) >> 1)); //line offset

        depth[r] = int(disT);

        sf::RectangleShape wall;
        wall.setFillColor(wallcolor);

        wall.setSize(sf::Vector2f(8, lineH));

        wall.setPosition(sf::Vector2f(float(r * 8), lineOff));

        window.draw(wall);

        ra = FixAng(float(ra - 0.5));
    }

}

void Player::draw_health(sf::RenderWindow& window) {
    sf::Color health_color(0, 255, 0);
    if (this->health < 70 && this->health >= 40) {
        health_color = sf::Color(255, 255, 0);
    }
    else if (this->health < 40 && this->health >= 20) {
        health_color = sf::Color(255, 165, 0);
    }
    else if (this->health < 20) {
        health_color = sf::Color(255, 0, 0);
    }

    sf::RectangleShape border;
    border.setFillColor(sf::Color(0, 0, 0));
    border.setSize(sf::Vector2f(210, 30));
    border.setPosition(sf::Vector2f(695, 25));

    sf::RectangleShape health;
    health.setFillColor(health_color);
    health.setSize(sf::Vector2f(float(this->health) * 2, 20));
    health.setPosition(sf::Vector2f(700, 30));

    window.draw(border);
    window.draw(health);
}

void Player::draw_stats(sf::RenderWindow& window) {


    // image
    sf::RectangleShape rect;

    // coins
    sf::Text coin_text;
    coin_text.setFont(this->font);
    coin_text.setString(std::to_string(coins));
    coin_text.setFillColor(sf::Color(255, 255, 255));
    coin_text.setOutlineColor(sf::Color(0, 0, 0));
    coin_text.setOutlineThickness(5.0f);
    coin_text.setPosition(sf::Vector2f(80, 200));

    rect.setSize(sf::Vector2f(40, 40));
    rect.setFillColor(sf::Color(255, 255, 0));
    rect.setPosition(sf::Vector2f(20, 200));

    window.draw(rect);
    window.draw(coin_text);

    // enemy count

    sf::Text enemy_count;
    enemy_count.setFont(this->font);
    enemy_count.setString(std::to_string(enemies_killed));
    enemy_count.setFillColor(sf::Color(255, 255, 255));
    enemy_count.setOutlineColor(sf::Color(0, 0, 0));
    enemy_count.setOutlineThickness(5.0f);
    enemy_count.setPosition(sf::Vector2f(80, 260));

    rect.setSize(sf::Vector2f(40, 40));
    rect.setFillColor(sf::Color(255, 0, 0));
    rect.setPosition(sf::Vector2f(20, 260));

    window.draw(rect);
    window.draw(enemy_count);

}