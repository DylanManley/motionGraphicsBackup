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
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}

	if (sf::Keyboard::A == t_event.key.code)
	{
		direction = 1;
	}

	if (sf::Keyboard::D == t_event.key.code)
	{
		direction = 2;
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

	movePlayer(direction);
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
		1,1,1,1,0,0,0,1,1,
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
	int wallsY = -1024;

	for (int i = 0; i < noWalls; i++)
	{
		walls[i].setPosition(wallsX, wallsY);
		walls[i].setSize(sf::Vector2f(90,60));
		
		if (levelData[i] == 1)
		{
			walls[i].setFillColor(sf::Color::Red);
		}
		else
		{
			walls[i].setFillColor(sf::Color::Black);
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
	player.setFillColor(sf::Color::Blue);
}

void Game::movePlayer(int t_direction)
{
	sf::Vector2f playerPos = player.getPosition();

	if (t_direction == 1)
	{
		playerPos.x = playerPos.x - 3;
	}
	else if(t_direction == 2)
	{
		playerPos.x = playerPos.x + 3;
	}

	player.setPosition(playerPos);
}
