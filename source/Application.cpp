#include "Application.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SettingsState.h"
#include "GameOverState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: _window(sf::VideoMode(640, 640), "pillerimies", sf::Style::Close)
, _textures()
, _fonts()
, _player()
, _ai()
, _stateStack(State::Context(_window, _textures, _fonts, _player, _ai))
, _statisticsText()
, _statisticsUpdateTime()
, _statisticsNumFrames(0)
{
	_window.setKeyRepeatEnabled(false);

	_fonts.load(Fonts::Main, "Media/OpenSans-Regular.ttf");
    
	_textures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
    _textures.load(Textures::ButtonNormal, "Media/Textures/ButtonNormal.png");
	_textures.load(Textures::ButtonSelected, "Media/Textures/ButtonSelected.png");
	_textures.load(Textures::ButtonPressed,	"Media/Textures/ButtonPressed.png");

	_statisticsText.setFont(_fonts.get(Fonts::Main));
	_statisticsText.setPosition(5.f, 500.f);
	_statisticsText.setCharacterSize(10u);

	registerStates();
	_stateStack.pushState(States::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (_stateStack.isEmpty())
				_window.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		_stateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			_window.close();
	}
}

void Application::update(sf::Time dt)
{
	_stateStack.update(dt);
}

void Application::render()
{
	_window.clear();

	_stateStack.draw();

	_window.setView(_window.getDefaultView());
	_window.draw(_statisticsText);

	_window.display();
}

void Application::updateStatistics(sf::Time dt)
{
	_statisticsUpdateTime += dt;
	_statisticsNumFrames += 1;
	if (_statisticsUpdateTime >= sf::seconds(1.0f))
	{
		_statisticsText.setString("FPS: " + toString(_statisticsNumFrames));

		_statisticsUpdateTime -= sf::seconds(1.0f);
		_statisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	_stateStack.registerState<TitleState>(States::Title);
	_stateStack.registerState<MenuState>(States::Menu);
	_stateStack.registerState<GameState>(States::Game);
	_stateStack.registerState<PauseState>(States::Pause);
    _stateStack.registerState<SettingsState>(States::Settings);
	_stateStack.registerState<GameOverState>(States::GameOver);
}
