#include "SettingsState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, _GUIContainer()
{
	_backgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	
	// Build key binding buttons and labels
	addButtonLabel(Player::TurnLeft,  150.f, "Turn Left", context);
	addButtonLabel(Player::TurnRight, 200.f, "Turn Right", context);
	addButtonLabel(Player::TurnUp,  250.f, "Turn Up", context);
	addButtonLabel(Player::TurnDown,  300.f, "Turn Down", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(80.f, 420.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	_GUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(_backgroundSprite);
	window.draw(_GUIContainer);
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < Player::ActionCount; ++action)
	{
		if (_bindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				_bindingButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		_GUIContainer.handleEvent(event);

	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < Player::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		_bindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
	_bindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	_bindingButtons[action]->setPosition(80.f, y);
	_bindingButtons[action]->setText(text);
	_bindingButtons[action]->setToggle(true);

	_bindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	_bindingLabels[action]->setPosition(300.f, y + 15.f);

	_GUIContainer.pack(_bindingButtons[action]);
	_GUIContainer.pack(_bindingLabels[action]);
}
