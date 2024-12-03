#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Mario.h"
#include "Platform.h"
#include "Enemy.h" 
#include <iostream>
#include <vector>  
#include <cstdlib> 
#include <ctime>   

std::vector<Enemy> enemies;

// Enemy spawn function
void spawnEnemy(float screenLeft, float screenRight) {
    float randomX = screenLeft + rand() % static_cast<int>(screenRight - screenLeft - 32);  // Random X coordinate
    float platformY = 500.f;  
    float enemyY = platformY - 32; 
    enemies.emplace_back(randomX, enemyY); 
}

int main() {
    const int windowWidth = 800;  // Width of the window in pixels
    const int windowHeight = 600; // Height of the window in pixels

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Super Mario OOP");
    window.setFramerateLimit(60);

    sf::View camera(sf::FloatRect(0.f, 0.f, windowWidth, windowHeight));

    World world(0.5f);
    world.loadMap("Images/Level-1.txt");

    // Create Mario
    Mario mario;
    mario.setPosition(windowWidth / 2.f - mario.getBounds().width / 2.f,
                      windowHeight / 2.f - mario.getBounds().height / 2.f);

    // Start screen setup
    sf::Texture startScreenTexture;
    if (!startScreenTexture.loadFromFile("Images/start.jpg")) {
        std::cerr << "Error: Could not load start screen image!\n";
        return -1;
    }
    sf::Sprite startScreenSprite;
    startScreenSprite.setTexture(startScreenTexture);

    sf::Vector2u startTextureSize = startScreenTexture.getSize();
    float startScaleX = static_cast<float>(windowWidth) / startTextureSize.x;
    float startScaleY = static_cast<float>(windowHeight) / startTextureSize.y;
    startScreenSprite.setScale(startScaleX, startScaleY);

    // End game screen setup
    sf::Texture endGameTexture;
    if (!endGameTexture.loadFromFile("Images/gameover.jpeg")) {
        std::cerr << "Error: Could not load end game image!\n";
        return -1;
    }
    sf::Sprite endGameSprite;
    endGameSprite.setTexture(endGameTexture);

    sf::Vector2u endTextureSize = endGameTexture.getSize();
    float endScaleX = static_cast<float>(windowWidth) / endTextureSize.x;
    float endScaleY = static_cast<float>(windowHeight) / endTextureSize.y;
    endGameSprite.setScale(endScaleX, endScaleY);

    // Music setup
    sf::Music startMusic;
    if (!startMusic.openFromFile("Super Mario Sounds/27. Main Theme.mp3")) {
        std::cerr << "Error: Could not load start screen music!\n";
        return -1;
    }
    sf::Music gameMusic;
    if (!gameMusic.openFromFile("Super Mario Sounds/27. Main Theme.mp3")) {
        std::cerr << "Error: Could not load game music!\n";
        return -1;
    }
    sf::Music endMusic;
    if (!endMusic.openFromFile("Super Mario Sounds/Game Over.wav")) {
        std::cerr << "Error: Could not load end game music!\n";
        return -1;
    }
    sf::SoundBuffer transitionBuffer;
    if (!transitionBuffer.loadFromFile("Super Mario Sounds/Level Start.wav")) {
        std::cerr << "Error: Could not load transition sound!\n";
        return -1;
    }
    sf::Sound transitionSound;
    transitionSound.setBuffer(transitionBuffer);

    // Start playing the start screen music
    startMusic.setLoop(true);  // Loop the start music
    startMusic.play();

    bool gameStarted = false;
    bool gameEnded = false;

    // Seed the random number generator
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const size_t maxEnemies = 5;  // Limit the number of enemies on screen

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (!gameStarted && event.key.code == sf::Keyboard::Space) {
                    gameStarted = true;

                    // Stop the start music and play the transition sound
                    startMusic.stop();
                    transitionSound.play();

                    // Start the game music after a short delay for the transition
                    sf::sleep(sf::milliseconds(500));
                    gameMusic.setLoop(true);
                    gameMusic.play();
                } else if (gameStarted && !gameEnded) {
                    if (event.key.code == sf::Keyboard::Up) {
                        mario.jump();  // Mario jumps
                    }
                }
            }
        }

        if (gameStarted && !gameEnded) {
            // Handle continuous movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                mario.move(-5.f, 0.f);  // Move left
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                mario.move(5.f, 0.f);   // Move right
            }

            // Update Mario's position and state
            mario.update(world);

            // Spawn enemies if Mario is within the screen
            if (enemies.size() < maxEnemies && std::rand() % 1000 < 5) {  // 0.5% chance to spawn if under limit
                float screenLeft = camera.getCenter().x - windowWidth / 2;
                float screenRight = camera.getCenter().x + windowWidth / 2;
                spawnEnemy(screenLeft, screenRight);
            }

            // Update enemies and handle collision with Mario
            for (auto& enemy : enemies) {
                enemy.update(mario.getBounds().left, camera.getCenter().x - windowWidth / 2, camera.getCenter().x + windowWidth / 2);

                if (!enemy.isDead() && mario.getBounds().intersects(enemy.getBounds())) {
                    if (mario.getVelocity().y > 0) {  // Mario is falling onto the enemy
                        enemy.die();  // Kill the enemy
                        mario.setVerticalVelocity(-10.f);  // Mario bounces upward slightly
                    } else {  // Mario collides with the enemy from the side or below
                        gameEnded = true;  // Mario dies
                        mario.setHealth(0);  // Set Mario health to zero
                        break;  // Stop further enemy checks
                    }
                }
            }

            // Check if Mario has fallen
            if (!mario.isAlive()) {
                gameEnded = true;

                // Stop game music and play end game music
                gameMusic.stop();
                endMusic.setLoop(true);
                endMusic.play();
            } else {
                // Update camera position to follow Mario
                float marioX = mario.getBounds().left + mario.getBounds().width / 2.f;
                float cameraX = std::max(marioX, windowWidth / 2.f);
                cameraX = std::min(cameraX, static_cast<float>(world.rightBoundary() - windowWidth / 2.f));
                camera.setCenter(cameraX, windowHeight / 2.f);
                window.setView(camera);
            }
        }

        // Render
        window.clear(sf::Color(135, 206, 235));  // Sky-blue background

        if (!gameStarted) {
            // Show start screen
            window.setView(window.getDefaultView());
            window.draw(startScreenSprite);
        } else if (gameEnded) {
            // Show end game screen
            window.setView(window.getDefaultView());
            window.draw(endGameSprite);
        } else {
            // Render game world and Mario
            world.draw(window);  // Draw the visible world
            mario.draw(window);  // Draw Mario

            // Draw enemies
            for (const auto& enemy : enemies) {
                enemy.draw(window);
            }
        }

        window.display();
    }

    return 0;
}
