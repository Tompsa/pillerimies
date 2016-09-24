#ifndef PICKUP_H
#define PICKUP_H

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Character;

class Pickup : public Entity
{
	public:
		enum Type
		{
			Pill,
			SuperPill,
			Fruit,
			TypeCount
		};
		
	public:
		Pickup(Type type, const TextureHolder& textures);
		
		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;
		Type					getType() const;
		
		void 					apply(Character& pacman) const;

	protected:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type 					_type;
		sf::Sprite				_sprite;
};

#endif //PICKUP_H
