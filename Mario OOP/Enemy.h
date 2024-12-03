#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Character.h"

class Enemy:public Character {
public:
    Enemy(float startX, float startY);  // constructor
    void update(float marioX, float screenLeft, float screenRight);  // update position and behavior
    void draw(sf::RenderWindow& window) const;  // render the enemy
    void die();  // marrk enemy as dead
    bool isDead() const;  // check if the enemy is dead
    sf::FloatRect getBounds() const;  // get enemy bounds for collision detection

private:
    sf::Texture aliveTexture;  // texture when the enemy is alive
    sf::Texture deadTexture;   // texture when the enemy is dead
    float speed;               // movement speed
    bool dead;                 // status of the enemy (alive or dead)
};

#endif
