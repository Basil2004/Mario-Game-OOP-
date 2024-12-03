#include "Mario.h"
#include <iostream>

Mario::Mario() : Character(0.f, 0.f), platformY(0.f) {
    loadTextures();
    sprite.setTexture(idleTexture);
}

void Mario::loadTextures() {
    if (!idleTexture.loadFromFile("Images/MarioIdle.png") ||
        !movingTexture.loadFromFile("Images/MarioWalk.png") ||
        !jumpingTexture.loadFromFile("Images/MarioJump.png") ||
        !deadTexture.loadFromFile("Images/MarioDeath.png")) {
        std::cerr << "Error loading Mario textures!" << std::endl;
    }
}

void Mario::jump() {
    //Mario can only jump if he is alive and on the ground
    if (isAlive() && onGround) {
        velocity = -15.f;  // initial jump velocity
        onGround = false;  // Mario is now in the air
    }
}

void Mario::move(float x, float y) {
    this->x += x;  // updates horizontal position
    this->y += y;  // updates vertical position
    sprite.setPosition(this->x, this->y);  // update sprite position
}

void Mario::update(const World& world) {
    if (!isAlive()) return;

    // applying gravity if not on the ground
    if (!onGround) {
        velocity += 0.5f;  // gravity effect
    }
    y += velocity;

    // handling horizontal movement
    float horizontalMovement = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        horizontalMovement -= 5.f;  //move left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        horizontalMovement += 5.f;  //move right
    }
    x += horizontalMovement;  // applying horizontal movement

    // assuming Mario is in the air unless proven otherwise
    onGround = false;

    // to check collisions with blocks in the world :)
    const auto& map = world.getMap();
    for (const auto& row : map) {
        for (const auto& block : row) {
            if (block) {
                sf::FloatRect blockBounds = block->getBounds();
                sf::FloatRect marioBounds = getBounds();

                // check for vertical collision (hitting head on a block)
                if (marioBounds.intersects(blockBounds) && velocity < 0) {
                    y = blockBounds.top + blockBounds.height;  // adjusts position
                    velocity = 0.f;  // stops vertical movement
                }

                // checking for vertical collision -(landing on a block)
                if (marioBounds.intersects(blockBounds) && velocity > 0) {
                    y = blockBounds.top - marioBounds.height;  // adjust the position
                    velocity = 0.f;  // stop vertical movement
                    onGround = true;  // mark as on the ground
                }
            }
        }
    }

    // check if Mario has fallen below the screen
    if (y > world.bottomBoundary()) {
        // mark Mario as dead
        setHealth(0);
        velocity = 0.f;
    }

    // keep Mario within world boundaries
    if (this->x < world.leftBoundary()) {
        this->x = world.leftBoundary();
    } else if (this->x + getBounds().width > world.rightBoundary()) {
        this->x = world.rightBoundary() - getBounds().width;
    }
    if (this->y < world.topBoundary()) {
        this->y = world.topBoundary();
    }

    setPosition(x, y);
    setMovementTexture();
}


void Mario::setMovementTexture() {
    if (!isAlive()) {
        sprite.setTexture(deadTexture);  // mario is dead
    } else if (!onGround) {
        sprite.setTexture(jumpingTexture);  // mario is in the air (jumping/falling)
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        sprite.setTexture(movingTexture);  // mario is walking
    } else {
        sprite.setTexture(idleTexture);  // mario is idle
    }
}

void Mario::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Mario::getVelocity() const {
    return sf::Vector2f(0.f, velocity);  // return velocity as a vector (x = 0 as horizontal velocity is not tracked here)
}

void Mario::setVerticalVelocity(float vy) {
    velocity = vy;  // set the vertical velocity
}
