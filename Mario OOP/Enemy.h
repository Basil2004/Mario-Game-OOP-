#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Character.h"

class Enemy : public Character {
public:
    Enemy(float startX, float startY);  // Constructor
    void update(float marioX, float screenLeft, float screenRight);  // Update position and behavior
    void draw(sf::RenderWindow& window) const;  // Render the enemy
    void die();  // Mark enemy as dead
    bool isDead() const;  // Check if the enemy is dead
    sf::FloatRect getBounds() const;  // Get enemy bounds for collision detection

private:
    sf::Texture aliveTexture;  // Texture when the enemy is alive
    sf::Texture deadTexture;   // Texture when the enemy is dead
    float speed;               // Movement speed
    bool dead;                 // Status of the enemy (alive or dead)
};

#endif
