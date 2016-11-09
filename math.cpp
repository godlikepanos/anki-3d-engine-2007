#include "math.h"
#include <memory.h>
#include <math.h>
#include <iostream>
#include <iomanip>
using namespace std;


/*
==========
misc
==========
*/
#define EPSILON 1.192092896e-07F

static float identity_mat4[4][4] = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
static float identity_mat3[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} };

// return 1/sqrt(f)
static float InvSqrt( float f )
{
	float fhalf = 0.5f*f;
	int i = *(int*)&f;
	i = 0x5F3759DF - (i>>1);
	f = *(float*)&i;
	f *= 1.5f - fhalf*f*f;
	return f;
}


// to rad
float ToRad( const float degrees )
{
	return degrees*(PI/180.0f);
}

float ToDegrees( const float rad )
{
	return rad*(180.0f/PI);
}


/*
==========
vec2_t
==========
*/

// copy
vec2_t& vec2_t::operator = ( vec2_t& in )
{
	memcpy( this, &in, sizeof(vec2_t) );
	return *this;
}

// +
vec2_t vec2_t::operator + ( vec2_t& b )
{
	vec2_t c;
	c[0]=v[0]+b[0];  c[1]=v[1]+b[1];
	return c;
}

// +=
vec2_t& vec2_t::operator += ( vec2_t& b )
{
	v[0]+=b[0];  v[1]+=b[1];
	return *this;
}

// -
vec2_t vec2_t::operator - ( vec2_t& b )
{
	vec2_t c;
	c[0]=v[0]-b[0];  c[1]=v[1]-b[1];
	return c;
}

// -=
vec2_t& vec2_t::operator -= ( vec2_t& b )
{
	v[0]-=b[0];  v[1]-=b[1];
	return *this;
}

// scale
vec2_t vec2_t::operator * ( float f )
{
	vec2_t c;
	c[0]=v[0]*f;  c[1]=v[1]*f;
	return c;
}

// scale
vec2_t& vec2_t::operator *= (float f)
{
	x()*=f;  y()*=f;
	return *this;
}

// scale
vec2_t vec2_t::operator / ( float f )
{
	vec2_t c;
	if( f==0.0 )
		c.LoadZero();
	else
	{
		c[0]=v[0]/f;  c[1]=v[1]/f;
	}
	return c;
}

// scale
vec2_t& vec2_t::operator /= (float f)
{
	if( f==0.0 )
		LoadZero();
	else
	{
		x()/=f;  y()/=f;
	}
	return *this;
}

// Length
float vec2_t::Length()
{
	return 1.0f/InvSqrt( x()*x() + y()*y() );
}

// load zero
void vec2_t::LoadZero()
{
	memset( this, 0, sizeof(vec2_t) );
}


/*
==========
vec3_t
==========
*/

// copy
vec3_t& vec3_t::operator =( vec3_t& in )
{
	memcpy( this, &in, sizeof(vec3_t) );
	return *this;
}


// +
vec3_t vec3_t::operator +( vec3_t& b )
{
	return vec3_t(
		v[0]+b[0],
		v[1]+b[1],
		v[2]+b[2]
	);
}

// +=
vec3_t& vec3_t::operator +=( vec3_t& b )
{
	v[0]+=b[0];  v[1]+=b[1];  v[2]+=b[2];
	return *this;
}

// -
vec3_t vec3_t::operator -( vec3_t& b )
{
	return vec3_t(
		v[0]-b[0],
		v[1]-b[1],
		v[2]-b[2]
	);
}

// -=
vec3_t& vec3_t::operator -=( vec3_t& b )
{
	v[0]-=b[0];  v[1]-=b[1];  v[2]-=b[2];
	return *this;
}

// cross prod
vec3_t vec3_t::operator *( vec3_t& b )
{
	return vec3_t( 
		v[1]*b[2] - v[2]*b[1],
		v[2]*b[0] - v[0]*b[2],
		v[0]*b[1] - v[1]*b[0]
	);
}

// cross prod
vec3_t& vec3_t::operator *=( vec3_t& b )
{
	return (*this) = vec3_t(
		v[1]*b[2] - v[2]*b[1],
		v[2]*b[0] - v[0]*b[2],
		v[0]*b[1] - v[1]*b[0]
	);
}

// scale
vec3_t vec3_t::operator *( float f )
{
	return vec3_t(
		v[0]*f,
		v[1]*f,
		v[2]*f
	);
}

// scale
vec3_t& vec3_t::operator *=( float f )
{
	v[0]*=f;  v[1]*=f;  v[2]*=f;
	return *this;
}

// down-scale
vec3_t vec3_t::operator /( float f )
{
	return vec3_t(
		v[0]/f,
		v[1]/f,
		v[2]/f
	);
}

// down-scale
vec3_t& vec3_t::operator /=( float f )
{
	v[0]/=f;  v[1]/=f;  v[2]/=f;
	return *this;
}

// DotProd
float vec3_t::DotProd( vec3_t& b )
{
	return x()*b.x() + y()*b.y() + z()*b.z();
}

// Project b to this
vec3_t vec3_t::Project( vec3_t& b )
{
	return vec3_t( b * ( DotProd(b) / b.DotProd(b) ) );
}

