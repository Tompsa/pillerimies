#ifndef DATATABLES_H
#define DATATABLES_H

#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>


//class Pacman;

struct CharacterData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
};

std::vector<CharacterData> initializeCharacterData();

#endif // DATATABLES_H
