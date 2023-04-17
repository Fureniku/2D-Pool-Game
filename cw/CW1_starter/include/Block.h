#pragma once
#include "PhysicalObject.h"

class Block : public sf::RectangleShape, public PhysicalObject {
public:
	Block(b2World *world, const sf::Vector2f &position, const sf::Vector2f &size, sf::Texture &texture, int texX, int texY, int sizeX, int sizeY);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};