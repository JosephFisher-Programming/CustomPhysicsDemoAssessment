#pragma once

#include <vector>			   // vector
#include <unordered_map>       // unordered map, like a hashtable

#include "physObj.h"

//  type-aliasing - for better readability
//  function pointers - poointers too runctioons so we call them later...
//						this is a form of dynamic dispatch
//
//						syntax for this is like so...
//						//  RETURN_TYPE(*)(PARAMETER_TYPES)
//  unordered_map == hash_table
//  
//  given two objects, we need to figure out what function to call
//  in order to test whether they are in collision
//
//  type-punning

using collisionPair = uint8_t; // type-alias, like a typedef

using collisionFunc = bool(*)(glm::vec2, collider, glm::vec2, collider);
using collisionMap = std::unordered_map<collisionPair, collisionFunc>;

using depenetrationFunc = glm::vec2(*)(glm::vec2, collider, glm::vec2, collider, float&);
using depenetrationMap = std::unordered_map<collisionPair, depenetrationFunc>;

//  RETURN_TYPE(*)(PARAMETER_TYPES)

class game
{
	//  Accumulator for physics updates.
	float accumulatedDeltaTime;

	std::vector<physObject> physObjects;

	static collisionMap collisionCheckers;
	static depenetrationMap depenetrationFuncs;

public:
	//  default constructor
	game();

	// time between every fixed update.
	float fixedTimeStep;

	//  Create the game and cause visuals.
	void init(int screenWidthInput, int screenHeightInput);

	//  Update logic for the game.
	//  - Return true as long as window should be open.
	bool tick();

	//  Update the physics logic for the game
	//  - this should only run every fixed timespan.
	void tickPhysics();

	//  Return true when a physics updatte is ready
	bool shouldTickPhysics() const;

	//  Render the visuals for the game.
	void draw() const;

	//  Destroy the window and exit the game.
	void exit();
};