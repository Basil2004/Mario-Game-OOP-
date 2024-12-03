#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Block.h"

class World {
public:
    World(float gravity);
    void loadMap(std::string filename);
    void draw(sf::RenderWindow& window);
    int leftBoundary() const;
    int rightBoundary() const;
    int topBoundary() const;
    int bottomBoundary() const;
    bool isSolidBlock(const int x, const int y) const;
    bool reachedFinish(const int x) const;
    float getGravity() const;
    float getPlatformY(float marioX) const;
    const std::vector<std::vector<std::unique_ptr<Block>>>& getMap() const;



private:
    std::vector<std::vector<std::unique_ptr<Block>>> map;
    float gravity;
    int finishLine;
};

#endif
