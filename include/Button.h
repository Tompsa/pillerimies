#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace GUI
{

class Button : public Component
{
    public:
        typedef std::shared_ptr<Button>		Ptr;
        typedef std::function<void()>		Callback;


	public:
								Button(const FontHolder& fonts, const TextureHolder& textures);

        void					setCallback(Callback callback);
        void					setText(const std::string& text);
        void					setToggle(bool flag);

        virtual bool			isSelectable() const;
        virtual void			select();
        virtual void			deselect();

        virtual void			activate();
        virtual void			deactivate();

        virtual void			handleEvent(const sf::Event& event);


    private:
        virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;


    private:
        Callback				_callback;
        const sf::Texture&		_normalTexture;
        const sf::Texture&		_selectedTexture;
        const sf::Texture&		_pressedTexture;
        sf::Sprite				_sprite;
        sf::Text				_text;
        bool					_isToggle;
};

}

#endif // BUTTON_H
