//**   File: cpp

#include <SDL/sdl.h>
#include <windows.h>
#include <iostream>
#include <gl/gl.h>
#include "input.h"
#include "renderer.h"

using namespace std;

input_t input;


/*
============
input_t::Reset
============
*/
void input_t::Reset( void )
{
	memset( this, 0, sizeof(input_t) );
}


/*
============
input_t::HandleEvents
============
*/
void input_t::HandleEvents()
{
	extern void QuitApp(int);
	SDL_Event event_;

	// add the times a key is bying pressed
	for( int x=0; x<SDLK_LAST; x++ )
	{
		if( keys[x] ) ++keys[x];
	}
	for( int x=0; x<8; x++ )
	{
		if( mouse_btns[x] ) ++mouse_btns[x];
	}

	mouse_velocity.LoadZero();
	vec2_t prev_mouse_pos( mouse_pos );
	
	while (SDL_PollEvent(&event_))
	{
		switch( event_.type ) 
		{
			case SDL_KEYDOWN:
				keys[event_.key.keysym.sym] = 1;
				break;

			case SDL_KEYUP:				
				keys[event_.key.keysym.sym] = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_btns[event_.button.button] = 1;
				break;

			case SDL_MOUSEBUTTONUP:
				mouse_btns[event_.button.button] = 0;
				break;
				
			case SDL_MOUSEMOTION:
				float viewport_args[4];
				glGetFloatv( GL_VIEWPORT, viewport_args );
			
				mouse_pos.x() = event_.button.x/(float)(viewport_args[2]/2) - 1.0f;
				mouse_pos.y() = 1.0f-( event_.button.y/(float)(viewport_args[3]/2) );
				
				mouse_velocity = mouse_pos - prev_mouse_pos;
				break;

			case SDL_QUIT:
				QuitApp(1);
				break;
		}
	}
	
	//SDL_WarpMouse( renderer.w/2, renderer.h/2);
	
	//cout << fixed << " velocity: " << mouse_velocity.x() << ' ' << mouse_velocity.y() << endl;
	//cout << fixed << mouse_pos.x() << ' ' << mouse_pos.y() << endl;
	//cout << crnt_keys[SDLK_m] << ' ' << prev_keys[SDLK_m] << "      " << keys[SDLK_m] << endl;
	//cout << mouse_btns[SDL_BUTTON_LEFT] << endl;
	
}