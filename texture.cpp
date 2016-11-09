#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "texture.h"

using namespace std;


static unsigned char tga_header_uncompressed[12] = {0,0,2,0,0,0,0,0,0,0,0,0};
static unsigned char tga_header_compressed[12]   = {0,0,10,0,0,0,0,0,0,0,0,0};


// image_t::LoadUncompressedTGA
bool image_t::LoadUncompressedTGA( char *filename, fstream& fs )
{
	// read the info from header
	unsigned char header6[6];
	fs.read( (char*)&header6[0], sizeof(header6) );
	if( fs.gcount() != sizeof(header6) )
	{										
		cout << "--ERROR (image_t::LoadUncompressedTGA): File \"" << filename << "\": Could not read info header." << endl;
		return false;
	}	

	width  = header6[1] * 256 + header6[0];
	height = header6[3] * 256 + header6[2];
	bpp	= header6[4];

	if( (width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)) )
	{
		cout << "--ERROR (image_t::LoadUncompressedTGA): File \"" << filename << "\": Invalid image information." << endl;
		return false;
	}

	if( bpp == 24 )
		type	= GL_RGB;
	else
		type	= GL_RGBA;

	// read the data
	int bytes_per_pxl	= (bpp / 8);
	int image_size = bytes_per_pxl * width * height;
	data = new char [ image_size ];

	fs.read( data, image_size );
	if( fs.gcount() != image_size )
	{
		cout << "--ERROR (image_t::LoadUncompressedTGA): File \"" << filename << "\": Could not read image data." << endl;
		return false;
	}

	for( int cswap = 0; cswap < image_size; cswap += bytes_per_pxl )
	{
		data[cswap] ^= data[cswap+2] ^= data[cswap] ^= data[cswap+2];
	}
	return true;	
}


// image_t::LoadCompressedTGA
bool image_t::LoadCompressedTGA( char * filename, fstream& fs )
{
	unsigned char header6[6];
	fs.read( (char*)&header6[0], sizeof(header6) );
	if( fs.gcount() != sizeof(header6) )
	{
		cout << "--ERROR (image_t::LoadCompressedTGA): File \"" << filename << "\": Could not read info header." << endl;
		return false;
	}

	width  = header6[1] * 256 + header6[0];
	height = header6[3] * 256 + header6[2];
	bpp	= header6[4];

	if( (width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)) )
	{
		cout << "--ERROR (image_t::LoadCompressedTGA): File \"" << filename << "\": Invalid texture information." << endl;
		return false;
	}

	if( bpp == 24 )
		type = GL_RGB;
	else
		type = GL_RGBA;

	int bytes_per_pxl = (bpp / 8);
	int image_size = bytes_per_pxl * width * height;
	data = new char [image_size];

	unsigned int pixelcount = height * width;
	unsigned int currentpixel = 0;
	unsigned int currentbyte	= 0;
	unsigned char colorbuffer [4];

	do
	{
		unsigned char chunkheader = 0;

		fs.read( (char*)&chunkheader, sizeof(unsigned char) );
		if( fs.gcount() != sizeof(unsigned char) )
		{
			cout << "--ERROR (image_t::LoadCompressedTGA): File \"" << filename << "\": Could not read RLE header." << endl;
			return false;
		}

		if( chunkheader < 128 )
		{
			chunkheader++;
			for( int counter = 0; counter < chunkheader; counter++ )
			{
				fs.read( (char*)&colorbuffer[0], bytes_per_pxl );
				if( fs.gcount() != bytes_per_pxl )
				{
					cout << "--ERROR (image_t::LoadCompressedTGA): File \"" << filename << "\": Could not read image data." << endl;
					return false;
				}
				
				data[currentbyte		] = colorbuffer[2];
				data[currentbyte + 1] = colorbuffer[1];
				data[currentbyte + 2] = colorbuffer[0];

				if( bytes_per_pxl == 4 )
				{
					data[currentbyte + 3] = colorbuffer[3];
				}

				currentbyte += bytes_per_pxl;
				currentpixel++;

				if( currentpixel > pixelcount )
				{
					cout << "--ERROR (image_t::LoadCompressedTGA): File \"" << filename << "\": Too many pixels read." << endl;
					return false;
				}
			}
		}
		else
		{
			chunkheader -= 127;
			fs.read( (char*)&colorbuffer[0], bytes_per_pxl );
			if( fs.gcount() != bytes_per_pxl )
			{	
				cout << "--ERROR (image_t::LoadCompressedTGA): File \"" << filename << "\": Could not read from file." << endl;
				return false;
			}

			for( int counter = 0; counter < chunkheader; counter++ )
			{
				data[currentbyte] = colorbuffer[2];
				data[currentbyte+1] = colorbuffer[1];
				data[currentbyte+2] = colorbuffer[0];

				if( bytes_per_pxl == 4 )
				{
					data[currentbyte + 3] = colorbuffer[3];
				}

				currentbyte += bytes_per_pxl;
				currentpixel++;

				if( currentpixel > pixelcount )
				{
					cout << "--ERROR (image_t::LoadCompressedTGA): File \"" << filename << "\": Too many pixels read." << endl;
					return false;
				}
			}
		}
	} while(currentpixel < pixelcount);
	
	return true;
}


/*
image_t::LoadTGA
Load a tga using the help of the above
*/
bool image_t::LoadTGA( char* filename )
{
	fstream fs;
	char my_tga_header[12];
	fs.open( filename, ios::in|ios::binary );

	if( !fs.good() )
	{
		cout << "--ERROR (image_t::LoadTGA): File \"" << filename << "\": Could not open file." << endl;
		return false;
	}

	fs.read( &my_tga_header[0], sizeof(my_tga_header) );
	if( fs.gcount() != sizeof(my_tga_header) )
	{
		cout << "--ERROR (image_t::LoadTGA): File \"" << filename << "\": Could not read file header." << endl;
		fs.close();
		return false;
	}

	bool funcs_return;
	if( memcmp(tga_header_uncompressed, &my_tga_header[0], sizeof(my_tga_header)) == 0 )
	{
		funcs_return = LoadUncompressedTGA(filename, fs);
	}
	else if( memcmp(tga_header_compressed, &my_tga_header[0], sizeof(my_tga_header)) == 0 )
	{
		funcs_return = LoadCompressedTGA(filename, fs);
	}
	else
	{
		cout << "--ERROR (image_t::LoadTGA): File \"" << filename << "\": Invalid image header." << endl;
		funcs_return = false;
	}
	
	fs.close();
	return funcs_return;	
}


/*
texture_t::Load
Load a texture file (tga supported) and bind it
*/
bool texture_t::Load( char* filename )
{
	char* c;
	image_t* img = 0;
	
	// get file extension
	c = strrchr( filename, '.' );
	++c;
	
	if( strcmp(c, "tga")==0 )
	{
		img = new image_t;
		// load it
		if( !img->LoadTGA( filename ) )  { delete img; return false; }
	}
	else
	{
		cout << "--ERROR (texture_t::Load): File \"" << filename << "\" has unsupported extension." << endl;
		return 0;
	}

	//
	width = img->width;
	height = img->height;
	bpp = img->bpp;
	type = img->type;

	// bind the texture
	glGenTextures( 1, (GLuint*)&id );
	glBindTexture( GL_TEXTURE_2D, id );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexImage2D( GL_TEXTURE_2D, 0, img->bpp/8, img->width, img->height, 0, img->type, GL_UNSIGNED_BYTE, img->data );

	delete img;
	
	return true;
}


// texture_t::Unload
void texture_t::Unload()
{
	glDeleteTextures( 1, (GLuint*)&id );
}