// Length
float vec3_t::Length()
{
	return 1.0f/InvSqrt( x()*x() + y()*y() + z()*z() );
}

// Normalize
void vec3_t::Normalize()
{
	float invsqrt = InvSqrt( x()*x() + y()*y() + z()*z() );
	*this *= invsqrt;
}

// VecsMiddle return middle vec3_t from this and b
vec3_t vec3_t::VecsMiddle( vec3_t& b )
{
	return ((*this)+b)*0.5;
}


void vec3_t::LoadZero()
{
	memset( this, 0, sizeof(vec3_t) );
}


void vec3_t::LoadQuat( quat_t& q )
{
	x()=q.x();  y()=q.y();  x()=q.y();
}


void vec3_t::Print()
{
	for( int i=0; i<3; i++ )
		cout << fixed << v[i] << " ";
	cout << "\n" << endl;
}


void vec3_t::Lerp( vec3_t v0, vec3_t v1, float t )
{
	(*this) = (v0*(1.0f-t))+(v1*t);
}


/*
==========
quaternions
==========
*/

// copy
quat_t& quat_t::operator =( quat_t& b )
{
	memcpy( this, &b, sizeof(quat_t) );
	return *this;
}

// +
quat_t quat_t::operator +( quat_t& b )
{
	quat_t c;
	c.x()=x()+b.x();  c.y()=y()+b.y();  c.z()=z()+b.z();  c.w()=w()+b.w();
	return c;
}

// +=
quat_t& quat_t::operator +=( quat_t& b )
{
	x()=x()+b.x();  y()=y()+b.y();  z()=z()+b.z();  w()=w()+b.w();
	return (*this);
}

// -
quat_t quat_t::operator -( quat_t& b )
{
	quat_t c;
	c.x()=x()-b.x();  c.y()=y()-b.y();  c.z()=z()-b.z();  c.w()=w()-b.w();
	return c;
}

// -=
quat_t& quat_t::operator -=( quat_t& b )
{
	x()=x()-b.x();  y()=y()-b.y();  z()=z()-b.z();  w()=w()-b.w();
	return (*this);
}

// *
quat_t quat_t::operator * ( quat_t& b )
{
	quat_t c;
	c.x() =  x() * b.w() + y() * b.z() - z() * b.y() + w() * b.x();
	c.y() = -x() * b.z() + y() * b.w() + z() * b.x() + w() * b.y();
	c.z() =  x() * b.y() - y() * b.x() + z() * b.w() + w() * b.z();
	c.w() = -x() * b.x() - y() * b.y() - z() * b.z() + w() * b.w();
	return c;
}

// *=
quat_t& quat_t::operator *= ( quat_t& b )
{
	quat_t c;
	c.x() =  x() * b.w() + y() * b.z() - z() * b.y() + w() * b.x();
	c.y() = -x() * b.z() + y() * b.w() + z() * b.x() + w() * b.y();
	c.z() =  x() * b.y() - y() * b.x() + z() * b.w() + w() * b.z();
	c.w() = -x() * b.x() - y() * b.y() - z() * b.z() + w() * b.w();
	(*this) = c;
	return *this;
}

// quat * float
quat_t quat_t::operator * ( float f )
{
	quat_t c;
	c.w()=w()*f;  c.x()=x()*f;  c.y()=y()*f;  c.z()=z()*f;
	return c;
}

// quat * float (self)
quat_t& quat_t::operator *= ( float f )
{
	w()=w()*f;  x()=x()*f;  y()=y()*f;  z()=z()*f;
	return *this;
}


void quat_t::Conj()
{
	x()=-x();  y()=-y();  z()=-z();
}


void quat_t::Normalize()
{
	float invs = InvSqrt( x()*x() + y()*y() + z()*z() + w()*w() );
	(*this) *= invs;
}


void quat_t::LoadIdent()
{
	w()=1.0f;  x()=y()=z()=0.0f;
}


void quat_t::LoadZero()
{
	memset( this, 0, sizeof(quat_t) );
}


void quat_t::Print()
{
	for( int i=0; i<4; i++ )
		cout << fixed << q[i] << " ";
	cout << "\n" << endl;
}

// 3x3 to quat
void quat_t::LoadMat3( mat3_t& a )
{
	float trace = a[0][0] + a[1][1] + a[2][2] + 1.0f;
	if( trace > EPSILON ) 
	{
		float s = 0.5f / sqrtf(trace);
		w() = 0.25f / s;
		x() = ( a[2][1] - a[1][2] ) * s;
		y() = ( a[0][2] - a[2][0] ) * s;
		z() = ( a[1][0] - a[0][1] ) * s;
	} 
	else 
	{
		if ( a[0][0] > a[1][1] && a[0][0] > a[2][2] ) 
		{
			float s = 2.0f * sqrtf( 1.0f + a[0][0] - a[1][1] - a[2][2]);
			w() = (a[1][2] - a[2][1] ) / s;
			x() = 0.25f * s;
			y() = (a[0][1] + a[1][0] ) / s;
			z() = (a[0][2] + a[2][0] ) / s;
		} 
		else if (a[1][1] > a[2][2]) 
		{
			float s = 2.0f * sqrtf( 1.0f + a[1][1] - a[0][0] - a[2][2]);
			w() = (a[0][2] - a[2][0] ) / s;
			x() = (a[0][1] + a[1][0] ) / s;
			y() = 0.25f * s;
			z() = (a[1][2] + a[2][1] ) / s;
		} 
		else 
		{
			float s = 2.0f * sqrtf( 1.0f + a[2][2] - a[0][0] - a[1][1] );
			w() = (a[0][1] - a[1][0] ) / s;
			x() = (a[0][2] + a[2][0] ) / s;
			y() = (a[1][2] + a[2][1] ) / s;
			z() = 0.25f * s;
		}
	}
}

