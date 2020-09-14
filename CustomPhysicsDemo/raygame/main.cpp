/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "game.h"

#include "glm/glm.hpp"

int main()
{
	game app;

	app.init(800,450);
	app.fixedTimeStep = 1.0f / 30.0f;

	bool shouldRun = true;


	// Main game loop
	while (shouldRun)    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		shouldRun = app.tick();

		//  do we need to run a physics update, if so then update.
		while (app.shouldTickPhysics())
		{
			app.tickPhysics();
		}

		// Draw
		//----------------------------------------------------------------------------------
		app.draw();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	app.exit();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}