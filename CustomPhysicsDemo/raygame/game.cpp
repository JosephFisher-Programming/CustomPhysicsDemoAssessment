#include "game.h"		//  header to be implemented

						//  STYSTEM LIBRARIES
#include <iostream>		//  cout
#include <ctime>		//  time
#include <cstdlib>		//  rand, srand

						//  PROJECT LIBRARIES
#include "raylib.h"		//  raylib

						//  PROJECT HEADERS
#include "enumUtils.h"  //  enum utils

//  define a function thatt will initialize our static variable in game
collisionMap setupCollisionChecks() 
{
	collisionMap map;

	map[static_cast<collisionPair>(shapeType::CIRCLE | shapeType::CIRCLE)] = checkCircleCircle;
	map[static_cast<collisionPair>(shapeType::AABB | shapeType::CIRCLE)] = checkAABBCircle;
	map[static_cast<collisionPair>(shapeType::AABB | shapeType::AABB)] = checkAABBAABB;

	return map;
}

depenetrationMap setupDepenetrationFunc()
{
	depenetrationMap map;

	map[(collisionPair)(shapeType::CIRCLE | shapeType::CIRCLE)] = gatherCollisionDataCircleCircle;
	map[(collisionPair)(shapeType::AABB | shapeType::CIRCLE)] = gatherCollisionDataAABBCircle;
	map[(collisionPair)(shapeType::AABB | shapeType::AABB)] = gatherCollisionDataAABBAABB;

	return map;
}

//  define the stattic variable in game
collisionMap game::collisionCheckers = setupCollisionChecks();
depenetrationMap game::depenetrationFuncs = setupDepenetrationFunc();

//  Inclusion order
//  - header that I am implementing
//  - system libraries
//  - project libraries
//  - other headers from this project

game::game() 
{
	accumulatedDeltaTime = 0.0f;

	fixedTimeStep = 1.0f / 30.0f;

	srand(time(0));
}

 void game::init(int screenWidthInput, int screenHeightInput)
{
	int screenWidth = screenWidthInput;
	int screenHeight = screenHeightInput;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
}

bool game::tick()
{
	accumulatedDeltaTime += GetFrameTime();

	//  check to see if ticked, if so, add new physObj at that location

	if(IsMouseButtonPressed(0))
	{
		auto cursorPos = GetMousePosition();

		physObject baby(10.0f, false);
		baby.pos = { cursorPos.x, cursorPos.y };
		baby.mass = (rand() % 10) + 1;
		baby.shape.circleData.radius = baby.mass;
		baby.addImpulse({ 0,0 });

		physObjects.push_back(baby);
	}

	if (IsKeyPressed(KEY_Q))
	{
		auto cursorPos = GetMousePosition();

		physObject baby(5.0f, 5.0f, true);
		baby.pos = { cursorPos.x, cursorPos.y };
		baby.mass = (rand() % 10) + 1;
		baby.shape.boxData.width *= baby.mass;
		baby.shape.boxData.height *= baby.mass;
		baby.addImpulse({ 200,0 });

		physObjects.push_back(baby);
	}

	if(IsMouseButtonPressed(1))
	{
		auto cursorPos = GetMousePosition();

		for (auto& targets : physObjects)
		{
			//  check the distance between the mouse cursor and the object
			if (glm::distance(glm::vec2{ cursorPos.x, cursorPos.y }, targets.pos) < 50)
			{
				//  Push the objects away from the mouse
				glm::vec2 direction = glm::normalize(glm::vec2{ targets.pos.x - cursorPos.x , targets.pos.y - cursorPos.y });
				direction.x *= 30;
				direction.y *= 30;
				targets.addImpulse(direction);
			}
		}
	}
	return !WindowShouldClose();
}

void game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;

	//  test for collision
	for (auto& lhs : physObjects)
	{
		//  add gravity to all physObjects
		if (lhs.isGravity) 
		{
			lhs.addAccel({ 0,20 * lhs.mass});
		}

		for (auto& rhs : physObjects)
		{
			//  skip ourselves
			if (&lhs == &rhs  || lhs.quadrantValue != rhs.quadrantValue) { continue; }

			auto *first = &lhs;
			auto *second = &rhs;

			//  swap our pointers around so they're in the right order
			if (static_cast<uint8_t> (lhs.shape.colliderShape) >
				static_cast<uint8_t> (lhs.shape.colliderShape)) 
			{
				first = &rhs;
				second = &lhs;
			}

			//  get the type of collision to test for
			collisionPair pairType = static_cast<collisionPair>(lhs.shape.colliderShape | rhs.shape.colliderShape);

			//  get the collision check function to call and call it
			bool collision = collisionCheckers[pairType](first->pos, first->shape, second->pos, second->shape);

			//  gather collision response data
			if (collision)
			{
				float pen = 0.0f;
				//  generate the responding impulses
				glm::vec2 normal = depenetrationFuncs[pairType](first->pos, first->shape,
																second->pos, second->shape,
																pen);
					
				//  NOTE: you could make this re-used between runs
				glm::vec2 recImpulses[2];
				resolveCollision(first->pos, first->vel, first->mass,
								 second->pos, second->vel, second->mass,
								 1.0f, normal, recImpulses);

				//  physically seperate the two obj
				first->pos += normal * pen;
				second->pos -= normal * pen;

				//  update their velocities
				first->vel = recImpulses[0];
				second->vel = recImpulses[1];
			}
		}
		if (lhs.pos.x > 850) 
		{
			lhs.pos.x = 0;
		}
		if (lhs.pos.x < 0)
		{
			lhs.pos.x = 850;
		}
		if (lhs.pos.y > 450)
		{
			lhs.pos.y = 0;
		}
		if (lhs.pos.y < 0)
		{
			lhs.pos.y = 450;
		}
	}

	for (auto& obj : physObjects)
	{
		obj.tickPhysics(fixedTimeStep);
	}
}

bool game::shouldTickPhysics() const
{
	return accumulatedDeltaTime >= fixedTimeStep;
}

void game::draw() const
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (auto& obj : physObjects)
	{
		obj.draw();
	}

	EndDrawing();
}

void game::exit()
{
	CloseWindow();
}