// vec3 to quat
void quat_t::LoadVec3( vec3_t& v )
{
	w()=0.0f;  x()=v.x();  y()=v.y();  z()=v.z();
}

// euler to quat
void quat_t::LoadEuler( euler_t &e )
{
	float c1 = cos( e.heading()/2.0f );
	float s1 = sin( e.heading()/2.0f );
	float c2 = cos( e.attitude()/2.0f );
	float s2 = sin( e.attitude()/2.0f );
	float c3 = cos( e.bank()/2.0f );
	float s3 = sin( e.bank()/2.0f );
	float c1c2 = c1*c2;
	float s1s2 = s1*s2;
	w() = c1c2*c3 - s1s2*s3;
	x() = c1c2*s3 + s1s2*c3;
	y() = s1*c2*c3 + c1*s2*s3;
	z() = c1*s2*c3 - s1*c2*s3;
}


float quat_t::DotProd( quat_t& b )
{
	return w()*b.w() + x()*b.x() + y()*b.y() + z()*b.z();
}


void quat_t::Log()
{
	float theta = acos( w() );
	float sin_theta = sin( theta );
	
	w() = 0.0;
	if( fabs(sin_theta) > EPSILON )
	{
		x() = x()/sin_theta*theta;
		y() = y()/sin_theta*theta;
		z() = z()/sin_theta*theta;
	}
}


void quat_t::Exp()
{
	float theta = sqrt( x()*x() + y()*y() + z()*z() );
	float sin_theta = sin( theta );
	
	w() = cos( theta );
	if( fabs(sin_theta) > EPSILON )
	{
		x() = x()/sin_theta*theta;
		y() = y()/sin_theta*theta;
		z() = z()/sin_theta*theta;
	}
}


void quat_t::Power( float f )
{
	Log();
	(*this) *= f;
	Exp();
}

// SLERP
// q1 != this  &&  q2 != this
void quat_t::Slerp( quat_t& q0, quat_t& q1_, float t )
{
	quat_t q1;
	q1 = q1_;
	float cos_half_theta = q0.w()*q1.w() + q0.x()*q1.x() + q0.y()*q1.y() + q0.z()*q1.z();
	if( cos_half_theta < 0.0f )
	{ 
		q1 *= -1.0f; // quat changes
		cos_half_theta = -cos_half_theta; 
	}
	if( abs(cos_half_theta) >= 1.0f )
	{
		*this = q0;
		return;
	}
	
	float half_theta = acos( cos_half_theta );
	float sin_half_theta = sqrt(1.0f - cos_half_theta*cos_half_theta);

	if( fabs(sin_half_theta) < 0.001f )
	{
		*this = q0*0.5f + q1*0.5f;
		return;
	}
	float ratio_a = sin((1.0f - t) * half_theta) / sin_half_theta;
	float ratio_b = sin(t * half_theta) / sin_half_theta; 
	*this = q0*ratio_a + q1*ratio_b;
}


/*
=========
euler angles
=========
*/

// cpy
euler_t& euler_t::operator = ( euler_t& b )
{
	memcpy( this, &b, sizeof(euler_t) );
	return *this;
}


void euler_t::LoadQuat( quat_t& q )
{
	float test = q.x()*q.y() + q.z()*q.w();
	if (test > 0.499f) 
	{
		heading() = 2.0f * atan2(q.x(),q.w());
		attitude() = PI/2.0f;
		bank() = 0.0f;
		return;
	}
	if (test < -0.499f) 
	{
		heading() = -2.0f * atan2(q.x(),q.w());
		attitude() = -PI/2.0f;
		bank() = 0.0f;
		return;
	}
	float sqx = q.x()*q.x();
	float sqy = q.y()*q.y();
	float sqz = q.z()*q.z();
	heading() = atan2( 2.0f*q.y()*q.w()-2.0f*q.x()*q.z(), 1.0f-2.0f*sqy-2.0f*sqz );
	attitude() = asin(2.0f*test);
	bank() = atan2(2.0f*q.x()*q.w()-2.0f*q.y()*q.z(), 1.0f-2.0f*sqx-2.0f*sqz);
}


// LoadMat3
// !!!!! NOT WORKING GOOD !!!!!
void euler_t::LoadMat3( mat3_t& m3 )
{
	// singularity at north pole
	if( m3[1][0] > 0.998f ) 
	{ 
		heading() = ToDegrees( atan2( m3[0][2], m3[2][2] ) );
		attitude() = ToDegrees( PI/2 );
		bank() = ToDegrees( 0.0f );
		return;
	}
	// singularity at south pole
	if( m3[1][0] < -0.998f ) 
	{ 
		heading() = ToDegrees( atan2( m3[0][2], m3[2][2] ) );
		attitude() = ToDegrees( -PI/2 );
		bank() = ToDegrees( 0.0f );
		return;
	}
	
	heading() = ToDegrees( atan2( -m3[2][0], m3[0][0] ) );
	bank() = ToDegrees( atan2( -m3[1][2], m3[1][1] ) );
	attitude() = ToDegrees( asin( m3[1][0] ) );
}


