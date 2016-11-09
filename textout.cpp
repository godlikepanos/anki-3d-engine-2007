#include "textout.h"
#ifdef WIN32
	#include <windows.h>
#endif
#include <gl/gl.h>


textout_t tout;


// Init
// exec after init SDL
void textout_t::Init() 
{
	font_map.Load( "gfx/fontmap_frank.tga" );
	SetPos( 0.0f, 0.0f );
	SetFontW( 0.05f );
	SetColor( 1.0f, 1.0f, 1.0f );
	italic = false;
}


// SetAll
// sets all params
void textout_t::SetAll( float x_, float y_, float font_w_, float font_h_, float red, float green, float blue, bool italic_ )
{
	SetPos( x_, y_ );
	SetColor( red, green, blue );
	font_w = font_w_;  font_h = font_h_;
	italic = italic_;
}


// SetFontW
// sets font width from the given param and the height fron the aspect ratio
void textout_t::SetFontW( float w_ )
{
	// width
	font_w = w_;
	// height
	float args[4];
	glGetFloatv( GL_VIEWPORT, args );
	font_h = font_w*( args[2]/args[3] );
}


// SetGL
void textout_t::SetGL()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glDisable( GL_LIGHTING );
	//glDisable( GL_DEPTH_TEST );
	glColor3f( color[0], color[1], color[2] );
	glPolygonMode( GL_FRONT, GL_FILL );
	
	glBindTexture( GL_TEXTURE_2D, font_map.id );
	
	// matrix stuff
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );
	
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	
}


// RestoreGL
void textout_t::RestoreGL()
{
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}


// DrawChar
void textout_t::DrawChar( char c )
{
	const float z = 0.0f;
	// first check for special chars
	if( c=='\n' ) // new line
	{
		float m4[16];
		glGetFloatv( GL_MODELVIEW_MATRIX, &m4[0] ); // get current x pos
		glTranslatef( x-m4[12], -font_h, 0.0f );
		return;
	}
	if( c=='\t' ) // tan
	{
		DrawChar( ' ' );
		DrawChar( ' ' );
		return;
	}
	if( c<' ' || c>'~' ) // out of range
	{
		c = '~'+1;
	}
	
	const int chars_per_line = 16; // the chars that font_map.tga has per line
	const int lines_num       = 8; // the lines of chars in font_map.tga
	
	// the uvs
	float char_width = 1.0f/float(chars_per_line);
	float char_height = 1.0f/float(lines_num);
	float uv_top = float(lines_num - (c-' ')/chars_per_line) / float(lines_num);
	float uv_left = float( (c-' ')%chars_per_line ) / float(chars_per_line);
	float uv_right = uv_left + char_width;
	float uv_bottom = uv_top - char_height;
	float uvs[4][2] = { {uv_left, uv_top}, {uv_left, uv_bottom}, {uv_right, uv_bottom}, {uv_right, uv_top} };
	
	// the coords
	float fwh = font_w/2.0f;
	float fhh = font_h/2.0f;
	float coords[4][2] = { {-fwh, fhh}, {-fwh, -fhh}, {fwh, -fhh}, {fwh, fhh} }; // fron top left counterclockwise
	
	
	if( italic )
	{
		coords[0][0] += font_w/5.0f;
		coords[3][0] += font_w/5.0f;
	}
	
	glBegin(GL_QUADS);
		glTexCoord2fv( uvs[0] );  // top left
		glVertex2fv( coords[0] ); 
		glTexCoord2fv( uvs[1] );  // bottom left
		glVertex2fv( coords[1] );
		glTexCoord2fv( uvs[2] );  // botton right
		glVertex2fv( coords[2] );
		glTexCoord2fv( uvs[3] );  // top right
		glVertex2fv( coords[3] );		
	glEnd();
	
	// draw outline
	/*if( 1 )
	glDisable( GL_TEXTURE_2D );
	glColor3f( 0.0, 0.0, 1.0 );
	glBegin(GL_LINES);
		glVertex2fv( coords[0] ); 
		glVertex2fv( coords[1] );
		glVertex2fv( coords[2] );
		glVertex2fv( coords[3] );
	glEnd();
	glEnable( GL_TEXTURE_2D );*/
	// end draw outline
	
	glTranslatef( font_w*0.8f, 0.0, 0.0 ); // font_w*0.?? to remove the space
}


// Printf
void textout_t::Printf( const char* format, ... )
{
	va_list ap;
	char text[200];
	
	va_start(ap, format);		// Parses The String For Variables
	    vsprintf(text, format, ap);  // And Converts Symbols To Actual Numbers
	va_end(ap);
	
	SetGL();
	glTranslatef( x, y, 0.0f );
	for( int i=0; i<(int)strlen(text); i++ )
		DrawChar( text[i] );
	RestoreGL();
}


//Print
void textout_t::Print( const char* text )
{
	SetGL();
	glTranslatef( x, y, 0.0f );
	for( int i=0; i<(signed int)strlen(text); i++ )
		DrawChar( text[i] );
	RestoreGL();
}