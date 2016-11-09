#include "input.h"
#include "camera.h"
#include <SDL/sdl.h>
#include <math.h>
#include <windows.h>
#include <gl/glu.h>



camera_t cam;


/*
===========
camera_t::Reposition
===========
*/
//void camera_t::Reposition()
//{
//	float velocity = 0.5f;
//	float mouse_ang = 3.0;
//
//	if( input.crnt_keys[SDLK_UP] )     co.z() += velocity;
//	if( input.crnt_keys[SDLK_DOWN] )   co.z() -= velocity;
//	if( input.crnt_keys[SDLK_LEFT] )   co.x() += velocity;
//	if( input.crnt_keys[SDLK_RIGHT] )  co.x() -= velocity;
//	
//	if( input.crnt_mouse_btns[0] )
//	{
//		if( input.crnt_mouse_coord.x() > 0 )       ang.y() += mouse_ang;
//		else if( input.crnt_mouse_coord.x() < 0 )  ang.y() -= mouse_ang;
//		
//		if( input.crnt_mouse_coord.y() > 0 )       ang.x() -= mouse_ang;
//		else if( input.crnt_mouse_coord.y() < 0 )  ang.x() += mouse_ang;
//	}
//}

camera_t::camera_t()
{
	memset(this, 0, sizeof(camera_t));
	viewdir.z() = -1.0;
	right.x() = 1.0;
	up.y() = 1.0;
}


void camera_t::Move( vec3_t direction )
{
	pos = pos + direction;
}


void camera_t::RotateX( float ang )
{
	ang_x += ang;
	
	//Rotate viewdir around the right vector:
	viewdir = viewdir*cos(ang*PI/180) + up*sin(ang*PI/180);
	viewdir.Normalize();

	//now compute the new UpVector(by cross product)
	up = (viewdir * right) * -1.0;
}


void camera_t::RotateY( float ang )
{
	ang_y += ang;
	
	//Rotate viewdir around the up vector:
	viewdir = viewdir*cos(ang*PI/180) - right*sin(ang*PI/180);
	viewdir.Normalize();

	//now compute the new RightVector(by cross product)
	right = viewdir*up;
}


void camera_t::RotateZ( float ang )
{
	ang_z += ang;
	
	//Rotate viewdir around the right vector:
	right = right*cos(ang*PI/180) + up*sin(ang*PI/180);
	right.Normalize();

	//now compute the new UpVector(by cross product)
	up = (viewdir*right) * -1.0;
}


void camera_t::MoveForward( float distance )
{
	pos += viewdir * distance;
}


void camera_t::StrafeRight( float distance )
{
	pos -= right * distance;
}


void camera_t::MoveUpward( float distance )
{
	pos += up*distance;
}


void camera_t::RepositionScene( void )
{
	//The point at which the camera looks:
	//vec3_t viewpoint = pos + viewdir;
	//as we know the up vector, we can easily use gluLookAt:
	//gluLookAt( pos.x(), pos.y(), pos.z(), viewpoint.x(), viewpoint.y(), viewpoint.z(), up.x(), up.y(), up.z() );
	
	/*mat4_t transf, transl, rot;
	
	rot.LoadIdent();
	rot[0][0] = right.x();
	rot[0][1] = right.y();
	rot[0][2] = right.z();
	rot[1][0] = up.x();
	rot[1][1] = up.y();
	rot[1][2] = up.z();
	rot[2][0] = -viewdir.x();
	rot[2][1] = -viewdir.y();
	rot[2][2] = -viewdir.z();
	
	transl.LoadVec3( pos*(-1.0f) );
	
	transf = rot * transl;
	
	transf.Transpose();*/
	
	mat3_t rot;
	
	rot[0][0] = right.x();
	rot[0][1] = right.y();
	rot[0][2] = right.z();
	rot[1][0] = up.x();
	rot[1][1] = up.y();
	rot[1][2] = up.z();
	rot[2][0] = -viewdir.x();
	rot[2][1] = -viewdir.y();
	rot[2][2] = -viewdir.z();
	
	vec3_t inv_pos = (rot * pos)*-1.0f;
	
	mat4_t transf;
	transf.LoadMat3( rot );
	transf[0][3] = inv_pos.x();
	transf[1][3] = inv_pos.y();
	transf[2][3] = inv_pos.z();
	
	transf.Transpose();
	
	glMultMatrixf( &transf[0][0] );
}