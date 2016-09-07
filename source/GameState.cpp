#include "GameState.h"


GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, _world(*context.window, *context.fonts)
, _player(*context.player)
{
	_player.setMissionStatus(Player::MissionRunning);
}

void GameState::draw()
{
	_world.draw();
}

bool GameState::update(sf::Time dt)
{
	_world.update(dt);

	if (!_world.hasAlivePlayer())
	{
		_player.setMissionStatus(Player::MissionFailure);
		requestStackPush(States::GameOver);
	}
	//else if (_world.hasPlayerReachedEnd())
	//{
		//_player.setMissionStatus(Player::MissionSuccess);
		//requestStackPush(States::GameOver);
	//}

	CommandQueue& commands = _world.getCommandQueue();
	_player.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = _world.getCommandQueue();
	_player.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}
