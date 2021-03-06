#include "Button.h"
#include "Utility.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

Button::Button(const FontHolder& fonts, const TextureHolder& textures)
: _callback()
, _normalTexture(textures.get(Textures::ButtonNormal))
, _selectedTexture(textures.get(Textures::ButtonSelected))
, _pressedTexture(textures.get(Textures::ButtonPressed))
, _sprite()
, _text("", fonts.get(Fonts::Main), 16)
, _isToggle(false)
{
	_sprite.setTexture(_normalTexture);

	sf::FloatRect bounds = _sprite.getLocalBounds();
	_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void Button::setCallback(Callback callback)
{
	_callback = std::move(callback);
}

void Button::setText(const std::string& text)
{
	_text.setString(text);
	centerOrigin(_text);
}

void Button::setToggle(bool flag)
{
	_isToggle = flag;
}

bool Button::isSelectable() const
{
    return true;
}

void Button::select()
{
	Component::select();

	_sprite.setTexture(_selectedTexture);
}

void Button::deselect()
{
	Component::deselect();

	_sprite.setTexture(_normalTexture);
}

void Button::activate()
{
	Component::activate();

    // If we are toggle then we should show that the button is pressed and thus "toggled".
	if (_isToggle)
		_sprite.setTexture(_pressedTexture);

	if (_callback)
		_callback();

    // If we are not a toggle then deactivate the button since we are just momentarily activated.
	if (!_isToggle)
		deactivate();
}

void Button::deactivate()
{
	Component::deactivate();

	if (_isToggle)
	{
        // Reset texture to right one depending on if we are selected or not.
		if (isSelected())
			_sprite.setTexture(_selectedTexture);
		else
			_sprite.setTexture(_normalTexture);
	}
}

void Button::handleEvent(const sf::Event&)
{
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(_sprite, states);
	target.draw(_text, states);
}

}
