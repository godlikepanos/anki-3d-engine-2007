#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "local.h"
#include "math.h"

/*
========
particle_t
========
*/
class particle_t: public object_t
{
	public:
		vec3_t v0;
		vec3_t a0;
		vec3_t v1;
		vec3_t a1;
		int life; // frames to death
		
		particle_t() {life = 0;}
		
		void Render();
};


/*
========
particle_emitter_t
========
*/
class particle_emitter_t: public object_t
{
	public:
		array_t<particle_t> particles;
		
		int start_frame;
		int stop_frame;
		int frame;
		bool repeat_emittion;
		int particle_life[2];  // []
		int particles_per_frame;
		
		vec3_t particles_local_translate[2]; // []
		
		euler_t v0_angs[2]; // []
		float v0_magnitude;
		float a0_magnitude;
		euler_t v1_angs[2]; // []
		float v1_magnitude;
		float a1_magnitude;
		
		//float initial_fade; // []
		//int fade_frame;
		//float fade_rate;
		//
		//float initial_size; // []
		//int size_frame;
		//float size_rate;
		
		
		particle_emitter_t(){}
		~particle_emitter_t(){ particles.Free(); }
		
		void Init();
		void ReInitParticle( particle_t& particle );
		void Render();
		
};

#endif