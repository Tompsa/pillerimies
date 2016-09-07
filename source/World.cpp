#include "World.h"
#include "Character.h"
#include "Foreach.h"
#include "TextNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>

World::World(sf::RenderWindow& window, FontHolder& fonts)
	: _window(window)
	, _worldView(window.getDefaultView())
	, _textures()
    , _fonts(fonts)
	, _sceneGraph()
	, _sceneLayers()
	, _worldBounds(0.f, 0.f, _worldView.getSize().x, _worldView.getSize().y)
	, _spawnPosition()
    , _pacman(nullptr)
    , _playerScore(0)
    , _playerLives(3)
    , _activeGhosts()
    , _scoreDisplay(nullptr)
    , _livesDisplay(nullptr)
{
	_spawnPosition = getPosFromNode(1,1);
	loadTextures();
	buildScene();
    
    std::unique_ptr<TextNode> scoreDisplay(new TextNode(fonts, ""));
	_scoreDisplay = scoreDisplay.get();
	_sceneLayers[ObjectLayer]->attachChild(std::move(scoreDisplay));
    
    std::unique_ptr<TextNode> livesDisplay(new TextNode(fonts, ""));
	_livesDisplay = livesDisplay.get();
	_sceneLayers[ObjectLayer]->attachChild(std::move(livesDisplay));
	
    updateTexts();
}

void World::update(sf::Time dt)
{
	// Forward commands to scene graph
	while (!_commandQueue.isEmpty())
		_sceneGraph.onCommand(_commandQueue.pop(), dt);
		

	// Collision detection and response (may destroy entities)
	//handleCollisions();

    // Remove all destroyed entities, create new ones
	_sceneGraph.removeWrecks();
    
	// Regular update step, adapt position (correct if outside view)
	_sceneGraph.update(dt, _commandQueue);
	//adaptPlayerPosition();
    
    // Update score display
    updateTexts();
}

void World::draw()
{
	_window.setView(_worldView);
	_window.draw(_sceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return _commandQueue;
}

bool World::hasAlivePlayer() const
{
	return !(_playerLives == 0);
}

void World::loadTextures()
{
	_textures.load(Textures::Entities, "Media/Textures/sprites_masked.png");
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	_sceneGraph.checkSceneCollision(_sceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs)
	{
		
	}
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == ObjectLayer) ? Category::Scene : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		_sceneLayers[i] = layer.get();

		_sceneGraph.attachChild(std::move(layer));
	}
	
	// Load map 
	loadMap("Level1.txt");

	// Add player's character
	std::unique_ptr<Character> pacman(new Character(Character::Pacman, _textures));
	_pacman = pacman.get();
	_pacman->setPosition(_spawnPosition);
	_sceneLayers[ObjectLayer]->attachChild(std::move(pacman));
	
	// Add level walls
	addWalls();
	
	// Add ghosts
	//addGhosts();
	
	// Add pills
	addPills();
}

void World::addWalls()
{
	//int Wall = 3 ;
	
	//Outer wall corners
	if((_map[0][0]==Wall))
		addWall(WallType::UpLeftCorner,sf::Vector2f(0, 0));
	if((_map[27][0]==Wall))
		addWall(WallType::UpRightCorner,sf::Vector2f(27*8, 0));
	if((_map[0][30]==Wall))
		addWall(WallType::DownLeftCorner,sf::Vector2f(0, 30*8));
	if((_map[27][30]==Wall))
		addWall(WallType::DownRightCorner,sf::Vector2f(27*8, 30*8));
	
	// Walls on the top and low
	for(int x = 1; x < 27; ++x)
	{		
		if((_map[x][0]==Wall)&&(_map[x+1][0]==Wall))
		{
			addWall(WallType::UpHorizontal,sf::Vector2f(x*8, 0));
		}
		if((_map[x][30]==Wall)&&(_map[x+1][30]==Wall))
		{
			addWall(WallType::DownHorizontal,sf::Vector2f(x*8, 30*8));	
		}
	}
	// Walls on the right and left
	for(int y = 1; y < 30; ++y)
	{
		if((_map[0][y]==Wall)&&(_map[0][y+1]==Wall))
		{
			addWall(WallType::LeftVertical,sf::Vector2f(0, y*8));
		}
		if((_map[27][y]==Wall)&&(_map[27][y+1]==Wall))
		{
			addWall(WallType::RightVertical,sf::Vector2f(27*8, y*8));
		}
	}
	for(int x=1;x<27;++x) for(int y=1;y<30;++y)
	{
		if(_map[x][y]==Wall)
		{
			// Inner walls
			if(_map[x-1][y] == Wall && _map[x+1][y] == Wall && _map[x][y+1] == Wall && _map[x][y-1] != Wall)
			{
				addWall(WallType::SolidWallUp,sf::Vector2f(x*8, y*8));
			}
			if(_map[x-1][y] == Wall && _map[x+1][y] == Wall && _map[x][y+1] != Wall && _map[x][y-1] == Wall)
			{
				addWall(WallType::SolidWallDown,sf::Vector2f(x*8, y*8));
			}
			if(_map[x-1][y] != Wall && _map[x+1][y] == Wall && _map[x][y+1] == Wall && _map[x][y-1] == Wall)
			{
				addWall(WallType::SolidWallLeft,sf::Vector2f(x*8, y*8));
			}
			if(_map[x-1][y] == Wall && _map[x+1][y] != Wall && _map[x][y+1] == Wall && _map[x][y-1] == Wall)
			{
				addWall(WallType::SolidWallRight,sf::Vector2f(x*8, y*8));
			}
			
			// Corners
			if(_map[x-1][y] != Wall && _map[x+1][y] == Wall && _map[x][y+1] == Wall && _map[x][y-1] != Wall)
			{
				addWall(WallType::SolidCornerUpLeft,sf::Vector2f(x*8, y*8));
			}
			if(_map[x-1][y] == Wall && _map[x+1][y] != Wall && _map[x][y+1] == Wall && _map[x][y-1] != Wall)
			{
				addWall(WallType::SolidCornerUpRight,sf::Vector2f(x*8, y*8));
			}
			if(_map[x-1][y] != Wall && _map[x+1][y] == Wall && _map[x][y+1] != Wall && _map[x][y-1] == Wall)
			{
				addWall(WallType::SolidCornerDownLeft,sf::Vector2f(x*8, y*8));
			}
			if(_map[x-1][y] == Wall && _map[x+1][y] != Wall && _map[x][y+1] != Wall && _map[x][y-1] == Wall)
			{
				addWall(WallType::SolidCornerDownRight,sf::Vector2f(x*8, y*8));
			}
		}	
	}

}

