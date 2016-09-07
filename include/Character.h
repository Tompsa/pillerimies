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
			void					setNextDirection(Direction var);
			
	private:
			virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
			virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
		
	private:
			Type 					_type;
			int						_status;
			sf::Sprite				_sprite;
			Direction				_direction;
			Direction				_nextDirection;
};


#endif //CHARACTER_H
