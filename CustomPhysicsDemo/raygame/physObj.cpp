#include "physObj.h"

#include "raylib.h"

physObject::physObject()
{
	pos = glm::vec2(0, 0);
	vel = glm::vec2(0, 0);
	forces = glm::vec2(0, 0);
	isGravity = true;

	mass = 1.0f;
	shape = { shapeType::CIRCLE, circle{10.0f} };
}

physObject::physObject(float radiusInput, bool gravity)
{
	pos = glm::vec2(0, 0);
	vel = glm::vec2(0, 0);
	forces = glm::vec2(0, 0);
	isGravity = gravity;

	mass = 1.0f;
	shape = { shapeType::CIRCLE, circle{radiusInput} };
}

physObject::physObject(float widthInput, float heightInput, bool gravity)
{
	pos = glm::vec2(0, 0);
	vel = glm::vec2(0, 0);
	forces = glm::vec2(0, 0);
	isGravity = gravity;

	mass = 1.0f;
	shape = { shapeType::AABB };
	shape.boxData.width = widthInput;
	shape.boxData.height = heightInput;
}

void physObject::tickPhysics(float deltaTime) 
{
	//  integrate forces into velocity
	vel += forces * deltaTime;
	forces = { 0,0 };

	//  TODO: insert some linear drag

	//  integrate velocity into position
	pos += vel * deltaTime;
}

void physObject::draw() const
{
	switch (shape.colliderShape)
	{
	case shapeType::NONE:
		DrawPixel((int)pos.x, (int)pos.y, RED);
	case shapeType::CIRCLE:
		DrawCircleLines((int)pos.x, (int)pos.y, shape.circleData.radius, GREEN);
	case shapeType::AABB:
		DrawRectangle((int)pos.x, (int)pos.y, (int)shape.boxData.width, (int)shape.boxData.height, PURPLE);
	default:
		break;
	}
}

void physObject::addForce(glm::vec2 force)
{
	forces += force * (1.0f / mass);
}

void physObject::addImpulse(glm::vec2 impulse)
{
	vel += impulse * (1.0f / mass);
}

void physObject::addAccel(glm::vec2 accel)
{
	forces += accel;
}

void physObject::addVelocityChange(glm::vec2 velChng)
{
	vel += velChng;
}
