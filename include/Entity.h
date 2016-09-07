#ifndef ENTITY_H
#define ENTITY_H

#include "SceneNode.h"


class Entity : public SceneNode
{
public:
    explicit            Entity(int hitpoints);

	void				setVelocity(sf::Vector2f velocity);
	void				setVelocity(float vx, float vy);
	sf::Vector2f		getVelocity() const;
    
    void				damage(int points);
	void				destroy();
	virtual bool		isDestroyed() const;


protected:
	virtual void		updateCurrent(sf::Time dt, CommandQueue& commands);


private:
	sf::Vector2f		_velocity;
	int					_hitpoints;

};

#endif // ENTITY_H
