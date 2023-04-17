#include "Sensor.h"

Sensor::Sensor(b2World *world, const sf::Vector2f &position, const sf::Vector2f &size, sf::Color col, const std::vector<PhysicalObject*> objects) {
	b2PolygonShape shape;
	std::cout << "Creating a sensor with " << objects.size() << " objects." << std::endl;
	objectList = objects;

	body = world->CreateBody(&defineBody(position, 0));

	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	shape.m_radius = 0.f;

	body->CreateFixture(&createFullFixture(&shape, true, BOUNDARY_WALL, BALL | SLIDER_PLATFORM));

	//SFML shape definitions
	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
	setRotation(0);
	setFillColor(col);
}

Sensor::Sensor(b2World *world, const sf::Vector2f &position, const sf::Vector2f &size, sf::Color col) {
	b2PolygonShape shape;
	this->isPocket = true;

	body = world->CreateBody(&defineBody(position, 0));

	shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	shape.m_radius = 0.f;

	body->CreateFixture(&createFullFixture(&shape, true, BOUNDARY_WALL, BALL | SLIDER_PLATFORM));

	//SFML shape definitions
	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
	setRotation(0);
	setFillColor(col);
}

void Sensor::handleSensor(bool enter) {
	std::cout << "Handling sensor, processing " << objectList.size() << " objects" << std::endl;
	for (int i = 0; i < objectList.size(); i++) {
		if (enter) {
			objectList[i]->sensorEnter();
		}
		else {
			objectList[i]->sensorExit();
		}
		
	}
}

void Sensor::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(static_cast<sf::RectangleShape>(*this));
}