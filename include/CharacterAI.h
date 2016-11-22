#ifndef CHARACTERAI_H
#define CHARACTERAI_H

#include "Command.h"
#include <SFML\System\Vector2.hpp>

#include <map>

class CommandQueue;

class CharacterAI
{
public:
	CharacterAI();
	void controlGhosts(CommandQueue& commands);
	
private:
	sf::Vector2f _pacTilePos;
	sf::Vector2f _pacDirection;
};

#endif //CHARACTERAI_H
