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

		if (a.getStatus() == Character::Super &&  a.getStateTimer().asSeconds() >= 5)
			a.setStatus(Character::Regular);

	});

	Command ghostCommand;
	ghostCommand.category = Category::Ghost;
	ghostCommand.action = derivedAction<Character>([this](Character& a, sf::Time)
	{
		int shortestDistance = 1000;
		int shortestDistanceAlt = 1000;
		sf::Vector2f newShortestDir;
		sf::Vector2f target;
		sf::Vector2f newDirAlt;

		if (a.getStatus() == Character::Scared)
		{
			// If player has eaten SuperPill, ghosts take random turns at intersections
			int randomIndex = rand() % static_cast<unsigned int>(a.getValidDirections().size());

			for (int i = 0; i < a.getValidDirections().size(); ++i)
			{
				if (a.getValidDirections().at(i) == -a.getDirection()) continue;

				if (i == randomIndex)
					newShortestDir = a.getValidDirections().at(i);
			}

			if (a.getStateTimer().asSeconds() >= 5)
				a.setStatus(Character::Regular);
		}

		else
		{
			if (a.getStatus() == Character::Regular)
			{
				switch (a.getCategory())
				{
				case Category::Blinky:
					target = _pacTilePos;
					break;
				case Category::Inky:
					target = _pacTilePos + _pacDirection * (2.f );
					break;
				case Category::Pinky:
					target = _pacTilePos + _pacDirection * (4.f );
					break;
				case Category::Clyde:
					target = _pacTilePos;
					break;
				}
			}

			if (a.getStatus() == Character::Scatter)
			{
				switch (a.getCategory())
				{
				case Category::Blinky:
					target = { 25.f , 0.f  };;
					break;
				case Category::Inky:
					target = { 27.f , 35.f  };;
					break;
				case Category::Pinky:
					target = { 2.f , 0.f  };
					break;
				case Category::Clyde:
					target = { 0.f , 35.f };
					break;
				}
				if (a.getStateTimer().asSeconds() >= 7)
					a.setStatus(Character::Regular);
			}

			if (a.getStatus() == Character::Eaten)
			{
				target = { 14.f , 14.f };
			}

			if (a.getStatus() == Character::InSpawn)
			{
				target = { 14.f , 14.f };
				if (a.getStateTimer().asSeconds() >= 7)
					a.setStatus(Character::Regular);
			}

			
			for (auto &dir : a.getValidDirections())
			{
				// Skip direction if opposite of current direction
				if (dir == -a.getDirection()) continue;

				// Tile of targeted direction
				sf::Vector2f var = a.getPosition() / 32.f + dir;

				int dist = getManhattanDistance(target, var);
				if (dist < shortestDistance)
				{
					shortestDistance = dist;
					newShortestDir = dir;
				}

				if (a.getCategory() == Category::Clyde && a.getStatus() != Character::Eaten)
				{
					int distAlt = getManhattanDistance(sf::Vector2f(0.f, 35.f*8.f), var);
					if (distAlt < shortestDistanceAlt)
					{
						shortestDistanceAlt = distAlt;
						newDirAlt = dir;
					}
				}
			}
		}

		if (a.getCategory() == Category::Clyde && shortestDistance <= 8 && a.getStatus() != Character::Eaten)
		{
			a.setNextDirection(newDirAlt);
		}
		else
			a.setNextDirection(newShortestDir);
	});

	commands.push(pacCommand);
	commands.push(ghostCommand);
}