// Print
void euler_t::Print()
{
	for( int i=0; i<3; i++ )
		cout << fixed << ToRad(v[i]) << char(167) << "/" << v[i] << char(0xF8) << "  ";
	cout << "\n" << endl;
}


/*
==========
matrix 3x3
==========
*/

// copy
mat3_t& mat3_t::operator = ( mat3_t& in )
{
	memcpy( this, &in, sizeof(mat3_t) );
	return *this;
}

// 3x3 + 3x3
mat3_t mat3_t::operator +( mat3_t& b )
{
	mat3_t c;
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			c[i][j] = m[i][j] + b[i][j];
	return c;
}

// 3x3 + 3x3 (self)
mat3_t& mat3_t::operator +=( mat3_t& b )
{
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			m[i][j] = m[i][j] + b[i][j];
	return *this;
}

// 3x3 - 3x3
mat3_t mat3_t::operator -( mat3_t& b )
{
	mat3_t c;
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			c[i][j] = m[i][j] - b[i][j];
	return c;
}

// 3x3 - 3x3 (self)
mat3_t& mat3_t::operator -=( mat3_t& b )
{
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			m[i][j] = m[i][j] - b[i][j];
	return *this;
}

// 3x3 * 3x3
mat3_t mat3_t::operator * ( mat3_t& b )
{
	mat3_t c;
	c[0][0] = m[0][0]*b[0][0] + m[0][1]*b[1][0] + m[0][2]*b[2][0];
	c[0][1] = m[0][0]*b[0][1] + m[0][1]*b[1][1] + m[0][2]*b[2][1];
	c[0][2] = m[0][0]*b[0][2] + m[0][1]*b[1][2] + m[0][2]*b[2][2];
	c[1][0] = m[1][0]*b[0][0] + m[1][1]*b[1][0] + m[1][2]*b[2][0];
	c[1][1] = m[1][0]*b[0][1] + m[1][1]*b[1][1] + m[1][2]*b[2][1];
	c[1][2] = m[1][0]*b[0][2] + m[1][1]*b[1][2] + m[1][2]*b[2][2];
	c[2][0] = m[2][0]*b[0][0] + m[2][1]*b[1][0] + m[2][2]*b[2][0];
	c[2][1] = m[2][0]*b[0][1] + m[2][1]*b[1][1] + m[2][2]*b[2][1];
	c[2][2] = m[2][0]*b[0][2] + m[2][1]*b[1][2] + m[2][2]*b[2][2];
	return c;
}

// 3x3 * 3x3 (self)
mat3_t& mat3_t::operator *=( mat3_t& b )
{
	mat3_t c;
	c[0][0] = m[0][0]*b[0][0] + m[0][1]*b[1][0] + m[0][2]*b[2][0];
	c[0][1] = m[0][0]*b[0][1] + m[0][1]*b[1][1] + m[0][2]*b[2][1];
	c[0][2] = m[0][0]*b[0][2] + m[0][1]*b[1][2] + m[0][2]*b[2][2];
	c[1][0] = m[1][0]*b[0][0] + m[1][1]*b[1][0] + m[1][2]*b[2][0];
	c[1][1] = m[1][0]*b[0][1] + m[1][1]*b[1][1] + m[1][2]*b[2][1];
	c[1][2] = m[1][0]*b[0][2] + m[1][1]*b[1][2] + m[1][2]*b[2][2];
	c[2][0] = m[2][0]*b[0][0] + m[2][1]*b[1][0] + m[2][2]*b[2][0];
	c[2][1] = m[2][0]*b[0][1] + m[2][1]*b[1][1] + m[2][2]*b[2][1];
	c[2][2] = m[2][0]*b[0][2] + m[2][1]*b[1][2] + m[2][2]*b[2][2];
	(*this) = c;
	return (*this);
}

// 3x3 * float
mat3_t mat3_t::operator *( float f )
{
	mat3_t c;
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			c[i][j] = m[i][j] * f;
	return c;
}

// 3x3 * float (self)
mat3_t& mat3_t::operator *=( float f )
{
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			m[i][j] = m[i][j] * f;
	return *this;
}

// 3x3 * vec3
vec3_t mat3_t::operator *( vec3_t& b )
{
	vec3_t c;
	mat3_t& a = *this;
	c[0] = a[0][0]*b[0] + a[0][1]*b[1] + a[0][2]*b[2];
	c[1] = a[1][0]*b[0] + a[1][1]*b[1] + a[1][2]*b[2];
	c[2] = a[2][0]*b[0] + a[2][1]*b[1] + a[2][2]*b[2];
	return c;
}

