#ifndef MAP_H
#define MAP_H

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"

#include <array>
#include <string>

class Map
{
public:
	Map();
	bool								loadMap(const std::string& path);
	void								addWalls(TextureHolder &textures, SceneNode &mazeLayer);

	bool								isPillTile(int x, int y) const;
	bool								isSuperPillTile(int x, int y) const;
	bool								isGateTile(sf::Vector2i pos) const;
	bool								isGateTile(int x, int y) const;
	bool								isGhostSpawnTile(int x, int y) const;
	bool								isTunnelTile(int x, int y) const;

	bool								isEnterableTile(sf::Vector2i pos) const;
	bool								isEnterableTile(int x, int y) const;


private:
	enum TileType
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
	sf::Vector2i _map[28][31];
};

#endif // !MAP_H

