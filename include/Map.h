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
	void 								addWall(TextureHolder &textures, SceneNode &mazeLayer, WallType type, sf::Vector2f pos);

private:
	unsigned char _map[28][31];


};

#endif // !MAP_H

