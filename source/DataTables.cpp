#include "DataTables.h"
#include "Character.h"
#include "Pickup.h"

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(Character::TypeCount);
	
	data[Character::Pacman].hitpoints = 1;
	data[Character::Pacman].speed = 50.f;
	data[Character::Pacman].texture = Textures::Entities;
	data[Character::Pacman].textureRect = sf::IntRect(59*8, 0, 16, 16);
	
	data[Character::Blinky].hitpoints = 1;
	data[Character::Blinky].speed = 25.f;
	data[Character::Blinky].texture = Textures::Entities;
	data[Character::Blinky].textureRect = sf::IntRect(456, 65, 16, 16);
	
	data[Character::Pinky].hitpoints = 1;
	data[Character::Pinky].speed = 25.f;
	data[Character::Pinky].texture = Textures::Entities;
	data[Character::Pinky].textureRect = sf::IntRect(456, 80, 16, 16);

	data[Character::Inky].hitpoints = 1;
	data[Character::Inky].speed = 25.f;
	data[Character::Inky].texture = Textures::Entities;
	data[Character::Inky].textureRect = sf::IntRect(456, 96, 16, 16);
	
	data[Character::Clyde].hitpoints = 1;
	data[Character::Clyde].speed = 25.f;
	data[Character::Clyde].texture = Textures::Entities;
	data[Character::Clyde].textureRect = sf::IntRect(456, 112, 16, 16);
	
	return data;
}


std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	
	//data[Pickup::Pill].action = [] (Character& a) { a.collectPill(); };
	data[Pickup::Pill].action = std::bind(&Character::collectPill, _1);
	data[Pickup::Pill].texture = Textures::Entities;
	data[Pickup::Pill].textureRect = sf::IntRect(8, 8, 8, 8);
	
	//data[Pickup::SuperPill].action = [] (Character& a) { a.setSuperMode(); };
	data[Pickup::SuperPill].action = std::bind(&Character::setSuperMode, _1, true);
	data[Pickup::SuperPill].texture = Textures::Entities;
	data[Pickup::SuperPill].textureRect = sf::IntRect(8, 24, 8, 8);
	
	data[Pickup::Fruit].action = [] (Character& a) { a.collectPill(); };
	data[Pickup::Fruit].texture = Textures::Entities;
	data[Pickup::Fruit].textureRect = sf::IntRect(488, 48, 16, 16);
	
	return data;
}
