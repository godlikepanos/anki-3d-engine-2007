#ifndef _MATH_H_
#define _MATH_H_


#define PI 3.14159265358979323846f

class quat_t;
class mat4_t;
class mat3_t;
class vec3_t;
class euler_t;


// vector 2
class vec2_t
{
	public:
		float v[2];
		// std
		inline float& operator [] (const int i)  { return v[i]; }
		inline float& x()  {return v[0];}
		inline float& y()  {return v[1];}
		// ops with same type
		vec2_t& operator = ( vec2_t& b ); // copy
		vec2_t  operator + ( vec2_t& b ); // add
		vec2_t& operator +=( vec2_t& b );	// add
		vec2_t  operator - ( vec2_t& b ); // sub
		vec2_t& operator -=( vec2_t& b ); // sub
		// other types
		vec2_t  operator * ( float f );   // scale
		vec2_t& operator *=( float f );   // scale
		vec2_t  operator / ( float f );   // scale
		vec2_t& operator /=( float f );   // scale
		// other
		float Length  ();
		void  LoadZero();
};


// vector 3
// also used as column matrix
class vec3_t
{
	public:
		float v[3];
		// std
		inline float& operator [] (const int i)  { return v[i]; }
		inline float& x()  {return v[0];}
		inline float& y()  {return v[1];}
		inline float& z()  {return v[2];}
		vec3_t() {}
		vec3_t( float x_, float y_, float z_ ) { x()=x_; y()=y_; z()=z_; }
		vec3_t( vec3_t& v3_ ) { (*this)=v3_; }
		// ops with same type
		vec3_t& operator = ( vec3_t& b ); // copy
		vec3_t  operator + ( vec3_t& b ); // add
		vec3_t& operator +=( vec3_t& b );	// add
		vec3_t  operator - ( vec3_t& b ); // sub
		vec3_t& operator -=( vec3_t& b ); // sub
		vec3_t  operator * ( vec3_t& b ); // cross prod
		vec3_t& operator *=( vec3_t& b ); // cross prod
		// other types
		vec3_t  operator * ( float f );   // scale
		vec3_t& operator *=( float f );   // scale
		vec3_t  operator / ( float f );   // down-scale
		vec3_t& operator /=( float f );   // down-scale
		// other
		float  DotProd   ( vec3_t& b );   
		vec3_t Project   ( vec3_t& b );  // project this to b and return the result to this
		float  Length    ();
		void   Normalize ();
		vec3_t VecsMiddle( vec3_t& b );
		void   LoadZero  ();
		void   LoadQuat  ( quat_t& q );
		void   Lerp      ( vec3_t v0, vec3_t v1, float t );
		void   Print     ();
};


// vector 4
class vec4_t
{
	public:
		float v[4];
		// std
		inline float& operator [] (const int i)  { return v[i]; }
};


// quaternion
class quat_t
{
	public:
		float q[4];
		// std
		inline float& operator [] (const int i)  { return q[i]; }
		inline float& w() { return q[0]; }
		inline float& x() { return q[1]; }
		inline float& y() { return q[2]; }
		inline float& z() { return q[3]; }
		quat_t() {};
		quat_t( float w_, float x_, float y_, float z_ ) { w()=w_; x()=x_; y()=y_; z()=z_; }
		quat_t( quat_t& q_ ) {(*this)=q_;}
		// ops with same
		quat_t& operator = ( quat_t& b ); // cpy
		quat_t  operator + ( quat_t& b ); // quat + quat
		quat_t& operator +=( quat_t& b ); // quat + quat (self)
		quat_t  operator - ( quat_t& b ); // quat - quat
		quat_t& operator -=( quat_t& b ); // quat - quat (self)
		quat_t  operator * ( quat_t& b ); // quat * quat
		quat_t& operator *=( quat_t& b ); // quat * quat (self)
		// ops with other
		quat_t  operator * ( float f );   // quat * float
		quat_t& operator *=( float f );   // quat * float (self)
		// other
		void  LoadMat3         ( mat3_t& m );
		void  LoadVec3         ( vec3_t& v );
		void  LoadEuler        ( euler_t& e );
		void  LoadIdent        ();
		void  LoadZero         ();
		void  Conj             ();
		void  Normalize        ();
		void  Print            ();
		float DotProd          ( quat_t& b );
		void  Log              ();
		void  Exp              ();
		void  Power            ( float f );
		void  Slerp            ( quat_t& q1, quat_t& q2, float t );
};


