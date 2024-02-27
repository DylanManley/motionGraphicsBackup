/// <summary>
/// @author Dylan Manley
///


#include "Game.h"
#include <iostream>


Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} 
{
	setup();
}


Game::~Game()
{
}


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
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	handleInput();
	Collisions();
	animatePlayer();

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			level[row][col].move(direction, 0);
		}

	}

	velocityY = velocityY + gravity;
	player.move(0, velocityY);
	gravity = 0.6;

}


void Game::render()
{
	m_window.clear(Night);

	if (!win)
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				m_window.draw(level[row][col]);
			}
		}
		m_window.draw(player);
	}
	else
	{
		m_window.draw(wintext);
	}

	m_window.display();
}

void Game::setup()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	wintext.setFillColor(Auburn);
	wintext.setPosition(300, 300);
	wintext.setCharacterSize(30);
	wintext.setString("You Won!");
	wintext.setFont(m_ArialBlackfont);

	groundTexture.loadFromFile("ASSETS\\IMAGES\\Ground_Tiles.jpg");
	crateTexture.loadFromFile("ASSETS\\IMAGES\\crate.png");
	playerTexture.loadFromFile("ASSETS\\IMAGES\\spritesheet.png");
	bounceTexture.loadFromFile("ASSETS\\IMAGES\\UpArrow.png");
	directionTexture.loadFromFile("ASSETS\\IMAGES\\DirectionArrow.png");
	player.setTexture(&playerTexture);
	player.setFillColor(Khaki);
	
	createLevel();
	view = window.getDefaultView();
	view.setCenter(player.getPosition());
	view.zoom(100);
	player.setSize(sf::Vector2f(20, 20));
	player.setPosition(160, 530);
	player.setFillColor(sf::Color::White);
	isJumping = false;
	reversed = false;

}

void Game::createLevel()
{
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			if (levelData[row][col] == 1)
			{

				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition(row * 70, col * 30);
				level[row][col].setFillColor(AshGrey);
				level[row][col].setTexture(&groundTexture);
			}
			if (levelData[row][col] == 0)
			{

				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition(row * 70, col * 30);
				level[row][col].setFillColor(Auburn);
			}
			if (levelData[row][col] == 2)
			{
				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition(row * 70, col * 30);
				level[row][col].setTexture(&crateTexture);
				level[row][col].setFillColor(Cream);

			}

			if (levelData[row][col] == 3)
			{
				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition(row * 70, col * 30);
				level[row][col].setFillColor(sf::Color::White);

			}

			if (levelData[row][col] == 4)
			{
				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition(row * 70, col * 30);
				level[row][col].setFillColor(Cream);

				level[row][col].setTexture(&bounceTexture);
			}

			if (levelData[row][col] == 5)
			{
				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition((row * 70) + 35 , col * 30);
				level[row][col].setFillColor(sf::Color::Blue);
				level[row][col].setOrigin(35, 0);
				level[row][col].setTexture(&directionTexture);
			}
		}
	}
}

void Game::handleInput()
{
	if (isJumping == false)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			velocityY = -11.8;
			isJumping = true;
		}

		if (gravity == 0)
		{
			isJumping = false;
		}
	}
}

void Game::Collisions()
{
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			if (velocityY >= 0)
			{
				if (levelData[row][col] == 1)
				{

					if (player.getGlobalBounds().intersects(level[row][col].getGlobalBounds()))
					{
						if (player.getPosition().y < level[row][col].getPosition().y)
						{
							gravity = 0;
							velocityY = 0;
							player.setPosition(player.getPosition().x, level[row][col].getPosition().y);
							player.move(0, -player.getGlobalBounds().height);
							isJumping = false;
							hasReversed = false;
							break;
						}
						else {
							setup();
						}
					}


				}

			}

			if (levelData[row][col] == 2)
			{
				if (player.getGlobalBounds().intersects(level[row][col].getGlobalBounds()))
				{
					setup();
					hasReversed = false;
				}
				
			}

			if (levelData[row][col] == 3)
			{
				if (player.getGlobalBounds().intersects(level[row][col].getGlobalBounds()))
				{
					win = true;

				}
			}

			if (levelData[row][col] == 4)
			{
				if (player.getGlobalBounds().intersects(level[row][col].getGlobalBounds()))
				{
					velocityY = -20;
					hasReversed = false;

				}
			}

			if (levelData[row][col] == 5)
			{
				if (player.getGlobalBounds().intersects(level[row][col].getGlobalBounds()))
				{
					velocityY = 0;
					gravity = 0;
					reverse();
				}
			}
		}
	}

	if (player.getPosition().y > 600)
	{
		setup();
	}

}

void Game::animatePlayer()
{
	int row;

	if (!isJumping)
	{
		row = 1;

		totalElapsed++;
		if (totalElapsed > 9)
		{
			totalElapsed = 0;
			currentFrame++;
			if (currentFrame > 10)
			{
				currentFrame = 0;
			}
		}

	}
	else
	{
		row = 2;
		totalElapsed = 0;

		totalElapsed++;
		if (totalElapsed > 2)
		{
			totalElapsed = 3;
			currentFrame++;
			if (currentFrame > 3)
			{
				currentFrame = 3;
			}
		}
	}

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 884;
	rectSourceSprite.width = 720;
	rectSourceSprite.left = rectSourceSprite.width * totalElapsed;
	rectSourceSprite.top = (rectSourceSprite.height * row) - rectSourceSprite.height;
	
	player.setTextureRect(rectSourceSprite);


}

void Game::reverse()
{
	if (!hasReversed)
	{
		hasReversed = true;
		if (!reversed)
		{
			direction = 3.5;
			reversed = true;
			player.setScale(-1, 1);
		}
		else
		{
			direction = -3.5;
			reversed = false;
			player.setScale(1, 1);
		}

		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				if (levelData[row][col] == 5)
				{
					level[row][col].setScale(level[row][col].getScale().x * -1, level[row][col].getScale().y);
				}
			}
		}
	}

	

}
