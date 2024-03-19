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

	switch (gameState)
	{
	case GameState::Menu:
			handleInput();
		break;
	case GameState::edit:
			editLevel();
		break;
	case GameState::running:
		Collisions();
		animatePlayer();
		handleInput();

		for (int row = 0; row < numRows; row++)
		{

			for (int col = 0; col < numCols; col++)
			{

				level[row][col].move(direction, 0);
				level[row][col].setOutlineThickness(0);
			}

		}

		velocityY = velocityY + gravity;
		player.move(0, velocityY);
		gravity = 0.6;

		break;
	case GameState::won:
		break;
	default:
		break;
	}	
}


void Game::render()
{
	switch (gameState)
	{
	case GameState::Menu:
		m_window.clear(Khaki);
		for (int i = 0; i < noButtons; i++)
		{
			m_window.draw(menuButtons[i]);
			m_window.draw(buttonText[i]);
		}
		break;
	case GameState::edit:
		m_window.clear(Auburn);
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				m_window.draw(level[row][col]);
			}

			for (int i = 0; i < 6; i++)
			{
				m_window.draw(selectableTiles[i]);
			}
		}

		break;
	case GameState::running:
		m_window.clear(Auburn);
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				m_window.draw(level[row][col]);
			}
		}

		m_window.draw(player);
		break;
	case GameState::won:
		m_window.clear(Night);
		m_window.draw(wintext);
		break;
	default:
		break;
	}

	m_window.display();
}

void Game::setup()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	

	groundTexture.loadFromFile("ASSETS\\IMAGES\\Ground_Tiles.jpg");
	crateTexture.loadFromFile("ASSETS\\IMAGES\\crate.png");
	playerTexture.loadFromFile("ASSETS\\IMAGES\\spritesheet.png");
	bounceTexture.loadFromFile("ASSETS\\IMAGES\\UpArrow.png");
	directionTexture.loadFromFile("ASSETS\\IMAGES\\DirectionArrow.png");
	player.setTexture(&playerTexture);
	player.setFillColor(Khaki);
	
	view = window.getDefaultView();
	view.setCenter(player.getPosition());
	view.zoom(100);

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			level[row][col].setSize(sf::Vector2f(70, 30));
			level[row][col].setPosition(row * 70, col * 30);
		}
	}

		selectableTiles[0].setSize(sf::Vector2f(70, 30));
		selectableTiles[0].setPosition(30, 40);
		selectableTiles[0].setFillColor(Auburn);
		selectableTiles[0].setOutlineColor(sf::Color::Cyan);
		selectableTiles[0].setOutlineThickness(2);

		selectableTiles[1].setSize(sf::Vector2f(70, 30));
		selectableTiles[1].setPosition(30, 80);
		selectableTiles[1].setFillColor(AshGrey);
		selectableTiles[1].setTexture(&groundTexture);
		selectableTiles[1].setOutlineColor(sf::Color::Cyan);
		selectableTiles[1].setOutlineThickness(2);

		selectableTiles[2].setSize(sf::Vector2f(70, 30));
		selectableTiles[2].setPosition(30, 120);
		selectableTiles[2].setTexture(&crateTexture);
		selectableTiles[2].setFillColor(Cream);
		selectableTiles[2].setOutlineColor(sf::Color::Cyan);
		selectableTiles[2].setOutlineThickness(2);

		selectableTiles[3].setSize(sf::Vector2f(70, 30));
		selectableTiles[3].setPosition(30, 160);
		selectableTiles[3].setFillColor(sf::Color::White);
		selectableTiles[3].setOutlineColor(sf::Color::Cyan);
		selectableTiles[3].setOutlineThickness(2);

		selectableTiles[4].setSize(sf::Vector2f(70, 30));
		selectableTiles[4].setPosition(30, 200);
		selectableTiles[4].setFillColor(Cream);
		selectableTiles[4].setTexture(&bounceTexture);
		selectableTiles[4].setOutlineColor(sf::Color::Cyan);
		selectableTiles[4].setOutlineThickness(2);

		selectableTiles[5].setSize(sf::Vector2f(70, 30));
		selectableTiles[5].setPosition(30, 240);
		selectableTiles[5].setFillColor(sf::Color::Blue);
		selectableTiles[5].setTexture(&directionTexture);
		selectableTiles[5].setOutlineColor(sf::Color::Cyan);
		selectableTiles[5].setOutlineThickness(2);

		player.setSize(sf::Vector2f(20, 20));
		player.setPosition(160, 530);
		player.setFillColor(sf::Color::White);
		isJumping = false;
		reversed = false;

		wintext.setFillColor(Auburn);
		wintext.setPosition(300, 300);
		wintext.setCharacterSize(30);
		wintext.setString("You Won!");
		wintext.setFont(m_ArialBlackfont);


		for (int i = 0; i < noButtons; i++)
		{
			menuButtons[i].setSize(sf::Vector2f(300, 50));
			menuButtons[i].setFillColor(Night);
			menuButtons[i].setOrigin(menuButtons[i].getSize().x / 2, menuButtons[i].getSize().y / 2);
			menuButtons[i].setPosition(m_window.getSize().x / 2, (m_window.getSize().y / 2) + 200 - (100 * i));

			buttonText[i].setFillColor(Auburn);
			buttonText[i].setOrigin(buttonText[i].getGlobalBounds().width / 2, buttonText[i].getGlobalBounds().height / 2);
			buttonText[i].setPosition(menuButtons[i].getPosition().x - 20 , menuButtons[i].getPosition().y - 10);
			buttonText[i].setCharacterSize(20);
			buttonText[i].setFont(m_ArialBlackfont);
			buttonText[2].setString("EDIT");
			buttonText[1].setString("LOAD");
			buttonText[0].setString("QUIT");
			
		}
}

