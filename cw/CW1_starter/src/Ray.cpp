#include "Ray.h"

Ray::Ray(b2World *world, sf::Color col) {
	this->world = world;

	line.resize(2);
	line.setPrimitiveType(sf::Lines);
	line[0].color = col;
	line[1].color = col;
}

// Pretty much just from the demo code. Not much to change/improve...
void Ray::cast(sf::Vector2f start) {
	// Calculate ray from start in direction with max length
	b2Vec2 p1 = b2Vec2(start.x, start.y);
	b2Vec2 p2 = p1 + maxLength * b2Vec2(cosf(direction), sinf(direction));

	// Cast ray
	b2RayCastInput input;
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1; // t = 1

	// Check every fixture of every body to find closest
	float closestFraction = input.maxFraction;
	b2Vec2 intersectionNormal(0, 0); // useful if you want to cast additional reflected rays
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			b2RayCastOutput output;
			if (fixture->RayCast(&output, input, 1) && output.fraction < closestFraction && !fixture->IsSensor()) {
				closestFraction = output.fraction;
				intersectionNormal = output.normal;
			}
		}
	}

	// Parametric line calculation
	b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);

	// Set render points
	line[0].position = start;
	line[1].position = sf::Vector2f(intersectionPoint.x, intersectionPoint.y);
}

void Ray::setDirection(float dir) {
	direction = dir;
}

void Ray::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(line);
}