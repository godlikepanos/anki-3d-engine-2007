#include "particles.h"
#include "handlers.h"
#ifdef WIN32
	#include <windows.h>
#endif
#include <gl/gl.h>
#include <iostream>

using namespace std;

/*
========
Render @ particle_t
========
*/
void particle_t::Render()
{
	if( !life ) return;
	glPointSize( 4.0 );
	
	// calc new pos
	float dt = ( float(TIMER_TICK)/1000 );
	v0 = (a0 * dt) + v0;
	v1 = (a1 * dt) + v1;
	world_translate = (v0+v1) * dt + world_translate;
	--life;
	
	// draw the point
	glColor3f( 1.0, 0.0, 0.0 );
	glBegin( GL_POINTS );
		glVertex3fv( &world_translate[0] );
	glEnd();
	
	// draw axis
	if( 0 )
	{
		glPushMatrix();
		mat4_t m4;
		TRS( world_translate, world_rotate, world_scale, m4 );
		m4.Transpose();
		glMultMatrixf( &m4[0][0] );
		
		glBegin( GL_LINES );
			// x-axis
			glColor3f( 1.0, 0.0, 0.0 );
			glVertex3f( 0.0, 0.0, 0.0 );
			glVertex3f( 1.0, 0.0, 0.0 );
			// y-axis
			glColor3f( 0.0, 1.0, 0.0 );
			glVertex3f( 0.0, 0.0, 0.0 );
			glVertex3f( 0.0, 1.0, 0.0 );
			// z-axis
			glColor3f( 0.0, 0.0, 1.0 );
			glVertex3f( 0.0, 0.0, 0.0 );
			glVertex3f( 0.0, 0.0, 1.0 );
		glEnd();
		
		glPopMatrix();
	}
	
	// debug 1
	if( 0 )
	{
		// draw the v0
		glColor3f( 0.0, 0.0, 1.0 );
		glBegin( GL_LINES );
			glVertex3fv( &world_translate[0] );
			glVertex3fv( &(v0+world_translate)[0] );
		glEnd();
		
		// draw the v1
		glColor3f( 0.0, 1.0, 0.0 );
		glBegin( GL_LINES );
			glVertex3fv( &world_translate[0] );
			glVertex3fv( &(v1+world_translate)[0] );
		glEnd();
		
		// draw the Sv
		glColor3f( 1.0, 1.0, 1.0 );
		glBegin( GL_LINES );
			glVertex3fv( &world_translate[0] );
			glVertex3fv( &(v0+v1+world_translate)[0] );
		glEnd();
	}
};


/*
========
Init @ particle_emitter_t
========
*/
void particle_emitter_t::Init()
{
	particles.Malloc(200);
	for( int i=0; i<particles.size; i++ )
		particles[i].parent = this;
	
	particle_life[0] = 200;
	particle_life[1] = 400;
	particles_per_frame = 1;
	
	v0_angs[0] = euler_t( 0.0, -30.0, 10.0 );
	v0_angs[1] = euler_t( 0.0, 30.0, 45.0 );
	v0_magnitude = 5.0;
	a0_magnitude = 0.0;
	
	v1_angs[0] = euler_t( 0.0, 0.0, 270.0 );
	v1_angs[1] = euler_t( 0.0, 0.0, 270.0 );
	v1_magnitude = 0.0;
	a1_magnitude = 1.0;
	
	particles_local_translate[0] = vec3_t( 0.0, 0.0, 0.0 );
	particles_local_translate[1] = vec3_t( 0.0, 0.0, 0.0 );
}


/*
========
ReInitParticle @ particle_emitter_t
========
*/
void particle_emitter_t::ReInitParticle( particle_t& particle )
{
	particle.life = RandRange( particle_life[0], particle_life[1] );
	particle.local_translate = vec3_t( 
		RandRange( particles_local_translate[0].x(), particles_local_translate[1].x() ),
		RandRange( particles_local_translate[0].y(), particles_local_translate[1].y() ),
		RandRange( particles_local_translate[0].z(), particles_local_translate[1].z() )
	);
	particle.UpdateWorldTransform();
	
	euler_t tmp_angs = euler_t(
		RandRange( v0_angs[0].x(), v0_angs[1].x() ),
		RandRange( v0_angs[0].y(), v0_angs[1].y() ),
		RandRange( v0_angs[0].z(), v0_angs[1].z() )
	);
	mat3_t m3;
	m3.LoadEuler( tmp_angs );
	particle.v0 = particle.a0 = m3 * vec3_t( 1.0, 0.0, 0.0 );
	particle.v0 *= v0_magnitude;
	particle.a0 *= a0_magnitude;
	
	tmp_angs = euler_t(
		RandRange( v1_angs[0].x(), v1_angs[1].x() ),
		RandRange( v1_angs[0].y(), v1_angs[1].y() ),
		RandRange( v1_angs[0].z(), v1_angs[1].z() ) 
	);
	m3.LoadEuler( tmp_angs );
	particle.v1 = particle.a1 = m3 * vec3_t( 1.0, 0.0, 0.0 );
	particle.v1 *= v1_magnitude;
	particle.a1 *= a1_magnitude;
}


/*
========
Render @ particle_emitter_t
========
*/
void particle_emitter_t::Render()
{
	UpdateWorldTransform();

	// emitt new particles
	int remain = particles_per_frame;
	for( int i=0; i<particles.size; i++ )
	{
		if( particles[i].life != 0 ) continue;
		
		ReInitParticle( particles[i] );
		
		--remain;
		if( remain == 0 ) break;
	}
	
	// render all particles
	for( int i=0; i<particles.size; i++ )
		particles[i].Render();
		
	// render the debug cube
	if( 1 )
	{
		glEnable( GL_DEPTH_TEST );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_BLEND );
		glLineWidth( 2.0 );
		glPolygonMode( GL_FRONT, GL_LINE );
		
		glPushMatrix();
		
		mat4_t transform;
		UpdateWorldTransform();
		TRS( world_translate, world_rotate, world_scale, transform );
		transform.Transpose();
		glMultMatrixf( &transform[0][0] );
		
		glColor3f( 0.0, 1.0, 0.0 );
		
		RenderCube();
		
		glPopMatrix();
	}
}
