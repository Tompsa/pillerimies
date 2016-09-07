#include "DataTables.h"
#include "Character.h"

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
	data[Character::Blinky].speed = 300.f;
	data[Character::Blinky].texture = Textures::Entities;
	data[Character::Blinky].textureRect = sf::IntRect(292, 171, 22, 26);
	
	data[Character::Pinky].hitpoints = 1;
	data[Character::Pinky].speed = 300.f;
	data[Character::Pinky].texture = Textures::Entities;
	data[Character::Pinky].textureRect = sf::IntRect(292, 171, 22, 26);

	data[Character::Inky].hitpoints = 1;
	data[Character::Inky].speed = 300.f;
	data[Character::Inky].texture = Textures::Entities;
	data[Character::Inky].textureRect = sf::IntRect(292, 171, 22, 26);
	
	data[Character::Clyde].hitpoints = 1;
	data[Character::Clyde].speed = 300.f;
	data[Character::Clyde].texture = Textures::Entities;
	data[Character::Clyde].textureRect = sf::IntRect(292, 171, 22, 26);
	
	return data;
}
