#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL\include\SDL.h"
#pragma comment( lib, "Source/SDL/libx86/SDL2.lib" )
#pragma comment( lib, "Source/SDL/libx86/SDL2main.lib" )

enum MAIN_STATES
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	MAIN_STATES state = MAIN_CREATION;
	Application* App = NULL;	

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			App->console.AddLOG("Starting game '%s'...", TITLE);
			App->console.AddLOG("-------------- Application Creation --------------");
			state = MAIN_START;
			break;

		case MAIN_START:

			App->console.AddLOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				App->console.AddLOG("[error] Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				App->console.AddLOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				App->console.AddLOG("[error] Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			App->console.AddLOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				App->console.AddLOG("[error] Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	App->console.AddLOG("Exiting game '%s'...\n", TITLE);
	delete App;
	
	return main_return;
}