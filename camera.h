#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "local.h"
#include "math.h"


class camera_t: public object_t
{
	public:
		vec3_t pos;
		vec3_t viewdir;
		vec3_t right;
		vec3_t up;
		float  ang_x;
		float  ang_y;
		float  ang_z;
		
		camera_t();
		
		void Move( vec3_t direction );
		void RotateX( float ang );
		void RotateY( float ang );
		void RotateZ( float ang );
		void MoveForward( float dist );
		void MoveUpward( float dist );
		void StrafeRight( float dist );
		
		void RepositionScene();
		void Render(){};
};


extern camera_t cam;

#endif