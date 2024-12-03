#include "Block.h"
#include <iostream>
using namespace std;
const int Block::TILE_SIZE = 32;  // this matches block size in pixels

Block::Block(Type type, int x, int y) {
    std::string texturePath;
    switch (type) {
        case Brick: texturePath = "Images/block.png"; break;
        case Question: texturePath = "Images/block.png"; break;
        case Empty: texturePath = "Images/block.png"; break;
        case Grass_Top: texturePath = "Images/block.png"; break;
        case Pipe_Top_Left: texturePath = "Images/block.png"; break;
        case Pipe_Top_Right: texturePath = "Images/block.png"; break;
        case Pipe_Left: texturePath = "Images/block.png"; break;
        case Pipe_Right: texturePath = "Images/block.png"; break;
    }

    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << "\n";
    }

    sprite.setTexture(texture);

    // scaling the sprite to fit TILE_SIZE
    sf::Vector2u textureSize = texture.getSize();
    float scaleX = static_cast<float>(TILE_SIZE) / textureSize.x;
    float scaleY = static_cast<float>(TILE_SIZE) / textureSize.y;
    sprite.setScale(scaleX, scaleY);

    sprite.setPosition(x, y);
}

void Block::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Block::getBounds() const {
    return sprite.getGlobalBounds();
}