void mat3_t::LoadQuat( quat_t& q )
{
	float sqw = q.w()*q.w();
	float sqx = q.x()*q.x();
	float sqy = q.y()*q.y();
	float sqz = q.z()*q.z();

	// invs (inverse square length) is only required if quaternion is not already normalised
	float invs = InvSqrt( sqx + sqy + sqz + sqw );
	m[0][0] = ( sqx - sqy - sqz + sqw)*invs ; // since sqw + sqx + sqy + sqz =1/invs*invs
	m[1][1] = (-sqx + sqy - sqz + sqw)*invs ;
	m[2][2] = (-sqx - sqy + sqz + sqw)*invs ;

	float tmp1 = q.x()*q.y();
	float tmp2 = q.z()*q.w();
	m[1][0] = 2.0f * (tmp1 + tmp2)*invs;
	m[0][1] = 2.0f * (tmp1 - tmp2)*invs;

	tmp1 = q.x()*q.z();
	tmp2 = q.y()*q.w();
	m[2][0] = 2.0f * (tmp1 - tmp2)*invs;
	m[0][2] = 2.0f * (tmp1 + tmp2)*invs;
	tmp1 = q.y()*q.z();
	tmp2 = q.x()*q.w();
	m[2][1] = 2.0f * (tmp1 + tmp2)*invs;
	m[1][2] = 2.0f * (tmp1 - tmp2)*invs; 
}


void  mat3_t::LoadEuler( euler_t& e )
{
	float ch = cos( ToRad(e.heading()) );
  float sh = sin( ToRad(e.heading()) );
  float ca = cos( ToRad(e.attitude()) );
  float sa = sin( ToRad(e.attitude()) );
  float cb = cos( ToRad(e.bank()) );
  float sb = sin( ToRad(e.bank()) );

  m[0][0] = ch * ca;
  m[0][1] = sh*sb - ch*sa*cb;
  m[0][2] = ch*sa*sb + sh*cb;
  m[1][0] = sa;
  m[1][1] = ca*cb;
  m[1][2] = -ca*sb;
  m[2][0] = -sh*ca;
  m[2][1] = sh*sa*cb + ch*sb;
  m[2][2] = -sh*sa*sb + ch*cb;
}


void mat3_t::LoadVec3( vec3_t& v )
{
	LoadIdent();
	m[0][2] = v.x();
	m[1][2] = v.y();
	m[2][2] = v.z();
}


void mat3_t::Transpose()
{
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			if( i>j )
			{
				float f = m[i][j];
				m[i][j] = m[j][i];
				m[j][i] = f;
			}
}


void mat3_t::Ortho()
{
	m[0].Normalize();  m[1].Normalize();  m[2].Normalize();
}


void mat3_t::LoadIdent()
{
	memcpy( this, &identity_mat3[0], sizeof(mat3_t) );
}


void mat3_t::LoadZero()
{
	memset( this, 0, sizeof(mat3_t) );
}


