/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();

	void update(sf::Time t_deltaTime);
	void render();
	void setup();
	void moveLeft();
	void moveRight();
	void moveDown();
	void moveUp();
	void HandleInput();
	void collision();
	void shoot();
	void EnemyBullets();



	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_gameOverMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	bool m_exitGame; // control exiting game

	static const int noWalls = 1024;
	sf::RectangleShape walls[noWalls];
	sf::RectangleShape player;
	sf::Vector2f playerPos;
	sf::Color wallColour{216, 30, 91};
	sf::Color backgroundColour{51, 24, 50};
	sf::Color playerColour {240, 84, 79};
	sf::Color yellowColour{253, 240, 213};
	sf::Color bulletColour{198, 216, 211};
	bool gameOver = false;
	bool scroll = false;

	bool shooting = false;
	bool canShoot = true;
	sf::CircleShape bullet;
	int currentBullet = 0;

	int score = 0;
	sf::Text scoreText;
	sf::Text startMessage;

	sf::CircleShape enemyBullet[10];
};

#endif // !GAME_HPP

