#ifndef WORLD_H
#define WORLD_H

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Character.h"
#include "CommandQueue.h"
#include "Command.h"
#include "TextNode.h"
#include "Map.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

namespace sf
{
	class RenderWindow;
}

class Pickup;

class World : private sf::NonCopyable
{
	public:
		explicit							World(sf::RenderWindow& window, FontHolder& fonts);
		void								update(sf::Time dt);
		void								draw();
		
		CommandQueue&						getCommandQueue();

		bool 								hasAlivePlayer() const;
		bool								hasPlayerReachedEnd() const;

	private:
		void								loadTextures();
		void								handleCollisions();
		
		void								buildScene();
		void 								addGhosts();
		void								addPills();
		sf::FloatRect						getViewBounds() const;
		void								checkCharacterDirections();
		bool 								checkDirection(sf::Vector2f position, sf::Vector2f direction, Character& ch);
		void								updateGhostStatus();
        
        void                                updateTexts();

	private:
		enum Layer
		{
			Background,
			MazeLayer,
			EntityLayer,
			LayerCount
		};

	private:
		sf::RenderWindow&					_window;
		sf::View							_worldView;
		TextureHolder						_textures;
		FontHolder&							_fonts;

		SceneNode							_sceneGraph;
		std::array<SceneNode*, LayerCount>	_sceneLayers;
		CommandQueue						_commandQueue;

		sf::FloatRect						_worldBounds;
		sf::Vector2f						_spawnPosition;
		Character*							_pacman;
		Map									_map;
        int                                 _playerScore;
        int                                 _playerLives;
		std::vector<Character*>				_activeGhosts;
		std::vector<Pickup*>				_remainingPills;
        
        TextNode*                           _scoreDisplay;
        TextNode*                           _livesDisplay;
};

#endif //WORLD_H
