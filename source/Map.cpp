#include "Map.h"
#include "SpriteNode.h"

#include <SFML\Graphics\RenderWindow.hpp>

#include <fstream>


Map::Map()
	: _map()
{

}

bool Map::loadMap(const std::string & path)
{
	std::ifstream file(path.c_str());
	if (!file.is_open()) return false;

	for (int y = 0; y<31; ++y) for (int x = 0; x<28; ++x)
	{
		if (file.eof()) return false;
		int current;
		file >> current;
		_map[x][y] = current;
	}
	return true;
}

void Map::addWalls(TextureHolder &textures, SceneNode &mazeLayer)
{
	//Outer wall corners
	if ((_map[0][0] == Wall))
		addWall(textures, mazeLayer,WallType::UpLeftCorner, sf::Vector2f(0, 0));
	if ((_map[27][0] == Wall))
		addWall(textures, mazeLayer, WallType::UpRightCorner, sf::Vector2f(27 * 8, 0));
	if ((_map[0][30] == Wall))
		addWall(textures, mazeLayer, WallType::DownLeftCorner, sf::Vector2f(0, 30 * 8));
	if ((_map[27][30] == Wall))
		addWall(textures, mazeLayer, WallType::DownRightCorner, sf::Vector2f(27 * 8, 30 * 8));

	// Walls on the top and low
	for (int x = 1; x < 27; ++x)
	{
		if ((_map[x][0] == Wall) && (_map[x + 1][0] == Wall))
		{
			addWall(textures, mazeLayer, WallType::UpHorizontal, sf::Vector2f(x * 8, 0));
		}
		if ((_map[x][30] == Wall) && (_map[x + 1][30] == Wall))
		{
			addWall(textures, mazeLayer, WallType::DownHorizontal, sf::Vector2f(x * 8, 30 * 8));
		}
	}
	// Walls on the right and left
	for (int y = 1; y < 30; ++y)
	{
		if ((_map[0][y] == Wall) && (_map[0][y + 1] == Wall))
		{
			addWall(textures, mazeLayer, WallType::LeftVertical, sf::Vector2f(0, y * 8));
		}
		if ((_map[27][y] == Wall) && (_map[27][y + 1] == Wall))
		{
			addWall(textures, mazeLayer, WallType::RightVertical, sf::Vector2f(27 * 8, y * 8));
		}
	}
	for (int x = 1; x<27; ++x) for (int y = 1; y<30; ++y)
	{
		if (_map[x][y] == Wall)
		{
			// Inner walls
			if (_map[x - 1][y] == Wall && _map[x + 1][y] == Wall && _map[x][y + 1] == Wall && _map[x][y - 1] != Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidWallUp, sf::Vector2f(x * 8, y * 8));
			}
			if (_map[x - 1][y] == Wall && _map[x + 1][y] == Wall && _map[x][y + 1] != Wall && _map[x][y - 1] == Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidWallDown, sf::Vector2f(x * 8, y * 8));
			}
			if (_map[x - 1][y] != Wall && _map[x + 1][y] == Wall && _map[x][y + 1] == Wall && _map[x][y - 1] == Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidWallLeft, sf::Vector2f(x * 8, y * 8));
			}
			if (_map[x - 1][y] == Wall && _map[x + 1][y] != Wall && _map[x][y + 1] == Wall && _map[x][y - 1] == Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidWallRight, sf::Vector2f(x * 8, y * 8));
			}

			// Corners
			if (_map[x - 1][y] != Wall && _map[x + 1][y] == Wall && _map[x][y + 1] == Wall && _map[x][y - 1] != Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidCornerUpLeft, sf::Vector2f(x * 8, y * 8));
			}
			if (_map[x - 1][y] == Wall && _map[x + 1][y] != Wall && _map[x][y + 1] == Wall && _map[x][y - 1] != Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidCornerUpRight, sf::Vector2f(x * 8, y * 8));
			}
			if (_map[x - 1][y] != Wall && _map[x + 1][y] == Wall && _map[x][y + 1] != Wall && _map[x][y - 1] == Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidCornerDownLeft, sf::Vector2f(x * 8, y * 8));
			}
			if (_map[x - 1][y] == Wall && _map[x + 1][y] != Wall && _map[x][y + 1] != Wall && _map[x][y - 1] == Wall)
			{
				addWall(textures, mazeLayer, WallType::SolidCornerDownRight, sf::Vector2f(x * 8, y * 8));
			}
		}
	}

}

bool Map::isPillTile(int x, int y) const
{
	if (_map[x][y] == Pill)
		return true;
	else
		return false;
}

bool Map::isSuperPillTile(int x, int y) const
{
	if (_map[x][y] == SuperPill)
		return true;
	else
		return false;
}

bool Map::isEnterableTile(sf::Vector2i pos) const
{
	if (_map[pos.x][pos.y] != Wall && _map[pos.x][pos.y] != Gate)
		return true;
	else
		return false;
}

bool Map::isEnterableTile(int x, int y) const
{
	if (_map[x][y] != (Wall || Gate))
		return true;
	else
		return false;
}

void Map::addWall(TextureHolder &textures, SceneNode &mazeLayer, WallType type, sf::Vector2f pos)
{
	sf::Texture& wallTexture = textures.get(Textures::Entities);
	sf::IntRect rect;

	switch (type)
	{
	case LeftVertical:
		rect = sf::IntRect(0, 8, 8, 8);
		break;
	case RightVertical:
		rect = sf::IntRect(27 * 8, 8, 8, 8);
		break;
	case UpHorizontal:
		rect = sf::IntRect(8, 0, 8, 8);
		break;
	case DownHorizontal:
		rect = sf::IntRect(8, 30 * 8, 8, 8);
		break;
	case UpLeftCorner:
		rect = sf::IntRect(0, 0, 8, 8);
		break;
	case UpRightCorner:
		rect = sf::IntRect(27 * 8, 0, 8, 8);
		break;
	case DownLeftCorner:
		rect = sf::IntRect(0, 30 * 8, 8, 8);
		break;
	case DownRightCorner:
		rect = sf::IntRect(27 * 8, 9 * 8, 8, 8);
		break;
	case SolidWallLeft:
		rect = sf::IntRect(2 * 8, 3 * 8, 8, 8);
		break;
	case SolidWallRight:
		rect = sf::IntRect(5 * 8, 3 * 8, 8, 8);
		break;
	case SolidWallUp:
		rect = sf::IntRect(3 * 8, 2 * 8, 8, 8);
		break;
	case SolidWallDown:
		rect = sf::IntRect(3 * 8, 4 * 8, 8, 8);
		break;
	case SolidCornerUpLeft:
		rect = sf::IntRect(2 * 8, 2 * 8, 8, 8);
		break;
	case SolidCornerUpRight:
		rect = sf::IntRect(5 * 8, 2 * 8, 8, 8);
		break;
	case SolidCornerDownLeft:
		rect = sf::IntRect(2 * 8, 4 * 8, 8, 8);
		break;
	case SolidCornerDownRight:
		rect = sf::IntRect(5 * 8, 4 * 8, 8, 8);
		break;
	}

	std::unique_ptr<SpriteNode> wallSprite(new SpriteNode(wallTexture, rect));
	wallSprite->setPosition(pos);
	mazeLayer.attachChild(std::move(wallSprite));
}

