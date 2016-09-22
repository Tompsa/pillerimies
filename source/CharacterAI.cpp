#include "CharacterAI.h"
#include "CommandQueue.h"
#include "Character.h"
#include "Foreach.h"
#include "Utility.h"

#include <map>
#include <string>
#include <algorithm>


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
	commands.push(_actionBinding[TurnRight]);
}
