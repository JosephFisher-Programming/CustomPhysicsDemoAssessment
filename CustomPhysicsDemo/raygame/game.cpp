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

	//  TODO: add CIRCLE - AABB check
	//  TODO: add AABB - AABB check

	return map;
}

depenetrationMap setupDepenetrationFunc()
{
	depenetrationMap map;

	map[(collisionPair)(shapeType::CIRCLE | shapeType::CIRCLE)] = gatherCollisionDataCircleCircle;

	//  TODO: add CIRCLE-AABB depen
	//  TODO: add AABB-AABB depen

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

		physObject baby;
		baby.pos = { cursorPos.x, cursorPos.y };
		baby.mass = (rand() % 10) + 1;
		baby.shape.circleData.radius = baby.mass;
		baby.addImpulse({ 100,0 });

		physObjects.push_back(baby);
	}

	//  TODO: right-click and push all of the nearby particles within a radius

	return !WindowShouldClose();
}

void game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;

	//  TODO:  add gravity for all physics objects

	//  test for collision
	//  TODO: optimize with spatial partitioning (octrees?)
	for (auto& lhs : physObjects)
	{
		for (auto& rhs : physObjects)
		{
			//  skip ourselves
			if (&lhs == &rhs) { continue; }

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
	}

	for (auto& obj : physObjects)
	{
		obj.tickPhysics(fixedTimeStep);
	}

	//  TODO:  wrap the objects around the level
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