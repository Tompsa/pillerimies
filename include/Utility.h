#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);

// Random number generation
int				randomInt(int exclusiveMax);

// Vector operations
float			length(sf::Vector2f vector);
sf::Vector2f	unitVector(sf::Vector2f vector);
float 			dot(sf::Vector2f vector1, sf::Vector2f vector2);

// Node operations
sf::Vector2f 	getPosFromNode(int x, int y);
sf::Vector2i	getNodeFromPos(sf::Vector2f pos);
int	getManhattanDistance(sf::Vector2f x, sf::Vector2f y);


#include "Utility.inl"
#endif // UTILITY_H
