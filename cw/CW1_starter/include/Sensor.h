#pragma once
#include "PhysicalObject.h"

class Sensor : public sf::RectangleShape, public PhysicalObject {
private:
	std::vector<PhysicalObject*> objectList; //!< The objects which will be triggered by this sensors actions
	bool isPocket = false; //!< Is this a pocket, or something else
public:
	Sensor() = default;
	Sensor(b2World *world, const sf::Vector2f &position, const sf::Vector2f &size, sf::Color col, const std::vector<PhysicalObject*> objects);
	Sensor(b2World *world, const sf::Vector2f &position, const sf::Vector2f &size, sf::Color col);
	void sensorEnter() override {} //!< Called when something enters the connected sensor. Not used as this is the sensor itself.
	void sensorExit() override {} //!< Called when something exits the connected sensor. Not used as this is the sensor itself.
	void handleSensor(bool enter); //!< Called when anything happens to the sensor
	bool isPocketSensor() { return isPocket; } //!< Checks if this sensor is specifically for pocketing balls
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override; //!< Draw the sensors graphical object
};