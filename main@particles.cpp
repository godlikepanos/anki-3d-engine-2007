#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <sdl/sdl.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "input.h"
#include "camera.h"
#include "math.h"
#include "renderer.h"
#include "textout.h"
#include "handlers.h"
#include "particles.h"
using namespace std;

vec3_t g( 0.0, -1.0, 0.0 );



// DrawGL()
void DrawGL()
{
	glDisable( GL_TEXTURE_2D );
	glPolygonMode( GL_FRONT, GL_LINE );
	glPointSize( 4.0 );
	
	glPushMatrix();
	
	
	//glColor3f( float(rand())/float(RAND_MAX), float(rand())/float(RAND_MAX), float(rand())/float(RAND_MAX) );
	
	vec3_t pos_init( 0.0, 10.0, 0.0 ), 
	       pos, 
	       g( 0.0, -10.0, 0.0 ), 
	       v0( 5.0, 15.3, 0.0 ),
	       vg;
	static float frame = 0.0;
	
	// calc new pos
	float dt = frame * ( float(TIMER_TICK)/1000 );
	vg = (g * dt);
	pos = (vg+v0) * dt + pos_init;
	frame += 1.0;
	if( pos.y() < 0.0 ) frame = 0.0;
	
	
	// draw the point
	glColor3f( 1.0, 0.0, 0.0 );
	glBegin( GL_POINTS );
		glVertex3fv( &pos[0] );
	glEnd();
	
	// draw the vg
	glColor3f( 0.0, 0.0, 1.0 );
	glBegin( GL_LINES );
		glVertex3fv( &pos[0] );
		glVertex3fv( &(vg+pos)[0] );
	glEnd();
	
	// draw the v0
	glColor3f( 0.0, 1.0, 0.0 );
	glBegin( GL_LINES );
		glVertex3fv( &pos[0] );
		glVertex3fv( &(v0+pos)[0] );
	glEnd();
	
	// draw the Sv
	glColor3f( 1.0, 1.0, 1.0 );
	glBegin( GL_LINES );
		glVertex3fv( &pos[0] );
		glVertex3fv( &(vg+v0+pos)[0] );
	glEnd();
	
	glPopMatrix();
}



// main
int main( int argc, char* argv[] )
{	
	InitWindow( renderer.w, renderer.h );
	SDL_WM_SetCaption("Particles", NULL);

	renderer.Init();
	tout.Init();
	
	cam.MoveForward( -10.0 );
	cam.MoveUpward( 2.0 );
	cam.RotateY( 30.0 );
	
	particle_emitter_t pem;
	pem.Init();

	do
	{	
		input.HandleEvents();
		renderer.Run();
		
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
		if( input.keys[SDLK_LEFT] ) cam.RotateY( ang );
		if( input.keys[SDLK_RIGHT] ) cam.RotateY( -ang );
		if( input.keys[SDLK_UP] ) cam.RotateX( ang );
		if( input.keys[SDLK_DOWN] ) cam.RotateX( -ang );
		
		// move the emitter
		if( input.mouse_btns[SDL_BUTTON_LEFT] )
		{
			float dist = 0.1;
			if( input.mouse_velocity.x() > 0.0 ) pem.local_translate.x() += dist;
			if( input.mouse_velocity.x() < 0.0 ) pem.local_translate.x() -= dist;
			if( input.mouse_velocity.y() < 0.0 ) pem.local_translate.z() += dist;
			if( input.mouse_velocity.y() > 0.0 ) pem.local_translate.z() -= dist;
		}
		if( input.mouse_btns[SDL_BUTTON_MIDDLE] )
		{
			float dist = 0.1;
			if( input.mouse_velocity.y() > 0.0 ) pem.local_translate.y() += dist;
			if( input.mouse_velocity.y() < 0.0 ) pem.local_translate.y() -= dist;
		}
		
		cam.RepositionScene();
	
		renderer.DrawGrid();
		
		//DrawGL();
		pem.Render();
		
		//
		mat4_t transf, m4transl, m4rot, m4scale;
		mat3_t rot;
		float scale = 2.0f;
		vec3_t transl(0.0, 0.0, 10.0);
		rot.LoadEuler( euler_t( 10.0, 0.0, 0.0 ) );
		
		m4scale.LoadIdent();  m4scale[0][0]*=scale;  m4scale[3][3]=1.0f;
		m4transl.LoadVec3( transl );
		m4rot.LoadMat3( rot );
		
		transf = m4transl * m4rot * m4scale;
		
		//TRS( transl, rot, scale, transf );
		transf.Transpose();
		glPushMatrix();
		//glMultMatrixf( &transf[0][0] );
		
		m4scale.Transpose(); m4rot.Transpose(); m4transl.Transpose();
		
		
		glMultMatrixf( &m4transl[0][0] );
		glMultMatrixf( &m4rot[0][0] );
		glMultMatrixf( &m4scale[0][0] );
		
		glColor3fv( &(vec3_t(1.0, 0.0, 0.0))[0] );
		RenderCube();
		glPopMatrix();
		//
		
		tout.SetPos( -0.98, 0.95 );
		tout.Printf( "Frame %d", renderer.frames_num );
		
		
		
		if( input.keys[SDLK_ESCAPE] ) break;
		SDL_GL_SwapBuffers();
		WaitForNextTick();
	}while( true );

	
	QuitApp( 0 );
	return 0;
}