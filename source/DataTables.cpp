#include "DataTables.h"
#include "Character.h"
#include "Pickup.h"

// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(Character::TypeCount);
	
	data[Character::Pacman].hitpoints = 1;
	data[Character::Pacman].speed = 150.f;
	data[Character::Pacman].texture = Textures::Entities;
	data[Character::Pacman].textureRect = sf::IntRect(10*32, 0, 32, 32);
	
	data[Character::Blinky].hitpoints = 1;
	data[Character::Blinky].speed = 100.f;
	data[Character::Blinky].texture = Textures::Entities;
	data[Character::Blinky].textureRect = sf::IntRect(0, 0, 32, 32);
	
	data[Character::Pinky].hitpoints = 1;
	data[Character::Pinky].speed = 100.f;
	data[Character::Pinky].texture = Textures::Entities;
	data[Character::Pinky].textureRect = sf::IntRect(4*32, 0, 32, 32);

	data[Character::Inky].hitpoints = 1;
	data[Character::Inky].speed = 100.f;
	data[Character::Inky].texture = Textures::Entities;
	data[Character::Inky].textureRect = sf::IntRect(6*32, 0, 32, 32);
	
	data[Character::Clyde].hitpoints = 1;
	data[Character::Clyde].speed = 100.f;
	data[Character::Clyde].texture = Textures::Entities;
	data[Character::Clyde].textureRect = sf::IntRect(2*32, 0, 32, 32);
	
	return data;
}


std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	
	//data[Pickup::Pill].action = [] (Character& a) { a.collectPill(); };
	data[Pickup::Pill].action = std::bind(&Character::collectPill, _1);
	data[Pickup::Pill].texture = Textures::MazeTiles;
	data[Pickup::Pill].textureRect = sf::IntRect(9*32, 2*32, 32, 32);
	
	//data[Pickup::SuperPill].action = [] (Character& a) { a.setSuperMode(); };
	data[Pickup::SuperPill].action = std::bind(&Character::setStatus, _1, Character::Status::Super);
	data[Pickup::SuperPill].texture = Textures::MazeTiles;
	data[Pickup::SuperPill].textureRect = sf::IntRect(7*32, 2*32, 32, 32);
	
	data[Pickup::Fruit].action = [] (Character& a) { a.collectPill(); };
	data[Pickup::Fruit].texture = Textures::MazeTiles;
	data[Pickup::Fruit].textureRect = sf::IntRect(8, 2, 32, 32);
	
	return data;
}
