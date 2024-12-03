#ifndef MARIO_H
#define MARIO_H

#include "Character.h"
#include "Platform.h"
#include "Block.h"
#include <SFML/Graphics.hpp>

class Mario : public Character {
public:
    Mario();
    void jump();
    void update(const World& world);  // Updates Mario's position based on world collision logic
    void move(float dx, float dy);    // Moves Mario by dx, dy
    void draw(sf::RenderWindow& window);
    sf::Vector2f getVelocity() const;  // Getter for velocity
    void setVerticalVelocity(float vy);  // Setter for vertical velocity


private:
    sf::Texture idleTexture;
    sf::Texture movingTexture;
    sf::Texture jumpingTexture;
    sf::Texture deadTexture;

    void loadTextures();
    void setMovementTexture();

    int jumpCount = 0;
    const int max_jumps = 2;

    float platformY;  // Height of the ground or platform

    float velocity; // Vertical velocity    
};

#endif
