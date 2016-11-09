#ifndef _TEXTOUT_H_
#define _TEXTOUT_H_

#include "texture.h"


class textout_t
{
	private:
		texture_t font_map;
		//float tx; // changes as the DrawChar goes
		//float ty; //             >>
		
		void SetGL();        // set gl state machine
		void RestoreGL();    // restor only the matrices
		void DrawChar( char c );
				
	public:
		float font_w;
		float font_h;
		float color[3];
		bool  italic;
		float x;
		float y;
		
		void Init(); // exec after init SDL
		void SetAll( float x_, float y_, float font_w_, float font_h_, float red, float green, float blue, bool italic_ );
		void SetColor( float red, float green, float blue ) { color[0]=red; color[1]=green; color[2]=blue; }
		void SetPos( float x_, float y_ ){ x=x_; y=y_; };
		void SetFontW( float w_ );
		void Printf( const char* format, ... );
		void Print( const char* str );
};


extern textout_t tout;


#endif