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

private:
		//enum NodeType
		//{
			//Pill = 1,
			//SuperPill,
			//Wall,			
			//GhostSpawn,
		//};
		
		enum NodeType
		{
			Wall,
			Pill,
			Empty,
			SuperPill,					
			GhostSpawn,
			LeftTunnel,
			RightTunnel,
			Gate,
		};

private: 
		enum WallType
		{
			LeftVertical,
			RightVertical,
			UpHorizontal,
			DownHorizontal,
			UpLeftCorner,
			UpRightCorner,
			DownLeftCorner,
			DownRightCorner,
			SolidWallLeft,
			SolidWallRight,
			SolidWallUp,
			SolidWallDown,
			SolidCornerUpLeft,
			SolidCornerUpRight,
			SolidCornerDownLeft,
			SolidCornerDownRight	
		};

	private:
		void								loadTextures();
		void								handleCollisions();
		
		bool								loadMap(const std::string& path);
		
		void								buildScene();
		void								addWalls();
		void 								addWall(World::WallType type, sf::Vector2f pos);
		void 								addGhosts();
		void								addPills();
		sf::FloatRect						getViewBounds() const;
		void								checkCharacterDirections();
		bool 								checkDirection(sf::Vector2f position, sf::Vector2f direction);
        
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
		unsigned char 						_map[28][31];
        int                                 _playerScore;
        int                                 _playerLives;
		std::vector<Character*>				_activeGhosts;
		std::vector<Pickup*>				_remainingPills;
        
        TextNode*                           _scoreDisplay;
        TextNode*                           _livesDisplay;
        TextNode*                           _debugDisplay;
};

#endif //WORLD_H