void mat3_t::Print()
{
	for( int i=0; i<3; i++ )
	{
		for( int j=0; j<3; j++ )
			cout << fixed << m[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}


float mat3_t::Determinant()
{
	/* precise method:
	return m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] + m[0][2]*m[1][0]*m[2][1] 
	- m[0][0]*m[1][2]*m[2][1] - m[0][1]*m[1][0]*m[2][2] - m[0][2]*m[1][1]*m[2][0];*/
	return m[0][0]*( m[1][1]*m[2][2] - m[1][2]*m[2][1] ) -
	m[0][1]*( m[1][0]*m[2][2] - m[1][2]*m[2][0] ) +
	m[0][2]*( m[0][1]*m[2][1] - m[1][1]*m[2][0] );
	
}

// Invert
// using Gramer's method ( Inv(A) = ( 1/Det(A) ) * Adj(A)  )
void mat3_t::Invert()
{
	mat3_t m3;
	m3 = (*this);
	m[0][0] = m3[1][1]*m3[2][2] - m3[1][2]*m3[2][1];
	m[0][1] = m3[0][2]*m3[2][1] - m3[0][1]*m3[2][2];
	m[0][2] = m3[0][1]*m3[1][2] - m3[0][2]*m3[1][1];
	m[1][0] = m3[1][2]*m3[2][0] - m3[1][0]*m3[2][2];
	m[1][1] = m3[0][0]*m3[2][2] - m3[0][2]*m3[2][0];
	m[1][2] = m3[0][2]*m3[1][0] - m3[0][0]*m3[1][2];
	m[2][0] = m3[1][0]*m3[2][1] - m3[1][1]*m3[2][0];
	m[2][1] = m3[0][1]*m3[2][0] - m3[0][0]*m3[2][1];
	m[2][2] = m3[0][0]*m3[1][1] - m3[0][1]*m3[1][0];
	(*this) *= 1.0f/m3.Determinant();
}


/*
===========
matrix 4x4
===========
*/

// copy
mat4_t& mat4_t::operator =( mat4_t& b )
{
	memcpy( this, &b, sizeof(mat4_t) );
	return *this;
}


// 4x4 + 4x4
mat4_t mat4_t::operator +( mat4_t& b )
{
	mat4_t c;
	for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
			c[i][j] = m[i][j] + b[i][j];
	return c;
}

// 4x4 + 4x4 (self)
mat4_t& mat4_t::operator +=( mat4_t& b )
{
	for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
			m[i][j] = m[i][j] + b[i][j];
	return *this;
}

// 4x4 - 4x4
mat4_t mat4_t::operator -( mat4_t& b )
{
	mat4_t c;
	for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
			c[i][j] = m[i][j] - b[i][j];
	return c;
}

// 4x4 - 4x4 (self)
mat4_t& mat4_t::operator -=( mat4_t& b )
{
	for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
			m[i][j] = m[i][j] - b[i][j];
	return *this;
}

// 4x4 * 4x4
mat4_t mat4_t::operator *( mat4_t& b )
{
	mat4_t c;
	c[0][0] = m[0][0]*b[0][0] + m[0][1]*b[1][0] + m[0][2]*b[2][0] + m[0][3]*b[3][0];
	c[0][1] = m[0][0]*b[0][1] + m[0][1]*b[1][1] + m[0][2]*b[2][1] + m[0][3]*b[3][1];
	c[0][2] = m[0][0]*b[0][2] + m[0][1]*b[1][2] + m[0][2]*b[2][2] + m[0][3]*b[3][2];
	c[0][3] = m[0][0]*b[0][3] + m[0][1]*b[1][3] + m[0][2]*b[2][3] + m[0][3]*b[3][3];
	c[1][0] = m[1][0]*b[0][0] + m[1][1]*b[1][0] + m[1][2]*b[2][0] + m[1][3]*b[3][0];
	c[1][1] = m[1][0]*b[0][1] + m[1][1]*b[1][1] + m[1][2]*b[2][1] + m[1][3]*b[3][1];
	c[1][2] = m[1][0]*b[0][2] + m[1][1]*b[1][2] + m[1][2]*b[2][2] + m[1][3]*b[3][2];
	c[1][3] = m[1][0]*b[0][3] + m[1][1]*b[1][3] + m[1][2]*b[2][3] + m[1][3]*b[3][3];
	c[2][0] = m[2][0]*b[0][0] + m[2][1]*b[1][0] + m[2][2]*b[2][0] + m[2][3]*b[3][0];
	c[2][1] = m[2][0]*b[0][1] + m[2][1]*b[1][1] + m[2][2]*b[2][1] + m[2][3]*b[3][1];
	c[2][2] = m[2][0]*b[0][2] + m[2][1]*b[1][2] + m[2][2]*b[2][2] + m[2][3]*b[3][2];
	c[2][3] = m[2][0]*b[0][3] + m[2][1]*b[1][3] + m[2][2]*b[2][3] + m[2][3]*b[3][3];
	c[3][0] = m[3][0]*b[0][0] + m[3][1]*b[1][0] + m[3][2]*b[2][0] + m[3][3]*b[3][0];
	c[3][1] = m[3][0]*b[0][1] + m[3][1]*b[1][1] + m[3][2]*b[2][1] + m[3][3]*b[3][1];
	c[3][2] = m[3][0]*b[0][2] + m[3][1]*b[1][2] + m[3][2]*b[2][2] + m[3][3]*b[3][2];
	c[3][3] = m[3][0]*b[0][3] + m[3][1]*b[1][3] + m[3][2]*b[2][3] + m[3][3]*b[3][3];
	return c;
}

// 4x4 * 4x4 (self)
mat4_t& mat4_t::operator *=( mat4_t& b )
{
	mat4_t c;
	c[0][0] = m[0][0]*b[0][0] + m[0][1]*b[1][0] + m[0][2]*b[2][0] + m[0][3]*b[3][0];
	c[0][1] = m[0][0]*b[0][1] + m[0][1]*b[1][1] + m[0][2]*b[2][1] + m[0][3]*b[3][1];
	c[0][2] = m[0][0]*b[0][2] + m[0][1]*b[1][2] + m[0][2]*b[2][2] + m[0][3]*b[3][2];
	c[0][3] = m[0][0]*b[0][3] + m[0][1]*b[1][3] + m[0][2]*b[2][3] + m[0][3]*b[3][3];
	c[1][0] = m[1][0]*b[0][0] + m[1][1]*b[1][0] + m[1][2]*b[2][0] + m[1][3]*b[3][0];
	c[1][1] = m[1][0]*b[0][1] + m[1][1]*b[1][1] + m[1][2]*b[2][1] + m[1][3]*b[3][1];
	c[1][2] = m[1][0]*b[0][2] + m[1][1]*b[1][2] + m[1][2]*b[2][2] + m[1][3]*b[3][2];
	c[1][3] = m[1][0]*b[0][3] + m[1][1]*b[1][3] + m[1][2]*b[2][3] + m[1][3]*b[3][3];
	c[2][0] = m[2][0]*b[0][0] + m[2][1]*b[1][0] + m[2][2]*b[2][0] + m[2][3]*b[3][0];
	c[2][1] = m[2][0]*b[0][1] + m[2][1]*b[1][1] + m[2][2]*b[2][1] + m[2][3]*b[3][1];
	c[2][2] = m[2][0]*b[0][2] + m[2][1]*b[1][2] + m[2][2]*b[2][2] + m[2][3]*b[3][2];
	c[2][3] = m[2][0]*b[0][3] + m[2][1]*b[1][3] + m[2][2]*b[2][3] + m[2][3]*b[3][3];
	c[3][0] = m[3][0]*b[0][0] + m[3][1]*b[1][0] + m[3][2]*b[2][0] + m[3][3]*b[3][0];
	c[3][1] = m[3][0]*b[0][1] + m[3][1]*b[1][1] + m[3][2]*b[2][1] + m[3][3]*b[3][1];
	c[3][2] = m[3][0]*b[0][2] + m[3][1]*b[1][2] + m[3][2]*b[2][2] + m[3][3]*b[3][2];
	c[3][3] = m[3][0]*b[0][3] + m[3][1]*b[1][3] + m[3][2]*b[2][3] + m[3][3]*b[3][3];
	(*this) = c;
	return (*this);
}

// 4x4 * vec3
vec3_t mat4_t::operator *( vec3_t& b )
{
	vec3_t c;
	c.LoadZero();
	for( int i=0; i<3; i++ )
	{
		for( int j=0; j<3; j++ )
			c[i] += m[i][j] * b[j];
			
		c[i] += m[i][3];
	}
	return c;
}

// 4x4 * scalar
mat4_t mat4_t::operator *( float f )
{
	mat4_t c;
	for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
			c[i][j] = m[i][j] * f;
	return c;
}

// 4x4 * scalar (self)
mat4_t& mat4_t::operator *=( float f )
{
	for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
			m[i][j] = m[i][j] * f;
	return (*this);
}

// 3x3 to 4x4
void mat4_t::LoadMat3( mat3_t& m3 )
{
	for( int i=0; i<3; i++ )
		for( int j=0; j<3; j++ )
			m[i][j] = m3[i][j];
	m[3][0] = m[3][1] = m[3][2] = m[0][3] = m[1][3] = m[2][3] = 0.0;
	m[3][3] = 1.0;
}

// Transpose
void mat4_t::Transpose()
{
	for( int i=0; i<4; i++ )
		for( int j=0; j<4; j++ )
			if( i>j )
			{
				float f = m[i][j];
				m[i][j] = m[j][i];
				m[j][i] = f;
			}
}

// LoadIdent
void mat4_t::LoadIdent()
{
	memcpy( this, &identity_mat4[0], sizeof(mat4_t) );
}

// LoadZero
void mat4_t::LoadZero()
{
	memset( this, 0, sizeof(mat4_t) );
}

// LoadVec3
void mat4_t::LoadVec3( vec3_t& v )
{
	LoadIdent();
	m[0][3] = v.x();
	m[1][3] = v.y();
	m[2][3] = v.z();
}

// Print
void mat4_t::Print()
{
	cout << fixed;
	for( int i=0; i<4; i++ )
	{
		for( int j=0; j<4; j++ )
		{
			if( m[i][j] < 0.0 )
				cout << m[i][j] << " ";
			else
				cout << " " << m[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

// Determinant
float mat4_t::Determinant()
{
	return 
	m[0][3]*m[1][2]*m[2][1]*m[3][0] - m[0][2]*m[1][3]*m[2][1]*m[3][0] - 
	m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][1]*m[1][3]*m[2][2]*m[3][0] +
	m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][1]*m[1][2]*m[2][3]*m[3][0] - 
	m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][2]*m[1][3]*m[2][0]*m[3][1] +
	m[0][3]*m[1][0]*m[2][2]*m[3][1] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - 
	m[0][2]*m[1][0]*m[2][3]*m[3][1] + m[0][0]*m[1][2]*m[2][3]*m[3][1] +
	m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][1]*m[1][3]*m[2][0]*m[3][2] - 
	m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][0]*m[1][3]*m[2][1]*m[3][2] +
	m[0][1]*m[1][0]*m[2][3]*m[3][2] - m[0][0]*m[1][1]*m[2][3]*m[3][2] - 
	m[0][2]*m[1][1]*m[2][0]*m[3][3] + m[0][1]*m[1][2]*m[2][0]*m[3][3] +
	m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][0]*m[1][2]*m[2][1]*m[3][3] -
	m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][0]*m[1][1]*m[2][2]*m[3][3];
}

