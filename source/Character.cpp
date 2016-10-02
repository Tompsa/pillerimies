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

Character::Character(Type type, const TextureHolder& textures)
: Entity(Table[type].hitpoints)
, _type(type)
, _status(Regular)
, _sprite(textures.get(Table[type].texture), Table[type].textureRect)
, _direction(0.f, 0.f)
, _nextDirection(0.f, 0.f)
, _target(0.f, 0.f)
{
	centerOrigin(_sprite);
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
			_target = getPosition() + _direction * 8.f;
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

	Entity::updateCurrent(dt, commands);
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
	
	move(direction * getMaxSpeed() * dt.asSeconds());
	
	if( abs( dot(direction, unitVector(goal - getPosition())) +1 ) < 0.1f )
		setPosition(goal);
		
	return getPosition() == goal;
}

void Character::collectPill()
{
	
}

void Character::setStatus(Status var)
{
	_status = var;
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