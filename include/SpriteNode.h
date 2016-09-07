#ifndef SPRITENODE_H
#define SPRITENODE_H

#include "SceneNode.h"

#include <SFML/Graphics/Sprite.hpp>


class SpriteNode : public SceneNode
{
public:
	explicit			SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
	void centerSprite();

private:
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	sf::Sprite			_sprite;
};

#endif // SPRITENODE_H
