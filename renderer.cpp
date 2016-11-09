#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <gl/glaux.h>
#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "texture.h"

renderer_t renderer;


// Init
void renderer_t::Init()
{
	glEnable( GL_TEXTURE_2D );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	glClearColor( 0.1f, 0.1f, 0.1f, 0.0f );
	glClearDepth( 1.0f );
	glColor3f( 0.5, 0.5, 0.5 );
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// lights
	static float light_col[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	static float light_defuse[]= { 0.7f, 1.0f, 0.7f, 1.0f };
	static float light_spec[] = { 1.0, 0.5, 0.9, 0.0 };
	static float light_pos[] = { 0.0, 1.1, 3.0, 1.0 };
	
	static float mat_shininess[] = { 10.0 };

	glLightfv( GL_LIGHT1, GL_AMBIENT, light_col );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, light_defuse );
	glLightfv( GL_LIGHT1, GL_SPECULAR, light_spec );
	glLightfv( GL_LIGHT1, GL_POSITION, light_pos );

	glMaterialfv( GL_FRONT, GL_SPECULAR, light_spec );
	glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );
	
	glEnable( GL_LIGHT1 );
	glEnable( GL_LIGHTING );

	
	/*glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );*/


	// on resize
	glViewport(0, 0, w, h);
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 300.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	// misc
	glPointSize( 2.0 );
	glLineWidth( 1.0 );
	glCullFace(GL_BACK);
	glPolygonMode( GL_BACK, GL_LINE );
	//glEnable(GL_CULL_FACE);

}


// Run
void renderer_t::Run()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	frames_num++;
}

// DrawGrid
void renderer_t::DrawGrid()
{
	float col0[] = { 0.5f, 0.5f, 0.5f };
	float col1[] = { 0.0f, 0.0f, 1.0f };
	float col2[] = { 1.0f, 0.0f, 0.0f };
	
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glColor3fv( col0 );
	glLineWidth(1.0);
	
	float space = 1.0; // space between lines
	int num = 57;  // lines number. must be odd
	
	float opt = ((num-1)*space/2);
	glBegin( GL_LINES );
		for( int x=0; x<num; x++ )
		{
			if( x==num/2 ) // if the middle line then change color
				glColor3fv( col1 );
			else if( x==(num/2)+1 ) // if the next line after the middle one change back to default col
				glColor3fv( col0 );
		
			float opt1 = (x*space);
			// line in z 
			glVertex3f( opt1-opt, 0.0, -opt );
			glVertex3f( opt1-opt, 0.0, opt );
			
			if( x==num/2 ) // if middle line change col so you can highlight the x-axis
				glColor3fv( col2 );
				
			// line in the x
			glVertex3f( -opt, 0.0, opt1-opt );
			glVertex3f( opt, 0.0, opt1-opt );	
		}
	glEnd();
}