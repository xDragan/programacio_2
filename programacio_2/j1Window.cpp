#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

j1Window::j1Window() : j1Module()
{
	window = NULL;
	screen_surface = NULL;
	update_on_pause = true;
	strncpy_s(name, "window", SHORT_STR);
}

// Destructor
j1Window::~j1Window()
{
}

// Called before render is available
bool j1Window::Awake()
{
	LOG("Init SDL window & surface");
	bool ret = true;
	

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		LOG( "SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError() );
		ret = false;
	}
	else
	{
		//Create window
		int width = App->config.GetInt("window", "width", SCREEN_WIDTH);
		int height = App->config.GetInt("window", "height", SCREEN_HEIGHT);
		char* title = App->config.GetString("window", "title", "j1 Engine");

		Uint32 flags = SDL_WINDOW_SHOWN;

		if(App->config.GetBool("window", "fullscreen", false) == true)
			flags |= SDL_WINDOW_FULLSCREEN;

		if(App->config.GetBool("window", "borderless", false) == true)
			flags |= SDL_WINDOW_BORDERLESS;

		if(App->config.GetBool("window", "resizable", false) == true)
			flags |= SDL_WINDOW_RESIZABLE;

		if(App->config.GetBool("window", "fullscreen_windowed", false) == true)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags );
		if( window == NULL )
		{
			LOG( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface( window );

			SetTitle(title);
		}
	}

	return ret;
}

// Called before the first frame
bool j1Window::Start()
{
	LOG("window start");
	return true;
}

// Add last frame fps stadistics in the title
bool j1Window::PreUpdate()
{
	if(App->last_fps >= 0)
	{
		static char framerate[MID_STR];
		sprintf_s(framerate, MID_STR, "%s - %d fps (%d/%d ms)", title, App->last_fps, App->last_frame_ms, App->capped_ms);
		SDL_SetWindowTitle(window, framerate);
	}
	else
		SDL_SetWindowTitle(window, title);

	return true;
}

// Called each loop iteration
bool j1Window::Update(float dt)
{
	bool ret = true;

	if(App->input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(App->input->GetKeyDown(SDLK_ESCAPE) == true)
		ret = false;

	if(App->input->GetWindowEvent(WE_HIDE) == true)
	{
		if(App->pause.Get() == false)
			LOG("Entering background mode");

		App->pause.Set();
	}

	if(App->input->GetWindowEvent(WE_SHOW) == true)
	{
		App->pause.Unset();

		if(App->pause.Get() == false)
			LOG("Leaving background mode");
	}

	return ret;
}

// Swap buffers
bool j1Window::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");
	
	//Destroy window
	if(window != NULL)
		SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	strcpy_s(title, MID_STR, new_title);
}