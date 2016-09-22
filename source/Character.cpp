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
, _validDirection(false)
, _validNextDirection(false)
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
		if(_validDirection)
			_target = getPosition() + _direction * 8.f;
		else
			_direction = _nextDirection;
	}
	else
	{
		if(moveTowardsPoint(_target, dt)){
			_target = sf::Vector2f(0.f, 0.f);
			if(_validNextDirection)
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

sf::Vector2f Character::getDirection()
{
	return _direction;
}

void Character::setNextDirection(sf::Vector2f var)
{
	_nextDirection = var;
}

sf::Vector2f Character::getNextDirection()
{
	return _nextDirection;
}

bool Character::moveTowardsPoint(sf::Vector2f goal, sf::Time dt)
{
	if(getPosition() == goal)
		return true;
		
	sf::Vector2f direction = unitVector(goal - getPosition());
	
	move(direction * 50.f * dt.asSeconds());
	
	if( abs( dot(direction, unitVector(goal - getPosition())) +1 ) < 0.1f )
		setPosition(goal);
		
	return getPosition() == goal;
}

void Character::setValidDirection(bool var)
{
	_validDirection = var;
}

void Character::setValidNextDirection(bool var)
{
	_validNextDirection = var;
}

void Character::collectPill()
{
	
}

void Character::setSuperMode(bool var)
{
	if(var)
		_status = Super;
	else
		_status = Regular;
}
