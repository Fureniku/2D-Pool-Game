#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Ray : public sf::Drawable {
private:
	b2World *world;
	sf::VertexArray line; //!< The line which represents this ray visually
	float direction = 0; //!< The direction as an angle which the ray should cast from its origin
	float maxLength = 10.f; //!< The maximum length of the line cast by the ray. Set larger than the game world so only effective if the ball somehow leaves the game world.
public:
	Ray(b2World *world, sf::Color col);
	Ray() = default;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const; //!< Draw the ray
	void cast(sf::Vector2f start); //!< Cast the ray, finding the end point based on collision
	void setDirection(float dir); //!< Adjust the direction of the ray.
};