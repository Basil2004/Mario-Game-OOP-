#include "Enemy.h"
#include <iostream>

Enemy::Enemy(float startX, float startY)
    : Character(startX, startY), speed(1.5f), dead(false) {
    if (!aliveTexture.loadFromFile("Images/GoombaWalk.png") ||
        !deadTexture.loadFromFile("Images/GoombaDeath.png")) {
        std::cerr << "Error loading enemy textures!" << std::endl;
    }

    sprite.setTexture(aliveTexture);  // setting initial texture
    sprite.setPosition(x, y);         // setting initial position
}

void Enemy::update(float marioX, float screenLeft, float screenRight) {
    if (dead) return;  // do nothing if the enemy is dead

    // enemy moves back and forth
    x += speed;

    // reverse direction if at screen edges
    if (x < screenLeft || x + sprite.getGlobalBounds().width > screenRight) {
        speed = -speed;  // reverses direction
    }

    sprite.setPosition(x, y);
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Enemy::die() {
    dead = true;
    sprite.setTexture(deadTexture);  // change texture to "dead" state
}

bool Enemy::isDead() const {
    return dead;
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}
