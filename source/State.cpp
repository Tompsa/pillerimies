#include "State.h"
#include "StateStack.h"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, CharacterAI& ai)
: window(&window)
, textures(&textures)
, fonts(&fonts)
, player(&player)
, ai(&ai)
{
}

State::State(StateStack& stack, Context context)
: _stack(&stack)
, _context(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	_stack->pushState(stateID);
}

void State::requestStackPop()
{
	_stack->popState();
}

void State::requestStateClear()
{
	_stack->clearStates();
}

State::Context State::getContext() const
{
	return _context;
}
