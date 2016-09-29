#include "CharacterAI.h"
#include "CommandQueue.h"
#include "Character.h"
#include "Foreach.h"
#include "Utility.h"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>


using namespace std::placeholders;

CharacterAI::CharacterAI()
{
	initializeActions();
	
	FOREACH(auto& pair, _actionBinding)
		pair.second.category = Category::Blinky;
}

void CharacterAI::initializeActions()
{
    _actionBinding[TurnLeft].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(-1,0)); });
    _actionBinding[TurnRight].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(+1,0)); });
    _actionBinding[TurnUp].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(0,-1)); });
    _actionBinding[TurnDown].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(0,1)); });  
}


void CharacterAI::controlGhosts(CommandQueue& commands)
{
	Command pacCommand;
	pacCommand.category = Category::Pacman;
	pacCommand.action = derivedAction<Character>([this](Character& a, sf::Time)
	{
		_pacTilePos = a.getPosition() / 8.f ;

	});

	Command ghostCommand;
	ghostCommand.category = Category::Ghost;
	ghostCommand.action = derivedAction<Character>([this](Character& a, sf::Time)
	{
		int shortestDistance = 1000;
		sf::Vector2f newDir;

			for (auto &dir : a.getValidDirections())
			{
				sf::Vector2f var = a.getPosition()/8.f + dir;
				int dist = getManhattanDistance(_pacTilePos, var);
				if (dist < shortestDistance)
				{
					shortestDistance = dist;
					newDir = dir;
				}
			}
			a.setNextDirection(newDir);
	});

	commands.push(pacCommand);
	commands.push(ghostCommand);
}
