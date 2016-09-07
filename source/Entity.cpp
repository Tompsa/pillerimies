#include "Entity.h"

#include <cassert>


Entity::Entity(int hitpoints)
	: _velocity()
	, _hitpoints(hitpoints)
{
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	_velocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	_velocity.x = vx;
	_velocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return _velocity;
}

void Entity::damage(int points)
{
	assert(points > 0);

	_hitpoints -= points;	
}

void Entity::destroy()
{
	_hitpoints = 0;	
}

bool Entity::isDestroyed() const
{
	return _hitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
	move(_velocity * dt.asSeconds());
}
