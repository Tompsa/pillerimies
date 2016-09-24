#include "Pickup.h"
#include "DataTables.h"
#include "Category.h"
#include "CommandQueue.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures)
: Entity(1)
, _type(type)
, _sprite(textures.get(Table[type].texture), Table[type].textureRect)
{
	centerOrigin(_sprite);
}

unsigned int Pickup::getCategory() const
{
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

Pickup::Type Pickup::getType() const
{
	return _type;
}

void Pickup::apply(Character& player) const
{
	Table[_type].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}
