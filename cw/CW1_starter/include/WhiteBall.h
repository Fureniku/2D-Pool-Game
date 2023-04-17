#pragma once
#include "DynamicPolygon.h"

class WhiteBall : public DynamicPolygon {
private:
	float direction = 0.f;
public:
	WhiteBall(b2World *world, sf::Vector2f position, float radius, sf::Texture &texture) : DynamicPolygon(world, position, radius, -1, texture, 896, 656, 128, 128, false, 0) {}
	WhiteBall() = default;

	void simulateCueImpact(float power); //!< Apply force projecting the ball in the direction of the ray, imitating the ball being struck by a pool cue
	void updateDirection(float amt); //!< Change the direction that the ball/ray are facing
	float getDirection(); //!< Return the current direction, for the ray
	bool isStationary() const; //!< Check whether the ball is moving or not
};