// euler angles in degrees
class euler_t
{
	public:
		// std
		float v[3];
		float& bank()     { return v[0]; }
		float& heading()  { return v[1]; }
		float& attitude() { return v[2]; }
		// axis
		float& x() { return bank(); }
		float& y() { return heading(); }
		float& z() { return attitude(); }
		euler_t() {}
		euler_t( float x_, float y_, float z_  ) { bank()=x_; heading()=y_; attitude()=z_; }
		euler_t( euler_t& e_ ) { (*this)=e_; }
		// ops with same
		euler_t& operator = ( euler_t& b ); // cpy
		// other
		void LoadQuat( quat_t& q );
		void LoadMat3( mat3_t& m3 );
		void Print();
};


// matrix 3x3
class mat3_t
{
	public:
		vec3_t m[3];
		// std
		inline vec3_t& operator [] (const int i)  { return m[i]; };
		mat3_t() {};
		mat3_t( mat3_t& m3_ ) { (*this)=m3_; };
		// ops with mat3
		mat3_t& operator = ( mat3_t& b ); // cpy
		mat3_t  operator + ( mat3_t& b ); // 3x3 + 3x3
		mat3_t& operator +=( mat3_t& b ); // 3x3 + 3x3 (self)
		mat3_t  operator - ( mat3_t& b ); // 3x3 - 3x3
		mat3_t& operator -=( mat3_t& b ); // 3x3 - 3x3 (self)
		mat3_t  operator * ( mat3_t& b ); // 3x3 * 3x3
		mat3_t& operator *=( mat3_t& b ); // 3x3 * 3x3 (self)
		// ops with others
		vec3_t  operator * ( vec3_t& b ); // 3x3 * vec3
		mat3_t  operator * ( float f );
		mat3_t& operator *=( float f );
		// other
		void  LoadQuat   ( quat_t& q );
		void  LoadEuler  ( euler_t& e );
		void  LoadVec3   ( vec3_t& v );
		void  Transpose  ();
		void  Ortho      ();
		void  LoadIdent  ();
		void  LoadZero   ();
		void  Print      ();
		float Determinant();
		void  Invert     ();
};


// matrix 4x4
class mat4_t
{
	public:
		vec4_t m[4];
		// std
		inline vec4_t& operator [] (const int i)  { return m[i]; };
		mat4_t() {};
		mat4_t( mat4_t& m4_ ) { (*this)=m4_; };
		// with same
		mat4_t& operator = ( mat4_t& b ); // copy
		mat4_t  operator + ( mat4_t& b ); // 4x4 + 4x4
		mat4_t& operator +=( mat4_t& b ); // 4x4 + 4x4 (self)
		mat4_t  operator - ( mat4_t& b ); // 4x4 - 4x4
		mat4_t& operator -=( mat4_t& b ); // 4x4 - 4x4 (self)
		mat4_t  operator * ( mat4_t& b ); // 4x4 * 4x4
		mat4_t& operator *=( mat4_t& b ); // 4x4 * 4x4 (self)
		// with other types
		vec3_t  operator * ( vec3_t& v ); // 4x4 * vec3
		mat4_t  operator * ( float f );   // scale
		mat4_t& operator *=( float f );   // scale (self)
		// other
		void  LoadMat3   ( mat3_t& m3 );
		void  LoadVec3   ( vec3_t& v );
		void  Transpose  ();
		void  LoadIdent  ();
		void  LoadZero   ();
		void  Print      ();
		float Determinant();
		void  Invert     ();
		void  Lerp       ( mat4_t& a, mat4_t& b, float t );
};


#endif