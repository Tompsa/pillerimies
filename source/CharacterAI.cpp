#include "CharacterAI.h"
#include "CommandQueue.h"
#include "Character.h"
#include "Foreach.h"
#include "Utility.h"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>


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
		_pacTilePos = a.getPosition() / 32.f ;
		_pacDirection = a.getDirection();

	});

	Command ghostCommand;
	ghostCommand.category = Category::Ghost;
	ghostCommand.action = derivedAction<Character>([this](Character& a, sf::Time)
	{
		float shortestDistance = 1000.f;
		float shortestDistanceAlt = 1000.f;
		sf::Vector2f newShortestDir;
		sf::Vector2f target;
		sf::Vector2f newDirAlt;

		switch (a.getCategory())
		{
		case Category::Blinky:
			target = _pacTilePos;
			break;
		case Category::Inky:
			target = _pacTilePos + _pacDirection * (2.f * 32.f);
			break;
		case Category::Pinky:
			target = _pacTilePos + _pacDirection * (4.f * 32.f);
			break;
		case Category::Clyde:
			target = _pacTilePos;
			break;
		}

		for (auto &dir : a.getValidDirections())
		{
			// Skip direction if opposite of current direction
			if (dir == -a.getDirection()) continue;

			sf::Vector2f var = a.getPosition() / 32.f + dir;
			int dist = getManhattanDistance(target, var);
			if (dist < shortestDistance)
			{
				shortestDistance = dist;
				newShortestDir = dir;
			}

			if (a.getCategory() == Category::Clyde)
			{			
				int distAlt = getManhattanDistance(sf::Vector2f(0.f, 35.f*8.f), var);
				if (distAlt < shortestDistanceAlt)
				{
					shortestDistanceAlt = distAlt;
					newDirAlt = dir;
				}
			}
		}

		if (a.getCategory() == Category::Clyde && shortestDistance <= 8)
		{
			a.setNextDirection(newDirAlt);
		}
		else
			a.setNextDirection(newShortestDir);
	});

	commands.push(pacCommand);
	commands.push(ghostCommand);
}
