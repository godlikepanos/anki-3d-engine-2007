#include <sdl/sdl.h>
#include "input.h"
#include "renderer.h"
#include "handlers.h"
#include <sdl/SDL_syswm.h>


SDL_Surface* main_surf;


// InitWindow
void InitWindow( int w, int h )
{
	SDL_Init( SDL_INIT_VIDEO );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	main_surf = SDL_SetVideoMode( w, h, 32, SDL_HWSURFACE | SDL_OPENGL );
	
#ifdef WIN32
	SDL_SysWMinfo i;
	SDL_VERSION( &i.version );
	if( SDL_GetWMInfo(&i) ) 
	{
		HWND hwnd = i.window;
		SetWindowPos( hwnd, HWND_TOP, 10, 25, w, h, NULL );
	}
#endif 
}


// TogleFullScreen
void TogleFullScreen()
{
	// Not Working
}


// QuitApp
void QuitApp( int code )
{
	SDL_FreeSurface( main_surf );
	SDL_Quit();
	exit(code);
}


// WaitForNextTick 
void WaitForNextTick()
{
	static int nextTime = SDL_GetTicks();

	int now = SDL_GetTicks();
	if(nextTime > now)
		SDL_Delay(nextTime - now);

	nextTime += TIMER_TICK;
}