// Invert
void mat4_t::Invert()
{
	mat4_t m_;
	m_ = *this;
	m[0][0] = m_[1][2]*m_[2][3]*m_[3][1] - m_[1][3]*m_[2][2]*m_[3][1] + m_[1][3]*m_[2][1]*m_[3][2] - m_[1][1]*m_[2][3]*m_[3][2] - m_[1][2]*m_[2][1]*m_[3][3] + m_[1][1]*m_[2][2]*m_[3][3];
	m[0][1] = m_[0][3]*m_[2][2]*m_[3][1] - m_[0][2]*m_[2][3]*m_[3][1] - m_[0][3]*m_[2][1]*m_[3][2] + m_[0][1]*m_[2][3]*m_[3][2] + m_[0][2]*m_[2][1]*m_[3][3] - m_[0][1]*m_[2][2]*m_[3][3];
	m[0][2] = m_[0][2]*m_[1][3]*m_[3][1] - m_[0][3]*m_[1][2]*m_[3][1] + m_[0][3]*m_[1][1]*m_[3][2] - m_[0][1]*m_[1][3]*m_[3][2] - m_[0][2]*m_[1][1]*m_[3][3] + m_[0][1]*m_[1][2]*m_[3][3];
	m[0][3] = m_[0][3]*m_[1][2]*m_[2][1] - m_[0][2]*m_[1][3]*m_[2][1] - m_[0][3]*m_[1][1]*m_[2][2] + m_[0][1]*m_[1][3]*m_[2][2] + m_[0][2]*m_[1][1]*m_[2][3] - m_[0][1]*m_[1][2]*m_[2][3];
	m[1][0] = m_[1][3]*m_[2][2]*m_[3][0] - m_[1][2]*m_[2][3]*m_[3][0] - m_[1][3]*m_[2][0]*m_[3][2] + m_[1][0]*m_[2][3]*m_[3][2] + m_[1][2]*m_[2][0]*m_[3][3] - m_[1][0]*m_[2][2]*m_[3][3];
	m[1][1] = m_[0][2]*m_[2][3]*m_[3][0] - m_[0][3]*m_[2][2]*m_[3][0] + m_[0][3]*m_[2][0]*m_[3][2] - m_[0][0]*m_[2][3]*m_[3][2] - m_[0][2]*m_[2][0]*m_[3][3] + m_[0][0]*m_[2][2]*m_[3][3];
	m[1][2] = m_[0][3]*m_[1][2]*m_[3][0] - m_[0][2]*m_[1][3]*m_[3][0] - m_[0][3]*m_[1][0]*m_[3][2] + m_[0][0]*m_[1][3]*m_[3][2] + m_[0][2]*m_[1][0]*m_[3][3] - m_[0][0]*m_[1][2]*m_[3][3];
	m[1][3] = m_[0][2]*m_[1][3]*m_[2][0] - m_[0][3]*m_[1][2]*m_[2][0] + m_[0][3]*m_[1][0]*m_[2][2] - m_[0][0]*m_[1][3]*m_[2][2] - m_[0][2]*m_[1][0]*m_[2][3] + m_[0][0]*m_[1][2]*m_[2][3];
	m[2][0] = m_[1][1]*m_[2][3]*m_[3][0] - m_[1][3]*m_[2][1]*m_[3][0] + m_[1][3]*m_[2][0]*m_[3][1] - m_[1][0]*m_[2][3]*m_[3][1] - m_[1][1]*m_[2][0]*m_[3][3] + m_[1][0]*m_[2][1]*m_[3][3];
	m[2][1] = m_[0][3]*m_[2][1]*m_[3][0] - m_[0][1]*m_[2][3]*m_[3][0] - m_[0][3]*m_[2][0]*m_[3][1] + m_[0][0]*m_[2][3]*m_[3][1] + m_[0][1]*m_[2][0]*m_[3][3] - m_[0][0]*m_[2][1]*m_[3][3];
	m[2][2] = m_[0][1]*m_[1][3]*m_[3][0] - m_[0][3]*m_[1][1]*m_[3][0] + m_[0][3]*m_[1][0]*m_[3][1] - m_[0][0]*m_[1][3]*m_[3][1] - m_[0][1]*m_[1][0]*m_[3][3] + m_[0][0]*m_[1][1]*m_[3][3];
	m[2][3] = m_[0][3]*m_[1][1]*m_[2][0] - m_[0][1]*m_[1][3]*m_[2][0] - m_[0][3]*m_[1][0]*m_[2][1] + m_[0][0]*m_[1][3]*m_[2][1] + m_[0][1]*m_[1][0]*m_[2][3] - m_[0][0]*m_[1][1]*m_[2][3];
	m[3][0] = m_[1][2]*m_[2][1]*m_[3][0] - m_[1][1]*m_[2][2]*m_[3][0] - m_[1][2]*m_[2][0]*m_[3][1] + m_[1][0]*m_[2][2]*m_[3][1] + m_[1][1]*m_[2][0]*m_[3][2] - m_[1][0]*m_[2][1]*m_[3][2];
	m[3][1] = m_[0][1]*m_[2][2]*m_[3][0] - m_[0][2]*m_[2][1]*m_[3][0] + m_[0][2]*m_[2][0]*m_[3][1] - m_[0][0]*m_[2][2]*m_[3][1] - m_[0][1]*m_[2][0]*m_[3][2] + m_[0][0]*m_[2][1]*m_[3][2];
	m[3][2] = m_[0][2]*m_[1][1]*m_[3][0] - m_[0][1]*m_[1][2]*m_[3][0] - m_[0][2]*m_[1][0]*m_[3][1] + m_[0][0]*m_[1][2]*m_[3][1] + m_[0][1]*m_[1][0]*m_[3][2] - m_[0][0]*m_[1][1]*m_[3][2];
	m[3][3] = m_[0][1]*m_[1][2]*m_[2][0] - m_[0][2]*m_[1][1]*m_[2][0] + m_[0][2]*m_[1][0]*m_[2][1] - m_[0][0]*m_[1][2]*m_[2][1] - m_[0][1]*m_[1][0]*m_[2][2] + m_[0][0]*m_[1][1]*m_[2][2];
	(*this) *= 1.0f/m_.Determinant();
}

// Lerp
void mat4_t::Lerp( mat4_t& a, mat4_t& b, float t )
{
	(*this) = (a*(1.0f-t))+(b*t);
}