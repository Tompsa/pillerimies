#include "World.h"
#include "Pickup.h"
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
	, _remainingPills()
	, _activeGhosts()
    , _scoreDisplay(nullptr)
    , _livesDisplay(nullptr)
	, _map()
{
	_spawnPosition = getPosFromNode(14,23);
	loadTextures();
	buildScene();
    
    std::unique_ptr<TextNode> scoreDisplay(new TextNode(fonts, ""));
	_scoreDisplay = scoreDisplay.get();
	_sceneLayers[EntityLayer]->attachChild(std::move(scoreDisplay));
    
    std::unique_ptr<TextNode> livesDisplay(new TextNode(fonts, ""));
	_livesDisplay = livesDisplay.get();
	_sceneLayers[EntityLayer]->attachChild(std::move(livesDisplay));
	
    updateTexts();
}

void World::update(sf::Time dt)
{
	// Forward commands to scene graph
	while (!_commandQueue.isEmpty())
		_sceneGraph.onCommand(_commandQueue.pop(), dt);
			
	checkCharacterDirections();
	
	// Collision detection and response (may destroy entities)
	handleCollisions();

    // Remove all destroyed entities, create new ones
	_sceneGraph.removeWrecks();
    
	// Regular update step, adapt position (correct if outside view)
	_sceneGraph.update(dt, _commandQueue);
    
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
	//_textures.load(Textures::Entities, "Media/Textures/sprites_masked.png");
	_textures.load(Textures::Entities, "Media/Textures/chompersprites.png");
	_textures.load(Textures::MazeTiles, "Media/Textures/chompermazetiles.png");
	_textures.load(Textures::Background, "Media/Textures/background.png");
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
	for (std::vector<Pickup*>::iterator it = _remainingPills.begin(); it != _remainingPills.end();)
	{
		if ( (_pacman->getPosition() == (*it)->getPosition()) )
		{
			(*it)->apply(*_pacman);
			if ((*it)->getType() == Pickup::Pill)
			{
				_playerScore += 10;
			}
			if ((*it)->getType() == Pickup::SuperPill)
			{
				_playerScore += 50;
				for (auto& ghost : _activeGhosts)
					ghost->setStatus(Character::Scared);
			}
			(*it)->destroy();
			it = _remainingPills.erase(it);
		}
		else
			++it;
	}
	for (std::vector<Character*>::iterator it = _activeGhosts.begin(); it != _activeGhosts.end(); ++it)
	{
		if (getNodeFromPos(_pacman->getPosition()) == getNodeFromPos((*it)->getPosition()))
		{	
			if (_pacman->getStatus() == Character::Super && (*it)->getStatus() == Character::Scared)
			{
				(*it)->setStatus(Character::Eaten);
				_playerScore += 50;
			}
			if ((*it)->getStatus() == Character::Regular)
			{
				_playerLives -= 1;
				_pacman->resetCharacter();
				_pacman->setPosition(_spawnPosition);
			}
		}
	}
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == EntityLayer) ? Category::Scene : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		_sceneLayers[i] = layer.get();

		_sceneGraph.attachChild(std::move(layer));
	}
	// Prepare the tiled background
	sf::Texture& bgTexture = _textures.get(Textures::Background);
	bgTexture.setRepeated(true);

	float viewHeight = _worldView.getSize().y;
	sf::IntRect textureRect(_worldBounds);
	textureRect.height += static_cast<int>(viewHeight);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> bgSprite(new SpriteNode(bgTexture, textureRect));
	bgSprite->setPosition(_worldBounds.left, _worldBounds.top - viewHeight);
	_sceneLayers[Background]->attachChild(std::move(bgSprite));

	
	// Load map 
	_map.loadMap("level.txt");
	
	// Add level walls
	_map.addWalls(_textures, *_sceneLayers[MazeLayer]);
	
	// Add pills
	addPills();

	// Add player's character
	std::unique_ptr<Character> pacman(new Character(Character::Pacman, _textures, _fonts));
	_pacman = pacman.get();
	_pacman->setPosition(_spawnPosition);
	_sceneLayers[EntityLayer]->attachChild(std::move(pacman));
	
	// Add ghosts
	addGhosts();
}

void World::addGhosts()
{
	for(int type = Character::Type::Blinky; type < Character::Type::TypeCount; type++)
	{
		std::unique_ptr<Character> ghost(new Character(static_cast<Character::Type>(type), _textures, _fonts));
		_activeGhosts.push_back(ghost.get());
		ghost->setPosition(getPosFromNode(type+8,11));
		_sceneLayers[EntityLayer]->attachChild(std::move(ghost));
	}
}

void World::addPills()
{
	for(int x=0;x<28;++x)for(int y=0;y<31;++y)
	{
		if(_map.isPillTile(x,y))
		{
			std::unique_ptr<Pickup> pill(new Pickup(Pickup::Pill, _textures));
			pill->setPosition(32*x + 16 , 32*y + 16);
			_remainingPills.push_back(pill.get());
			_sceneLayers[EntityLayer]->attachChild(std::move(pill));
		}		
		if (_map.isSuperPillTile(x,y))
		{
			std::unique_ptr<Pickup> pill(new Pickup(Pickup::SuperPill, _textures));
			pill->setPosition(32 * x + 16, 32 * y + 16);
			_remainingPills.push_back(pill.get());
			_sceneLayers[EntityLayer]->attachChild(std::move(pill));
		}
	}
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
}

void World::updateTexts()
{
    _scoreDisplay->setString(toString(_playerScore));
    _scoreDisplay->setPosition(1100.f, 475.f);
    
    _livesDisplay->setString("Lives: " + toString(_playerLives));
    _livesDisplay->setPosition(1100.f, 500.f);

}

void World::checkCharacterDirections()
{
	Command charCollector;
	charCollector.category = Category::Character;
	charCollector.action = derivedAction<Character>([this] (Character& character, sf::Time)
	{
		character.resetValidDirections();
		if (checkDirection(character.getPosition(), sf::Vector2f(-1, 0), character))
			character.addValidDirection(sf::Vector2f(-1, 0));
		if (checkDirection(character.getPosition(), sf::Vector2f(1, 0), character))
			character.addValidDirection(sf::Vector2f(1, 0));
		if (checkDirection(character.getPosition(), sf::Vector2f(0, 1), character))
			character.addValidDirection(sf::Vector2f(0, 1));
		if (checkDirection(character.getPosition(), sf::Vector2f(0, -1), character))
			character.addValidDirection(sf::Vector2f(0, -1));
	});	
		
	_commandQueue.push(charCollector);
}

bool World::checkDirection(sf::Vector2f position, sf::Vector2f direction, Character& ch)
{
	sf::Vector2i mapNodePos(position.x / 32, position.y / 32);
		
	sf::Vector2i intDirection(direction.x, direction.y);
		
	sf::Vector2i targetedTile = mapNodePos + intDirection;

	if (ch.getStatus() != Character::Eaten && _map.isGateTile(targetedTile))
		return false;
	else
		return _map.isEnterableTile(targetedTile);
} 
