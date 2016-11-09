#include <windows.h>
#include <SDL/sdl.h>
#include <gl/gl.h>
#include <iostream>
#include "input.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "texture.h"
#include "textout.h"
#include <math.h>

using namespace std;


int main( int argc, char* argv[] )
{
	extern int WaitForNextTick();
	extern void QuitApp(int);
	extern void InitWindow( int w, int h );
	extern void TogleFullScreen();
	
	// init SDL
	InitWindow( renderer.w, renderer.h );
	SDL_WM_SetCaption("skeletal animation test by GODlike", NULL);

	
	renderer.Init();
	tout.Init();
	
	cam.MoveForward( -7.0 );
	cam.MoveUpward( 2.0 );
	cam.RotateX( -10.0 );
	
	texture_t tex;
	model_t mdl;
	
	tex.Load( "models\\imp\\imp_d.tga" );
	mdl.Load( "models\\imp\\gmdl.txt" );
	
	int prev_time = 0, time = 0;
	int frame = 0, frame1 = 1;
	do
	{
		prev_time = time;
		time = SDL_GetTicks();
	
		renderer.Run();
		input.HandleEvents();
		
		
		float dist = 0.2;
		float ang = 2.0;
		if( input.keys[SDLK_w] ) cam.MoveForward( dist );
		if( input.keys[SDLK_s] ) cam.MoveForward( -dist );
		if( input.keys[SDLK_r] ) cam.RotateX( ang );
		if( input.keys[SDLK_f] ) cam.RotateX( -ang );
		if( input.keys[SDLK_LSHIFT] ) cam.MoveUpward( dist );
		if( input.keys[SDLK_SPACE] ) cam.MoveUpward( -dist );	
		if( input.keys[SDLK_a] ) cam.StrafeRight( dist );
		if( input.keys[SDLK_d] ) cam.StrafeRight( -dist );
		if( input.mouse_btns[SDL_BUTTON_MIDDLE] )
		{
			if( input.mouse_velocity.x()>0.0 ) cam.RotateY( -ang );
			if( input.mouse_velocity.x()<0.0 ) cam.RotateY( ang );
			
			if( input.mouse_velocity.y()>0.0 ) cam.RotateX( ang );
			if( input.mouse_velocity.y()<0.0 ) cam.RotateX( -ang );
		}
		if( input.keys[SDLK_LEFT] ) cam.RotateY( ang );
		if( input.keys[SDLK_RIGHT] ) cam.RotateY( -ang );
		if( input.keys[SDLK_UP] ) cam.RotateX( ang );
		if( input.keys[SDLK_DOWN] ) cam.RotateX( -ang );
		cam.RepositionScene();
		
		renderer.DrawGrid();
		
		//
		extern void DrawMesh( mesh_t&, GLuint );
		
		mdl.Interpolate( 1, frame++ );
		mdl.ApplyArmatAnimToMesh();
		if( frame >= mdl.anims[1].frames_num ) frame=10;
		
		glPushMatrix();
		glRotatef( -90.0, 1.0, 0.0, 0.0 );
		DrawMesh( mdl.mesh, tex.id );
		//mdl.armat.Draw();	
		glPopMatrix();
		
		tout.SetPos( -0.98, 0.95 );
		tout.Printf( "Frame %d\nFPS %.1f", renderer.frames_num, 1000/(float)(time-prev_time)  );
		
		//	
		
		SDL_GL_SwapBuffers();
		
		if( input.keys[SDLK_ESCAPE] ) QuitApp(0);
		if( input.keys[SDLK_F1] == 1 ) TogleFullScreen();
		
	}while( WaitForNextTick() );
	
	return 0;
}