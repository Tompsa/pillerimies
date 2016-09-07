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
, _status(0)
, _sprite(textures.get(Table[type].texture), Table[type].textureRect)
, _direction()
, _nextDirection()
{
	centerOrigin(_sprite);
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::updateCurrent(dt, commands);
}

unsigned int Character::getCategory() const
{
	if(_type == Type::Pacman)
		return Category::Pacman;
	else
		return Category::Ghost;
}

sf::FloatRect Character::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

float Character::getMaxSpeed() const
{
	return Table[_type].speed;
}

void Character::setNextDirection(Direction var)
{
	_nextDirection = var;
}
