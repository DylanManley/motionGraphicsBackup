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
	setupFontAndText(); // load font 
	setupSprite(); // load texture
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
		int yPos = walls[i].getPosition().y;
		int newYPos = yPos + 1;
		walls[i].setPosition(walls[i].getPosition().x, newYPos);
	}

	HandleInput();
	player.setPosition(playerPos);
	collision();
}


void Game::render()
{
	m_window.clear(sf::Color::White);
	for (int i = 0; i < noWalls; i++)
	{
		m_window.draw(walls[i]);
	}
	m_window.draw(player);
	m_window.display();
}

void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}


}

void Game::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		std::cout << "problem loading logo" << std::endl;
	}



	int levelData[] = {
		1,1,1,1,0,1,1,1,1,
		1,1,1,1,0,1,1,1,1,
		1,1,1,1,0,1,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,1,0,0,1,1,
		1,1,0,0,1,0,0,1,1,
		1,1,0,0,1,0,0,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,0,0,0,1,1,
		1,0,0,0,0,0,0,0,1,
		1,0,0,0,1,1,0,0,1,
		1,0,0,1,1,1,0,0,1,
		1,0,1,1,1,1,1,0,1,
		1,0,1,1,1,1,1,0,1,
		1,0,1,1,1,1,1,0,1,
		1,0,0,1,1,1,0,0,1,
		1,1,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,1,1,
		1,1,1,1,0,0,0,1,1,
		1,1,1,1,1,0,0,0,1,
		1,1,1,1,1,1,0,0,1,
		1,1,1,1,1,0,0,0,1,
		1,1,1,1,1,0,0,0,1,
		1,1,1,1,1,0,0,0,1,
		1,1,1,1,0,0,0,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,0,1,1,1,1,
		1,0,0,0,1,1,1,1,1,
		1,0,0,0,1,1,1,1,1,
		1,1,0,0,0,1,1,1,1,
		1,1,1,0,0,0,1,1,1,
		1,1,0,0,0,0,0,1,1,
		1,1,0,0,0,0,0,1,1,
		1,0,0,0,0,0,0,0,1
						};

	int wallsX = 0;
	int wallsY = -1200;

	for (int i = 0; i < noWalls; i++)
	{
		walls[i].setPosition(wallsX, wallsY);
		walls[i].setSize(sf::Vector2f(90,60));
		
		if (levelData[i] == 1)
		{
			walls[i].setFillColor(wallColour);
		}
		else if(levelData[i] == 0)
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
}

void Game::collision()
{
	for (int i = 0; i < noWalls; i++)
	{
		if (player.getGlobalBounds().intersects(walls[i].getGlobalBounds()))
		{
			if (walls[i].getFillColor() == wallColour)
			{
				endGame();
			}
		}
	}
}

void Game::endGame()
{
	m_window.close();
}
