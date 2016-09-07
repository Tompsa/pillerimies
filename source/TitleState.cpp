#include "TitleState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, _text()
, _showText(true)
, _textEffectTime(sf::Time::Zero)
{
	_backgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	_text.setFont(context.fonts->get(Fonts::Main));
	_text.setString("Press any key to start");
	centerOrigin(_text);
	_text.setPosition(sf::Vector2f(context.window->getSize() / 2u));
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(_backgroundSprite);

	if (_showText)
		window.draw(_text);
}

bool TitleState::update(sf::Time dt)
{
	_textEffectTime += dt;

	if (_textEffectTime >= sf::seconds(0.5f))
	{
		_showText = !_showText;
		_textEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}
