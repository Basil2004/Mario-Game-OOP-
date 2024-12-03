#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>

class Block {
public:
    enum Type {
        Brick, Question, Empty, Grass_Top, Pipe_Top_Left, Pipe_Top_Right, Pipe_Left, Pipe_Right
    };

    static const int TILE_SIZE;  

    Block(Type type, int x, int y);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;


private:
    sf::Sprite sprite;
    sf::Texture texture;
};

#endif
