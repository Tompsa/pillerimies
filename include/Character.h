#ifndef CHARACTER_H
#define CHARACTER_H

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Character : public Entity
{
	public:
			enum Type
			{
				Pacman,
				Blinky, //Red
				Pinky,  //Pink
				Inky,	//Cyan
				Clyde,	//Orange	
				TypeCount
			};
			
	public:
			enum Direction
			{
				Up, 
				Down, 
				Left, 
				Right, 
				None
			};
			
	public:
			Character(Type type, const TextureHolder& textures);
			
			virtual unsigned int	getCategory() const;
			virtual sf::FloatRect 	getBoundingRect() const;
			float 					getMaxSpeed() const;
			void					setDirection(sf::Vector2f var);
			sf::Vector2f			getDirection();
			void					setNextDirection(sf::Vector2f var);
			sf::Vector2f			getNextDirection();
			void					setValidDirection(bool var);
			void					setValidNextDirection(bool var);
			
	private:
			virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
			virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
			bool					moveTowardsPoint(sf::Vector2f goal, sf::Time dt);
		
	private:
			Type 					_type;
			int						_status;
			sf::Sprite				_sprite;
			sf::Vector2f			_direction;
			sf::Vector2f			_nextDirection;
			sf::Vector2f			_target;
			bool					_validDirection;
			bool					_validNextDirection;
};


#endif //CHARACTER_H
