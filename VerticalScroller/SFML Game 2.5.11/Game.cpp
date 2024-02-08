#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 1000U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setup();
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
	}
}


void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	for (int i = 0; i < noWalls; i++)
	{
		if (scroll)
		{
			if (gameOver == false)
			{
				walls[i].setPosition(walls[i].getPosition().x, walls[i].getPosition().y + 1);
				winText.setPosition(270, walls[0].getPosition().y - 500);
			}
		}
	}

	HandleInput();
	player.setPosition(playerPos);
	collision();

	if (shooting == true)
	{
		bullet.setPosition(bullet.getPosition().x, bullet.getPosition().y - 20);
	}

	if (bullet.getPosition().y <= 0)
	{
		canShoot = true;
	}

	scoreText.setString("Score: " + std::to_string(score));

	EnemyBullets();

	if (walls[0].getPosition().y == 1000)
	{
		scroll = false;
	}
}


void Game::render()
{
	m_window.clear(backgroundColour);
	for (int i = 0; i < noWalls; i++)
	{
		m_window.draw(walls[i]);
	}

	if (shooting == true)
	{
		m_window.draw(bullet);
	}
	m_window.draw(player);

	if (gameOver == true)
	{
		m_window.draw(m_gameOverMessage);
		scoreText.setPosition(m_gameOverMessage.getPosition().x + 30, m_gameOverMessage.getPosition().y + 100);
	}

	if (!scroll)
	{
		m_window.draw(startMessage);
	}

	m_window.draw(winText);

	m_window.draw(scoreText);

	m_window.display();
}


void Game::setup()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	m_gameOverMessage.setString("      Game Over \n Press R to restart");
	m_gameOverMessage.setFont(m_ArialBlackfont);
	m_gameOverMessage.setFillColor(yellowColour);
	m_gameOverMessage.setCharacterSize(30);
	m_gameOverMessage.setPosition(270, 450);

	startMessage.setString("Press R to start");
	startMessage.setFont(m_ArialBlackfont);
	startMessage.setFillColor(yellowColour);
	startMessage.setCharacterSize(30);
	startMessage.setPosition(270, 450);


	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setFont(m_ArialBlackfont);
	scoreText.setFillColor(yellowColour);
	scoreText.setCharacterSize(30);
	scoreText.setPosition(10, 10);

	winText.setString("YOU WON");
	winText.setFont(m_ArialBlackfont);
	winText.setFillColor(yellowColour);
	winText.setCharacterSize(30);
	winText.setPosition(270, walls[0].getPosition().y - 450);

	shooting = false;
	canShoot = true;

	int levelData[] = {
		1,1,1,1,0,1,1,1,1,
		1,1,1,1,2,1,1,1,1,
		1,1,1,1,0,1,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,1,0,0,1,1,
		1,1,0,0,1,3,0,1,1,
		1,1,0,0,1,0,0,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,1,0,2,0,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,3,0,0,1,1,
		1,0,0,0,0,0,0,0,1,
		1,0,0,0,1,1,0,0,1,
		1,0,0,1,1,1,0,0,1,
		1,0,1,1,1,1,1,3,1,
		1,3,1,1,1,1,1,2,1,
		1,0,1,1,1,1,1,0,1,
		1,0,0,1,1,1,0,0,1,
		1,1,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,1,1,
		1,1,1,1,0,0,0,1,1,
		1,1,1,1,1,0,0,0,1,
		1,1,1,1,1,1,0,0,1,
		1,1,1,1,1,0,0,0,1,
		1,1,1,1,1,0,2,0,1,
		1,1,1,1,1,0,0,0,1,
		1,1,1,1,0,0,0,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,0,1,1,1,1,
		1,0,2,0,1,1,1,1,1,
		1,0,0,0,1,1,1,1,1,
		1,1,0,0,0,1,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,1,1,
		1,0,0,0,0,0,0,0,1
						};

	int wallsX = 0;
	int wallsY = -1250;

	for (int i = 0; i < noWalls; i++)
	{
		walls[i].setPosition(wallsX, wallsY);
		walls[i].setSize(sf::Vector2f(90,60));
		
		if (levelData[i] == 1)
		{
			walls[i].setFillColor(wallColour);
		}
		else if (levelData[i] == 2)
		{
			walls[i].setFillColor(yellowColour);
			walls[i].setSize(sf::Vector2f(10, 10));
			walls[i].setOrigin(5, 5);
			walls[i].setPosition(walls[i].getPosition().x + 45, walls[i].getPosition().y + 45);
		}
		else if (levelData[i] == 3)
		{
			walls[i].setFillColor(bulletColour);
			walls[i].setSize(sf::Vector2f(30, 30));
			walls[i].setOrigin(15, 15);
			walls[i].setPosition(walls[i].getPosition().x + 45, walls[i].getPosition().y + 45);
			//enemyBullet[i].setPosition(walls[i].getPosition());
		}

		else if (levelData[i] == 0)
		{
			walls[i].setFillColor(backgroundColour);
			
		}

		wallsX = wallsX + 90;

		if (wallsX == 810)
		{
			wallsY = wallsY + 60;
			wallsX = 0;
		}
	}

	player.setSize(sf::Vector2f(20, 40));
	player.setPosition(400, 820);
	player.setFillColor(playerColour);
	playerPos = player.getPosition();

	bullet.setRadius(10);
	bullet.setPosition(player.getPosition());
	bullet.setFillColor(bulletColour);

	for (int i = 0; i < 5; i++)
	{
		enemyBullet[i].setFillColor(bulletColour);
		enemyBullet[i].setRadius(10);
		enemyBullet[i].setPosition(0, 0);
	}

}

