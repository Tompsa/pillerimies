#include "Character.h"
#include "DataTables.h"
#include "Utility.h"
#include "CommandQueue.h"
#include "ResourceHolder.h"


#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

namespace
{
	const std::vector<CharacterData> Table = initializeCharacterData();
}

Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, _type(type)
, _status(Regular)
, _sprite(textures.get(Table[type].texture), Table[type].textureRect)
, _direction(0.f, 0.f)
, _nextDirection(0.f, 0.f)
, _target(0.f, 0.f)
, _elapsedTime(sf::Time::Zero)
, _duration(sf::seconds(0.25f))
, _currentFrame(0)
, _statusTimer(sf::Time::Zero)
, _debugDisplay(nullptr)
{
	centerOrigin(_sprite);


	std::unique_ptr<TextNode> debugDisplay(new TextNode(fonts, ""));
	debugDisplay->setPosition(0, 25);
	_debugDisplay = debugDisplay.get();
	attachChild(std::move(debugDisplay));
	

	updateTexts();
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{

	if(_target == sf::Vector2f(0.f, 0.f))
	{
		//if(_validDirection)
		if (std::find(_validDirections.begin(),_validDirections.end(), _direction) != _validDirections.end())
			_target = getPosition() + _direction * 32.f;
		else
			_direction = _nextDirection;
	}
	else
	{
		if(moveTowardsPoint(_target, dt)){
			_target = sf::Vector2f(0.f, 0.f);
			//if(_validNextDirection)
			if (std::find(_validDirections.begin(), _validDirections.end(), _nextDirection) != _validDirections.end())
				_direction = _nextDirection;
		}
	}

	updateMovementAnimation(dt);
	_statusTimer += dt;

	Entity::updateCurrent(dt, commands);

	updateTexts();
}

unsigned int Character::getCategory() const
{
	if(_type == Type::Pacman)
		return Category::Pacman;
	if(_type == Type::Blinky)
		return Category::Blinky;
	if(_type == Type::Pinky)
		return Category::Pinky;
	if(_type == Type::Inky)
		return Category::Inky;
	if(_type == Type::Clyde)
		return Category::Clyde;
	else
		return Category::None;
}

sf::FloatRect Character::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

float Character::getMaxSpeed() const
{
	return Table[_type].speed;
}

void Character::setDirection(sf::Vector2f var)
{
	_direction = var;
}

sf::Vector2f Character::getDirection() const
{
	return _direction;
}

void Character::setNextDirection(sf::Vector2f var)
{
	_nextDirection = var;
}

sf::Vector2f Character::getNextDirection() const
{
	return _nextDirection;
}

void Character::addValidDirection(sf::Vector2f var)
{
	_validDirections.push_back(var);
}

std::vector<sf::Vector2f>& Character::getValidDirections()
{
	return _validDirections;
}

void Character::resetValidDirections()
{
	_validDirections.clear();
}

bool Character::moveTowardsPoint(sf::Vector2f goal, sf::Time dt)
{
	if(getPosition() == goal)
		return true;
		
	sf::Vector2f direction = unitVector(goal - getPosition());
	
	if(_status == Eaten)
		move(direction * 10.f * getMaxSpeed() * dt.asSeconds());
	else
		move(direction * getMaxSpeed() * dt.asSeconds());
	
	if( abs( dot(direction, unitVector(goal - getPosition())) +1 ) < 0.1f )
		setPosition(goal);
		
	return getPosition() == goal;
}

void Character::updateTexts()
{
	if (_debugDisplay)
	{
		_debugDisplay->setString( "status " + toString(_status)	+
		"\nvalid directions size " + toString(_validDirections.size())
		);
	}
}

void Character::updateMovementAnimation(sf::Time dt)
{
	std::size_t numFrames = 2;
	bool repeat = true;

	sf::Time timePerFrame = _duration / static_cast<float>(numFrames);
	_elapsedTime += dt;

	sf::IntRect textureRect = _sprite.getTextureRect();

	if (_status != Scared && _status != Eaten)
	{
		// Default sprite, moving right
		if (_direction == sf::Vector2f(1, 0) && textureRect.top != Table[_type].textureRect.top)
			textureRect = Table[_type].textureRect;
		// Moving down
		if (_direction == sf::Vector2f(0, 1) &&
			textureRect.top != Table[_type].textureRect.top + Table[_type].textureRect.height)
		{
			textureRect = Table[_type].textureRect;
			textureRect.top += Table[_type].textureRect.height;
		}
		// Moving left
		if (_direction == sf::Vector2f(-1, 0) &&
			textureRect.top != Table[_type].textureRect.top + 2 * Table[_type].textureRect.height)
		{
			textureRect = Table[_type].textureRect;
			textureRect.top += 2 * Table[_type].textureRect.height;
		}
		// Moving up
		if (_direction == sf::Vector2f(0, -1) &&
			textureRect.top != Table[_type].textureRect.top + 3 * Table[_type].textureRect.height)
		{
			textureRect = Table[_type].textureRect;
			textureRect.top += 3 * Table[_type].textureRect.height;
		}
	}
	else
	{
		if(_status == Scared)
			textureRect.top = 0;
		if (_status == Eaten)
			textureRect.top = 2*Table[_type].textureRect.height;
	}

	if (_elapsedTime >= timePerFrame && (_currentFrame <= numFrames || repeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// And progress to next frame
		_elapsedTime -= timePerFrame;
		if (repeat)
		{
			_currentFrame = (_currentFrame + 1) % numFrames;

			if (_currentFrame == 0)
			{
				if (_status != Scared && _status != Eaten)
					textureRect.left = Table[_type].textureRect.left;
				else
					textureRect.left = 12 * Table[_type].textureRect.width;
			}
				
		}
	}
	_sprite.setTextureRect(textureRect);
}

void Character::collectPill()
{
	
}

void Character::setStatus(Status var)
{
	_status = var;
	_statusTimer = sf::Time::Zero;
}

Character::Status Character::getStatus() const
{
	return _status;
}

void Character::resetCharacter()
{
	_direction = sf::Vector2f(0.f, 0.f);
	_nextDirection = sf::Vector2f(0.f, 0.f);
	_target = sf::Vector2f(0.f, 0.f);
}

sf::Time Character::getStateTimer()
{
	return _statusTimer;
}
