#include "PhysicalObject.h"

b2FixtureDef PhysicalObject::createFixture(b2Shape *shape, uint16 catBits, uint16 maskBits) {
	return createFullFixture(shape, false, catBits, maskBits);
}

b2FixtureDef PhysicalObject::createFullFixture(b2Shape *shape, bool isSensor, uint16 catBits, uint16 maskBits) {
	b2FixtureDef fixtureDef;

	//Everything uses the same densitiy/friction/restitution
	fixtureDef.density = density;
	fixtureDef.isSensor = isSensor;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.shape = shape;

	//Collision filtering
	fixtureDef.filter.categoryBits = catBits;
	fixtureDef.filter.maskBits = maskBits;

	return fixtureDef;
}

b2BodyDef PhysicalObject::defineBody(sf::Vector2f position, float orientation) {
	b2BodyDef bodyDef;

	bodyDef.position.Set(position.x, position.y);
	bodyDef.angle = orientation * DEG2RAD;

	if (isDynamic) {
		bodyDef.type = b2_dynamicBody;
	}

	return bodyDef;
}

b2Body* PhysicalObject::getBody() {
	return body;
}