#ifndef _LOCAL_H_
#define _LOCAL_H_

#include <stdio.h>
#include <memory.h>
#include "math.h"


// misc
extern int   RandRange( int min, int max );
extern float RandRange( float min, float max );
extern void RenderCube();
extern void TRS( vec3_t& translate, mat3_t& rotate, float scale, mat4_t& transform );

// array_t
template <class type_t> class array_t
{
	private:
		void Reset() { data=NULL; size=NULL; }
		
	public:
		// data
		type_t* data;
		int size;
		
		// []
		type_t& operator []( const int i )const  
		{ 
#ifdef _DEBUG
			if( i >= size || i < 0 )
				cout<< "--WARNING (array_t::operator []): Array out of bounts"<< endl;
#endif
			return data[i];
		}
		
		// = 
		array_t<type_t>& operator = ( array_t<type_t>& from ) // cupy from array_t to array_t
		{
			Free(); 
			Malloc( from.size );
			memcpy( data, from.data, size*sizeof(type_t) );
			return *this;
		}
		
		// Malloc
		void Malloc( int size_ ) 
		{ 
			size=size_; 
			data=new type_t [size]; 
		}
		
		// free
		void Free()
		{ 
			if( size!=0 ) 
			{
				delete [] data;
				size=0; 
			}
		}
		
		// constuctors & distructors
		 array_t()            { Reset(); }
		 array_t( int size_ ) { Reset(); Malloc(size_); }
		~array_t()            { Free(); }
};


/*
========
Cpy
copy an object to another
========
*/
template <class type0_t, class type1_t> void Cpy( type0_t& in, type0_t& out )
{
#ifdef _DEBUG
	if( sizeof(type0_t)!=sizeof(type1_t) )
	{
		cout << "-->ERROR (Cpy): The types are not the same" << endl;
		exit(0);
	}
#endif
	memcpy( &out, &in, sizeof(type0_t) );
}



// vertex_t
class vertex_t
{
	public:
		vec3_t coords;
		vec3_t normal;
};


// triangle_t
class triangle_t
{
	public:
		int    vert_ids[3]; // an array with the vertex indexes in the mesh class
		vec3_t normal;
};


// pose_t
struct pose_t
{
	mat4_t matrix;
	quat_t quat;
	vec3_t loc;
	vec3_t scale;
	int    keyframe;
};

/*
==========
object_t
==========
*/
class object_t
{
	public:
		vec3_t local_translate;
		float  local_scale;
		mat3_t local_rotate;
		
		vec3_t world_translate;
		float  world_scale;
		mat3_t world_rotate;
		
		object_t* parent;
		
		virtual void Render() = 0;
		
		object_t()
		{
			parent = NULL;
			local_translate.LoadZero();
			local_scale = 1.0;
			local_rotate.LoadIdent();
			world_translate.LoadZero();
			world_scale = 1.0;
			world_rotate.LoadIdent();
		}
		
		void UpdateWorldTransform()
		{
			if( parent )
			{
				world_scale = parent->world_scale * local_scale;
				world_rotate = parent->world_rotate * local_rotate;
				world_translate = parent->world_translate + (parent->world_rotate*parent->world_scale)*local_translate;
			}
			else
			{
				world_scale = local_scale;
				world_rotate = local_rotate;
				world_translate = local_translate;
			}
		}
};



#endif