void World::addWall(WallType type, sf::Vector2f pos)
{
	sf::Texture& wallTexture = _textures.get(Textures::Entities);
	sf::IntRect rect;
	
	switch(type)
	{
		case LeftVertical:
			rect = sf::IntRect(0, 8, 8, 8);
			break;
		case RightVertical:
			rect = sf::IntRect(27*8, 8, 8, 8);
			break;
		case UpHorizontal:
			rect = sf::IntRect(8, 0, 8, 8);
			break;
		case DownHorizontal:
			rect = sf::IntRect(8, 30*8, 8, 8);
			break;
		case UpLeftCorner:
			rect = sf::IntRect(0, 0, 8, 8);
			break;
		case UpRightCorner:
			rect = sf::IntRect(27*8, 0, 8, 8);
			break;
		case DownLeftCorner:
			rect = sf::IntRect(0, 30*8, 8, 8);
			break;
		case DownRightCorner:
			rect = sf::IntRect(27*8, 9*8, 8, 8);
			break;
		case SolidWallLeft:
			rect = sf::IntRect(2*8, 3*8, 8, 8);
			break;
		case SolidWallRight:
			rect = sf::IntRect(5*8, 3*8, 8, 8);
			break;		
		case SolidWallUp:
			rect = sf::IntRect(3*8, 2*8, 8, 8);
			break;
		case SolidWallDown:
			rect = sf::IntRect(3*8, 4*8, 8, 8);
			break;
		case SolidCornerUpLeft:
			rect = sf::IntRect(2*8, 2*8, 8, 8);
			break;
		case SolidCornerUpRight:
			rect = sf::IntRect(5*8, 2*8, 8, 8);
			break;
		case SolidCornerDownLeft:
			rect = sf::IntRect(2*8, 4*8, 8, 8);
			break;
		case SolidCornerDownRight:
			rect = sf::IntRect(5*8, 4*8, 8, 8);
			break;
	}
	
	std::unique_ptr<SpriteNode> wallSprite(new SpriteNode(wallTexture, rect));
	wallSprite->setPosition(pos);
	_sceneLayers[ObjectLayer]->attachChild(std::move(wallSprite));
}
/*
void World::addGhosts()
{
	
}
*/

void World::addPills()
{
	sf::Texture& pillTexture = _textures.get(Textures::Entities);
	
	for(int x=0;x<28;++x)for(int y=0;y<31;++y)
	{
		if(_map[x][y] == 1)
		{
			std::unique_ptr<SpriteNode> pillSprite(new SpriteNode(pillTexture, sf::IntRect(11, 11, 2, 2)));
			pillSprite->centerSprite();
			pillSprite->setPosition(8*x + 4 , 8*y + 4);
			_sceneLayers[ObjectLayer]->attachChild(std::move(pillSprite));
		}
		
	}
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds
	sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());

}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::updateTexts()
{
    _scoreDisplay->setString(toString(_playerScore));
    _scoreDisplay->setPosition(_worldView.getSize().x / 2.f, 475.f);
    
    _livesDisplay->setString("Lives: " + toString(_playerLives));
    _livesDisplay->setPosition(_worldView.getSize().x / 2.f, 500.f);
}

bool World::loadMap(const std::string& path)
{
		std::ifstream file(path.c_str());
		if(!file.is_open()) return false;

		for(int y=0;y<31;++y) for(int x=0;x<28;++x)
		{
			if(file.eof()) return false;
			int current;
			file>>current;
			_map[x][y]=current;
		}
		return true;
}
