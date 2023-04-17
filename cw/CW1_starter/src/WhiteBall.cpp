#include "WhiteBall.h"

void WhiteBall::simulateCueImpact(float power) {
	if (isStationary()) {
		body->ApplyLinearImpulseToCenter(b2Vec2(power * cosf(direction), power * sinf(direction)), true);
	}
}

void WhiteBall::updateDirection(float amount) {
	direction += amount;
}

float WhiteBall::getDirection() {
	return direction;
}

bool WhiteBall::isStationary() const {
	return body->GetLinearVelocity().Length() < 0.1f;
}