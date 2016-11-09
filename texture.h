#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <fstream>
using namespace std;


// image_t
class image_t
{
	private:
		bool LoadUncompressedTGA( char * filename, fstream& fs );
		bool LoadCompressedTGA( char * filename, fstream& fs );
	public:
		int   width;
		int   height;
		int   bpp;
		int   type;
		char* data;
		
		 image_t() { data=NULL; }
		~image_t() { if( data ) delete [] data; }
		bool LoadTGA( char* filename );
};


// texture_t
class texture_t
{
	public: 
		int         width;
		int         height;
		int         bpp;
		int         id;
		int         type;
		
		bool Load( char* filename );
		void Unload();
};



#endif