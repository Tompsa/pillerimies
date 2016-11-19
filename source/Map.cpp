#include "Map.h"
#include "SpriteNode.h"

#include <SFML\Graphics\RenderWindow.hpp>

#include <fstream>
#include <cctype>
#include <string>


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
		std::string str;
		file >> str;
		char j = str[0], k = str[2];
		if (!isdigit(k))
			_map[x][y] = sf::Vector2i(j-'0', -1);
		else
			_map[x][y] = sf::Vector2i(j - '0', k - '0');
	}
	return true;
}

void Map::addWalls(TextureHolder &textures, SceneNode &mazeLayer)
{
	sf::Texture& wallTexture = textures.get(Textures::MazeTiles);

	for (int i = 0; i < 28; ++i) for (int j = 0; j < 31; ++j)
	{
		if (_map[i][j].y != -1)
		{
			sf::IntRect rect(_map[i][j].x * 32, _map[i][j].y * 32, 32, 32);
			std::unique_ptr<SpriteNode> wallSprite(new SpriteNode(wallTexture, rect));
			wallSprite->setPosition(i*32, j*32);
			mazeLayer.attachChild(std::move(wallSprite));
		}

	}

}

bool Map::isPillTile(int x, int y) const
{
	if (_map[x][y].x == Pill && _map[x][y].y == -1)
		return true;
	else
		return false;
}

bool Map::isSuperPillTile(int x, int y) const
{
	if (_map[x][y].x == SuperPill && _map[x][y].y == -1)
		return true;
	else
		return false;
}

bool Map::isGateTile(sf::Vector2i pos) const
{
	if (_map[pos.x][pos.y].x == Gate && _map[pos.x][pos.y].y == -1)
		return true;
	else
		return false;
}

bool Map::isGateTile(int x, int y) const
{
	if (_map[x][y].x == Gate && _map[x][y].y == -1)
		return true;
	else
		return false;
}

bool Map::isGhostSpawnTile(int x, int y) const
{
	if (_map[x][y].x == GhostSpawn && _map[x][y].y == -1)
		return true;
	else
		return false;
}

bool Map::isTunnelTile(int x, int y) const
{
	if ((_map[x][y].x == LeftTunnel || _map[x][y].x == RightTunnel) && _map[x][y].y == -1)
		return true;
	else
		return false;
}

bool Map::isEnterableTile(sf::Vector2i pos) const
{
	if (_map[pos.x][pos.y].x != Wall && _map[pos.x][pos.y].y == -1)
		return true;
	else
		return false;
}

bool Map::isEnterableTile(int x, int y) const
{
	if (_map[x][y].x != Wall && _map[x][y].y == -1)
		return true;
	else
		return false;
}