void Game::moveLeft()
{
	playerPos.x = playerPos.x - 5;
}

void Game::moveRight()
{
	playerPos.x = playerPos.x + 5;
}

void Game::moveDown()
{
	playerPos.y = playerPos.y + 2;
}

void Game::moveUp()
{
	playerPos.y = playerPos.y - 1;
}

void Game::HandleInput()
{
	if (scroll)
	{
		if (gameOver == false)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				moveLeft();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				moveRight();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				moveUp();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				moveDown();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{

				if (canShoot)
				{
					shoot();
				}
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		if (!scroll)
		{
			scroll = true;
		}

		if (gameOver == true)
		{
			gameOver = false;
			setup();
		}
	}
}

void Game::collision()
{
	for (int i = 0; i < noWalls; i++)
	{
		if (player.getGlobalBounds().intersects(walls[i].getGlobalBounds()))
		{
			if (walls[i].getFillColor() == wallColour || walls[i].getFillColor() == bulletColour)
			{
				gameOver = true;
			}

			if (walls[i].getFillColor() == yellowColour)
			{
				walls[i].setPosition(2000, 2000);
				score++;
			}
		}

		if (bullet.getGlobalBounds().intersects(walls[i].getGlobalBounds()))
		{
			if (walls[i].getFillColor() == bulletColour)
			{
				walls[i].setPosition(2000, 2000);
				score = score + 5;
			}
		}
	}
}

void Game::shoot()
{
	bullet.setPosition(playerPos);
	shooting = true;
	canShoot = false;
}

void Game::EnemyBullets()
{
	for (int i = 0; i < 5; i++)
	{
		if (player.getPosition().y < enemyBullet[i].getPosition().y)
		{
			enemyBullet[i].setPosition(enemyBullet[i].getPosition().x, enemyBullet[i].getPosition().y - 10);
		}
		else if (player.getPosition().y > enemyBullet[i].getPosition().y)
		{
			enemyBullet[i].setPosition(enemyBullet[i].getPosition().x, enemyBullet[i].getPosition().y + 10);
		}
	}

}


