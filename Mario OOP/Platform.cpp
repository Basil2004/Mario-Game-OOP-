#include "Platform.h"
#include <fstream>
#include <iostream>

World::World(float gravity) : gravity(gravity), finishLine(0) {}
void World::loadMap(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    map.clear();  //clear any existing map data

    int row = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::unique_ptr<Block>> mapRow;
        for (int col = 0; col < line.length(); ++col) {
            char tile = line[col];
            Block::Type type;

            switch (tile) {
                case '1': type = Block::Brick; break;
                case '2': type = Block::Question; break;
                case '3': type = Block::Empty; break;
                case '4': type = Block::Grass_Top; break;
                case 'A': type = Block::Pipe_Top_Left; break;
                case 'B': type = Block::Pipe_Top_Right; break;
                case 'C': type = Block::Pipe_Left; break;
                case 'D': type = Block::Pipe_Right; break;
                case '-': mapRow.push_back(nullptr); continue;  
                default: continue;  
            }

            int x = col * Block::TILE_SIZE;
            int y = row * Block::TILE_SIZE;

            mapRow.emplace_back(std::make_unique<Block>(type, x, y));
        }
        map.push_back(std::move(mapRow));
        ++row;
    }

    file.close();
    std::cout << "Map loaded successfully with " << map.size() << " rows." << std::endl;
}


void World::draw(sf::RenderWindow& window) {
    sf::FloatRect viewBounds = window.getView().getViewport();
    sf::View currentView = window.getView();
    sf::FloatRect visibleArea(currentView.getCenter() - currentView.getSize() / 2.f, currentView.getSize());

    for (const auto& row : map) {
        for (const auto& block : row) {
            if (block && block->getBounds().intersects(visibleArea)) {
                block->draw(window);  // Draw only blocks within the camera's visible area
            }
        }
    }
}



int World::leftBoundary() const {
    return 0;
}

int World::rightBoundary() const {
    return map.empty() ? 0 : map[0].size() * Block::TILE_SIZE;
}

int World::topBoundary() const {
    return 0;
}

int World::bottomBoundary() const {
    return map.size() * Block::TILE_SIZE;
}

bool World::isSolidBlock(const int x, const int y) const {
    return map[x][y] != nullptr;
}

bool World::reachedFinish(const int x) const {
    return x > finishLine * Block::TILE_SIZE;
}

float World::getGravity() const {
    return gravity;
}

float World::getPlatformY(float marioX) const {
    //here finding the bottom-most platform Y for Mario's current X position
    int col = static_cast<int>(marioX) / Block::TILE_SIZE;

    for (int row = map.size() - 1; row >= 0; --row) {  //start from the bottom row
        if (col < map[row].size() && map[row][col]) {
            return (row + 1) * Block::TILE_SIZE;  //return the top Y position of the block
        }
    }

    return map.size() * Block::TILE_SIZE;  //default to the bottom of the screen
}

const std::vector<std::vector<std::unique_ptr<Block>>>& World::getMap() const {
    return map;
}