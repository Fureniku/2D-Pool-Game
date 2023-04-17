#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "Block.h"
#include "DynamicPolygon.h"
#include "Slider.h"
#include "Spinner.h"
#include "Sensor.h"
#include "Ray.h"
#include "Trapdoor.h"
#include "WhiteBall.h"
#include "SFMLDebugDraw.h"
#include "ContactListener.h"

class GameManager : public sf::Drawable {
private:
	sf::View view; //!< The view maps from physical co-ordinates to rendering co-ordinates
	sf::Vector2f worldSize = sf::Vector2f(9.f, 6.75f); //!< World slightly bigger to fit UI stuff without blocking game view

	b2World *world = nullptr; //!< Pointer to the Box2D world.  Using pointers as BOX2D has it's own memory management
	const int velIterations = 7; //!< On each update there will be 7 velocity iterations in the physics engine
	const b2Vec2 gravity = b2Vec2(0.f, 0.f); //!< No gravity for this simulation.

	sf::Texture texture; //!< The texture sheet used for all textures

	std::vector<Block> staticBlocks; //!< A collection of fixed blocks that make up the game world. 
	std::vector<DynamicPolygon> balls; //!< A collection of balls the player can knock around.

	std::vector<PhysicalObject*> spinners; //!< Used exclusively by the spinner sensor to know about the spinners it needs to control
	std::vector<PhysicalObject*> trapdoors; //!< Used exclusively by the trapdoor sensor to know about the trapdoors it needs to control

	std::vector<PhysicalObject*> objects; //!< Contains a pointer of all the small physical objects (not walls/uncontrolled balls) for simplicity in update/drawing

	sf::Color maskCol = sf::Color(127, 127, 127); //!< The colour of the areas outside the game area.
	std::vector<sf::RectangleShape> masks; //!< The masks used to cover things outside the game area

	Ray ray; //!< The ray that shows the balls hit direction

	sf::RectangleShape background; //!< The background 
	sf::RectangleShape chargeBg; //!< The background of the charge bar
	sf::RectangleShape chargeBar; //!< The charge bar itself

	DynamicPolygon controlledBlock; //!< The block the player can control with WASDQE
	WhiteBall whiteBall; //!< The ball the player can shoot with < > [Space]

	Slider magentaSlider; //!< The slider in the bottom-right corner, moving back and forth continuously.

	Spinner spinner1; //!< The spinner in the top-right corner, spin direction is toggled by hitting the colour-coded sensor in the middle of screen
	Spinner spinner2; //!< The spinner in the top-right corner, spin direction is toggled by hitting the colour-coded sensor in the middle of screen

	Trapdoor leftTrapdoor; //!< The left-side trapdoor half, open while colour-coded sensor is active
	Trapdoor rightTrapdoor; //!< The right-side trapdoor half, open while colour-coded sensor is active

	Sensor trapdoorSensor; //!< Sensor to open/close trapdoor
	Sensor spinnerSensor; //!< Sensor to invert spin direction

	Sensor tlPocket; //!< Sensor for top-left pocket
	Sensor trPocket; //!< Sensor for top-left pocket
	Sensor blPocket; //!< Sensor for top-left pocket
	Sensor brPocket; //!< Sensor for top-left pocket

	ContactListener listener; //!< Contact listener to change colours based on collisions

	bool debug = false; //!< Toggle for debug drawing
	SFMLDebugDraw debugDraw; //!< Box2D debug drawing

	sf::Font font; //!< The font used by text 
	sf::Text counterText; //!< The text displayed at the bottom of the screen
	sf::Text helperText; //!< The text at the top of the screen explaining controls
	sf::Text sizeText; //!< The current number of sides a right click shape has

	int pocketed = 0; //!< How many balls are pocketed. Calculated based on how many are left.
	float power = 0.f; //!< The power charged to the shot

	void initText(sf::Text &text, float posX, float posY, float scale); //!< Prepare the text object with default parameters
	void createGameWorld(); //!< Create and define the game objects

public:
	GameManager();
	~GameManager();

	void tick(float step); //!< Physics updates
	void mouseClickHandler(sf::Vector2i mousePosIn, sf::RenderWindow &window, bool leftClick); //!< Create a ball at the mouse cursor if appropriate to do so
	void keyHandler(sf::Vector2f moveAmount, float rotation, float wbDir); //!< Pass input to the controllable slider
	void draw(sf::RenderTarget &target, sf::RenderStates states) const; //!< Draw everything
	void toggleDebug() { debug = !debug; } //!< Turns debug on/off when pressing TAB
	void chargeWhiteBall(bool keyDown); //!< Handle charging and firing the white ball
};