#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>

class Character {
public:
    Character(float startX, float startY);
    void setPosition(float x, float y);
    void move(float dx, float dy);
    int getHealth() const;
    void setHealth(int health);
    bool isAlive() const;
    virtual void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOnGround() const;
    void setOnGround(bool value);

protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float x, y;
    float velocity;
    bool onGround;

private:
    int health;
};

#endif
