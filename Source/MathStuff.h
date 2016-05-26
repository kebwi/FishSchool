#ifndef __MATH_STUFF__
#define __MATH_STUFF__

#include <math.h>
#ifdef __MACH__	//Building on a Mac
	#include <GLUT/glut.h>
#else	//Presumably building on Linux
	#include <GL/glew.h>
	#include <GL/glext.h>
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif

static const float Pi = 3.14159265359;
static const float PiTimes2 = Pi * 2.0;
static const float PiOver2 = Pi / 2.0;
static const float PiOver4 = Pi / 4.0;

static float DegToRad(float a)
{
	return ( (a * Pi) / 180.0 );
}

static float RadToDeg(float a)
{
	return ( (a * 180.0) / Pi );
}

static void Truncate(float low, float &value, float high)
{
	if (value < low)
		value = low;
	else if (value > high)
		value = high;
}

template<class T>
void Truncate(T low, T &value, T high)
{
	if (value < low)
		value = low;
	else if (value > high)
		value = high;
}

static void Constrain(float low, float &value, float high)
{
	while (value < low)
		value += (high - low);
	while (value > high)
		value -= (high - low);
}

static inline void vecAdd(GLfloat v1[3], GLfloat v2[3])
{
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
}

static inline void vecCross(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3])
{
	v3[0] = v1[1] * v2[2] - v2[1] * v1[2];
	v3[1] = v1[2] * v2[0] - v2[2] * v1[0];
	v3[2] = v1[0] * v2[1] - v2[0] * v1[1];
}

static inline GLfloat vecDot(GLfloat v1[3], GLfloat v2[3])
{
	return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

static inline void vecScale(GLfloat v[3], GLfloat scalar)
{
	v[0] *= scalar;
	v[1] *= scalar;
	v[2] *= scalar;
}

static inline GLfloat vecLengthSquared(GLfloat v[3])
{
	return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

static inline GLfloat vecLength(GLfloat v[3])
{
	return sqrt(vecLengthSquared(v));
}

//return true for success, false for failure
static inline bool vecNormalize(GLfloat v[3])
{
	GLfloat len = vecLength(v);
	if (len == 0.0f)
		return false;
	
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
	
	return true;
}

static inline void vecSetLength(GLfloat v[3], GLfloat vecLength = 1.0)
{
	if (vecNormalize(v))
		vecScale(v, vecLength);
}

static inline void vecAvg(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3])
{
	GLfloat v1Len = vecLength(v1);
	GLfloat v2Len = vecLength(v2);
	GLfloat totalLen = v1Len + v2Len;
	
	v3[0] = (v1[0] * v1Len + v2[0] * v2Len) / totalLen;
	v3[1] = (v1[1] * v1Len + v2[1] * v2Len) / totalLen;
	v3[2] = (v1[2] * v1Len + v2[2] * v2Len) / totalLen;
}

static void makeRotMatrix(GLfloat axis[3], GLfloat angleRot, GLfloat R[16])
{
	GLfloat c = cos(angleRot);
	GLfloat s = sin(angleRot);
	GLfloat t = 1.0 - c;
	
	R[0] = t * axis[0] * axis[0] + c;
	R[1] = t * axis[0] * axis[1] + s * axis[2];
	R[2] = t * axis[0] * axis[2] - s * axis[1];
	R[4] = t * axis[0] * axis[1] - s * axis[2];
	R[5] = t * axis[1] * axis[1] + c;
	R[6] = t * axis[1] * axis[2] + s * axis[0];
	R[8] = t * axis[0] * axis[2] + s * axis[1];
	R[9] = t * axis[1] * axis[2] + s * axis[0];
	R[10] = t * axis[2] * axis[2] + c;
	
	R[15] = 1.0;
}

static void multVecByRotMatrix(GLfloat v1[3], GLfloat R[16], GLfloat v2[3])
{
	v2[0] = v1[0] * R[0] + v1[1] * R[4] + v1[2] * R[8];
	v2[1] = v1[0] * R[1] + v1[1] * R[5] + v1[2] * R[9];
	v2[2] = v1[0] * R[2] + v1[1] * R[6] + v1[2] * R[10];
}

#endif
