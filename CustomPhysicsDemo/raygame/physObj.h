#pragma once

#include "glm/glm.hpp"

#include "shapes.h"

class physObject 
{
	//  forces
	glm::vec2 forces;

public:
	//  position
	glm::vec2 pos;

	//  velocity
	glm::vec2 vel;

	//  mass
	float mass;

	collider shape;

	//  constructer (defaults)
	physObject();
	
	//  tickPhysics
	void tickPhysics(float deltaTime);

	//  draw
	void draw() const;

	//  addForce
	void addForce(glm::vec2 force);

	//  addImpulse
	void addImpulse(glm::vec2 impulse);

	//  addAcceleration
	void addAccel(glm::vec2 accel);

	//  addVelocity
	void addVelocityChange(glm::vec2 velChng);
};