void Game::editLevel()
{
	sf::Vector2f mousePos;
	mousePos.x = sf::Mouse::getPosition(m_window).x;
	mousePos.y = sf::Mouse::getPosition(m_window).y;


	for (int i = 0; i < 6; i++)
	{
		if (selectableTiles[i].getGlobalBounds().contains(mousePos))
		{
			selectedTile = i;
		}
	}

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (level[row][col].getGlobalBounds().contains(mousePos))
				{
					levelData[row][col] = selectedTile;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				level[row][col].move(3.5, 0);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				level[row][col].move(-3.5, 0);
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				setup();
				gameState = GameState::running;
			}

			//tileTypes
			if (levelData[row][col] == 1)
			{
				level[row][col].setPosition(level[row][col].getPosition());
				level[row][col].setFillColor(AshGrey);
				level[row][col].setTexture(NULL);
				level[row][col].setTexture(&groundTexture);
			}
			if (levelData[row][col] == 0)
			{
				level[row][col].setPosition(level[row][col].getPosition());
				level[row][col].setFillColor(Auburn);
				level[row][col].setTexture(NULL);
			}
			if (levelData[row][col] == 2)
			{
				level[row][col].setPosition(level[row][col].getPosition());
				level[row][col].setTexture(NULL);
				level[row][col].setTexture(&crateTexture);
				level[row][col].setFillColor(Cream);

			}

			if (levelData[row][col] == 3)
			{
				level[row][col].setPosition(level[row][col].getPosition());
				level[row][col].setFillColor(sf::Color::White);
				level[row][col].setTexture(NULL);
			}

			if (levelData[row][col] == 4)
			{
				level[row][col].setPosition(level[row][col].getPosition());
				level[row][col].setFillColor(Cream);
				level[row][col].setTexture(NULL);
				level[row][col].setTexture(&bounceTexture);
			}

			if (levelData[row][col] == 5)
			{
				level[row][col].setPosition(level[row][col].getPosition());
				level[row][col].setFillColor(sf::Color::Blue);
				level[row][col].setTexture(NULL);
				level[row][col].setTexture(&directionTexture);
			}

			level[row][col].setOutlineThickness(2);
			level[row][col].setOutlineColor(sf::Color::White);
		}
	}
}

void Game::handleInput()
{
	sf::Vector2f mousePos;
	mousePos.x = sf::Mouse::getPosition(m_window).x;
	mousePos.y = sf::Mouse::getPosition(m_window).y;


	switch (gameState)
	{
	case GameState::Menu:
		
		for (int i = 0; i < noButtons; i++)
		{
			if (menuButtons[i].getGlobalBounds().contains(mousePos))
			{
				menuButtons[i].setFillColor(sf::Color::White);
			}
			else
			{
				menuButtons[i].setFillColor(Night);
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if(menuButtons[0].getGlobalBounds().contains(mousePos))
			{
				m_window.close();
			}

			if (menuButtons[2].getGlobalBounds().contains(mousePos))
			{
				gameState = GameState::edit;
			}
		}

		break;
	case GameState::edit:
		break;
	case GameState::running:
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
		break;
	case GameState::won:
		break;
	default:
		break;
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
							gameState = GameState::edit;
						}
					}


				}

			}

			if (levelData[row][col] == 2)
			{
				if (player.getGlobalBounds().intersects(level[row][col].getGlobalBounds()))
				{
					hasReversed = false;
					gameState = GameState::edit;
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
		gameState = GameState::edit;
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
	}
}
