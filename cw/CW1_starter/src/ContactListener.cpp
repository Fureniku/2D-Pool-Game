#include "ContactListener.h"
#include "DynamicPolygon.h"
#include <iostream>

void ContactListener::BeginContact(b2Contact *contact) {
	HandleContact(contact, sf::Color::Green, true);
}

void ContactListener::EndContact(b2Contact *contact) {
	HandleContact(contact, sf::Color::White, false);
}

//Both begin/end in the example were doing the exact same functionality, just changing colour. Merge to one function.
void ContactListener::HandleContact(b2Contact *contact, sf::Color col, bool enter) {
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	if (bodyA->GetUserData() != nullptr && bodyB->GetUserData() != nullptr) { //Ensure both things are not null, so I don't need to implement for every single object.
		std::pair<std::string, void *> dataA = *(std::pair<std::string, void *>*) bodyA->GetUserData();
		std::pair<std::string, void *> dataB = *(std::pair<std::string, void *>*) bodyB->GetUserData();

		if (contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()) {
			Sensor *sens = static_cast<Sensor*>(dataA.second);
			DynamicPolygon *ball = static_cast<DynamicPolygon*>(dataB.second);
			if (sens != nullptr && !sens->isPocketSensor()) {
				std::cout << "Normal, non-pocket sensor handling" << std::endl;
				sens->handleSensor(enter);
			}

			if (ball != nullptr && sens->isPocketSensor()) {
				std::cout << "pocket sensor handling, pocketed " << ball->getId() << std::endl;
				ball->pocket();
			}

			return;
		}
		sf::Shape *block = static_cast<sf::Shape*>(dataA.second);

		if (enter) {
			block->setFillColor(col);
		}
		else {
			block->setFillColor(sf::Color::White);
		}
	}
}