// Jack Hedges
// P2551027

/*!
\file main.cpp
\brief Contains the entry point for the application
*/

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

#include "../include/GameManager.h"


void main() /** Entry point for the application */
{
	std::cout << "begin program" << std::endl;
	int screenW = 1200;
	int screenH = 900;

	bool clicked = false;

	//Movement data
	float lr = 0.f;
	float ud = 0.f;
	float rot = 0.f;
	float wbDir = 0.f;

	bool leftDown = false;
	bool rightDown = false;
	bool upDown = false;
	bool downDown = false;
	bool rotLeftDown = false;
	bool rotRightDown = false;

	bool ballLeftDown = false;
	bool ballRightDown = false;

	bool chargeCueDown = false;

	
	sf::RenderWindow window(sf::VideoMode(screenW, screenH), "2D Physics Pool"); // Open main window
	//window.setFramerateLimit(60);
	GameManager game;
	float frameTime = 1.f / 60.f;

	// Start a clock to provide time for the physics
	sf::Clock clock;

	// Run a game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// window closed button pressed
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			//Listen for key presses, handle them later.
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::A) {
					leftDown = true;
				}
				if (event.key.code == sf::Keyboard::D) {
					rightDown = true;
				}
				if (event.key.code == sf::Keyboard::W) {
					upDown = true;
				}
				if (event.key.code == sf::Keyboard::S) {
					downDown = true;
				}
				if (event.key.code == sf::Keyboard::Q) {
					rotLeftDown = true;
				}
				if (event.key.code == sf::Keyboard::E) {
					rotRightDown = true;
				}

				if (event.key.code == sf::Keyboard::Left) {
					ballLeftDown = true;
				}
				if (event.key.code == sf::Keyboard::Right) {
					ballRightDown = true;
				}

				if (event.key.code == sf::Keyboard::Tab) {
					game.toggleDebug();
				}
				if (event.key.code == sf::Keyboard::Space) {
					chargeCueDown = true;
				}
			}

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::A) {
					leftDown = false;
				}
				if (event.key.code == sf::Keyboard::D) {
					rightDown = false;
				}
				if (event.key.code == sf::Keyboard::W) {
					upDown = false;
				}
				if (event.key.code == sf::Keyboard::S) {
					downDown = false;
				}
				if (event.key.code == sf::Keyboard::Q) {
					rotLeftDown = false;
				}
				if (event.key.code == sf::Keyboard::E) {
					rotRightDown = false;
				}

				if (event.key.code == sf::Keyboard::Left) {
					ballLeftDown = false;
				}
				if (event.key.code == sf::Keyboard::Right) {
					ballRightDown = false;
				}

				if (event.key.code == sf::Keyboard::Space) {
					chargeCueDown = false;
				}
			}

			//If left or right is down, but not both of them...
			if ((leftDown || rightDown) && !(leftDown && rightDown)) {
				lr = leftDown ? -1.f : 1.f;
			}
			else {
				lr = 0.f;
			}

			if ((upDown || downDown) && !(upDown && downDown)) {
				ud = upDown ? -1.f : 1.f;
			}
			else {
				ud = 0.f;
			}

			if ((rotLeftDown || rotRightDown) && !(rotLeftDown && rotRightDown)) {
				rot = rotLeftDown ? -5.f : 5.f;
			}
			else {
				rot = 0.f;
			}

			if ((ballLeftDown || ballRightDown) && !(ballLeftDown && ballRightDown)) {
				wbDir = ballLeftDown ? -0.025f : 0.025f;
			}
			else {
				wbDir = 0.f;
			}
			
			//Handle mouse clicks now; need better precision.
			if (event.type == sf::Event::MouseButtonPressed) {
				//Ensure only one ball per mouse click happens.
				if (!clicked) {
					game.mouseClickHandler(sf::Mouse::getPosition(window), window, event.mouseButton.button == sf::Mouse::Left);
					clicked = true;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				clicked = false;
			}
		}

		

		// Find out how much time has elapsed
		float elapsedTime = clock.getElapsedTime().asSeconds();
		// If a frame has past the update the physics
		if (elapsedTime > frameTime)
		{
			game.chargeWhiteBall(chargeCueDown);

			//Handle the key presses per frame to ensure consistency across different framerate devices
			game.keyHandler(sf::Vector2f(lr, ud), rot, wbDir);

			game.tick(elapsedTime);
			clock.restart();
		}

		window.clear(sf::Color(255, 255, 255));
		window.draw(game);
		window.display();
	}
}