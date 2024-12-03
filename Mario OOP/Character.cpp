#include "Character.h"

Character::Character(float startX, float startY)
    : x(startX), y(startY), health(100), velocity(0.f), onGround(true) {
    sprite.setPosition(x, y);
}

void Character::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    sprite.setPosition(x, y);
}

void Character::move(float dx, float dy) {
    x += dx;
    y += dy;
    sprite.move(dx, dy);
}

int Character::getHealth() const {
    return health;
}

void Character::setHealth(int health) {
    this->health = health;
}

bool Character::isAlive() const {
    return health > 0;
}

void Character::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Character::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Character::isOnGround() const {
    return onGround;
}

void Character::setOnGround(bool value) {
    onGround = value;
}
