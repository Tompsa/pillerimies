#ifndef CHARACTERAI_H
#define CHARACTERAI_H

#include "Command.h"

#include <map>

class CommandQueue;

class CharacterAI
{
public:
	enum Action
	{
		TurnLeft,
		TurnRight,
		TurnUp,
		TurnDown,
		ActionCount
	};
	
public:
	CharacterAI();
	void controlGhosts(CommandQueue& commands);

private:
	void initializeActions();
	
private:
	std::map<Action, Command> _actionBinding;
};

#endif //CHARACTERAI_H
