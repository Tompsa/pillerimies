#ifndef CATEGORY_H
#define CATEGORY_H

// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		Pacman = 1 << 1,
        Ghost = 1 << 2,
	};
}

#endif //CATEGORY_H
