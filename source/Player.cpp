#include "Player.h"
#include "CommandQueue.h"
#include "Character.h"
#include "Foreach.h"
#include "Utility.h"

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

Player::Player()
: _currentMissionStatus(MissionRunning)
{
	// Set initial key bindings
	_keyBinding[sf::Keyboard::Left] = TurnLeft;
	_keyBinding[sf::Keyboard::Right] = TurnRight;
	_keyBinding[sf::Keyboard::Up] = TurnUp;
	_keyBinding[sf::Keyboard::Down] = TurnDown;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's character
	FOREACH(auto& pair, _actionBinding)
		pair.second.category = Category::Pacman;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = _keyBinding.find(event.key.code);
		if (found != _keyBinding.end() && !isRealtimeAction(found->second))
			commands.push(_actionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, _keyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(_actionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = _keyBinding.begin(); itr != _keyBinding.end();)
	{
		if (itr->second == action)
			_keyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	_keyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, _keyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(MissionStatus status)
{
	_currentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return _currentMissionStatus;
}


void Player::initializeActions()
{
    _actionBinding[TurnLeft].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(-1,0)); });
    _actionBinding[TurnRight].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(+1,0)); });
    _actionBinding[TurnUp].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(0,-1)); });
    _actionBinding[TurnDown].action = derivedAction<Character>([] (Character& a, sf::Time){ a.setNextDirection(sf::Vector2f(0,1)); });  
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case TurnLeft:
	case TurnRight:
	case TurnUp:
	case TurnDown:
		return true;

	default:
		return false;
	}
}
