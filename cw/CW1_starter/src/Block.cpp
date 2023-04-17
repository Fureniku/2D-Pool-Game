#include "Block.h"

Block::Block(b2World *world, const sf::Vector2f &position, const sf::Vector2f &size, sf::Texture &texture, int texX, int texY, int sizeX, int sizeY) {
	b2PolygonShape shape;

	body = world->CreateBody(&defineBody(position, 0));

	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	shape.m_radius = 0.f;

	body->CreateFixture(&createFixture(&shape, BOUNDARY_WALL, BALL | SLIDER_PLATFORM));
	body->SetUserData(this);

	//SFML shape definitions
	setTexture(&texture);
	setTextureRect(sf::IntRect(texX, texY, sizeX, sizeY));
	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
}

void Block::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(static_cast<sf::RectangleShape>(*this));
}