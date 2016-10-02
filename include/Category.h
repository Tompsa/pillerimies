#ifndef CATEGORY_H
#define CATEGORY_H

// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None 	= 0,
		Scene 	= 1 << 0,
		Pacman 	= 1 << 1,
		Blinky 	= 1 << 2,
		Pinky 	= 1 << 3,
		Inky 	= 1 << 4,
		Clyde 	= 1 << 5,
		Pickup 	= 1 << 6,
		Ghost = Blinky | Pinky | Inky | Clyde,
		Character = Pacman | Ghost,
	};
}

#endif //CATEGORY_H
