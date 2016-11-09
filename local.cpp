#include "local.h"
#include <memory.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <gl/gl.h>


using namespace std;

// RandRange
int RandRange( int min, int max )
{
	return (rand() % (max-min+1)) + min ;
}

float RandRange( float min, float max )
{	
	double d = max - min; // difference
	if( d==0.0 ) return min;
	// diferrense = mant * e^exp
	int exp; 
	double mant = frexp( d, &exp );
	
	int precision = 1000; // more accurate
	
	mant *= precision;
	double new_mant = rand() % (int)mant;
	new_mant /= precision;
	
	return min + (float)ldexp( new_mant, exp ); // return min + (new_mant * e^exp)
}

// TRS
// from the translate, rotate and scale calculate and return the 
// correct mat4x4. AKA: mat4 = ToMat4(translate) * ToMat4(rotate) * ToMat4(scale)
void TRS( vec3_t& translate, mat3_t& rotate, float scale, mat4_t& transform )
{
	transform.LoadIdent();
	
	transform.LoadMat3( rotate*scale );
	
	transform[0][3] = translate.x();
	transform[1][3] = translate.y();
	transform[2][3] = translate.z();
}

// RenderCube
void RenderCube()
{
	glBegin( GL_QUADS );
		// Front Face
		glNormal3f( 0.0f, 0.0f, 0.5f);
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
		// Top Face
		glNormal3f( 0.0f, 0.5f, 0.0f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
		// Bottom Face
		glNormal3f( 0.0f,-0.5f, 0.0f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
		glVertex3f(-0.5f, -0.5f,  0.5f);
		// Right face
		glNormal3f( 0.5f, 0.0f, 0.0f);
		glVertex3f( 0.5f, -0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f, -0.5f);
		glVertex3f( 0.5f,  0.5f,  0.5f);
		glVertex3f( 0.5f, -0.5f,  0.5f);
		// Left Face
		glNormal3f(-0.5f, 0.0f, 0.0f);	
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f,  0.5f);
		glVertex3f(-0.5f,  0.5f, -0.5f);
	glEnd();
}


