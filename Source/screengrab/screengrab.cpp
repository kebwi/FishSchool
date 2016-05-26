//-------------------------------------------------------------
/// \file	screengrab.cpp
/// \author	Rob Bateman
/// \date	9-feb-2005
/// \brief	some code to take a screen grab of an opengl window.
//-------------------------------------------------------------

#include "screengrab.h"
#include "tgasave.h"
#ifdef __MACH__	//Building on a Mac
	#include <GLUT/glut.h>
#else	//Presumably building on Linux
	#include <GL/glew.h>
	#include <GL/glext.h>
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#include <iostream>

using namespace std;

bool SaveScreenGrab(const char* filename, bool rle, int w, int h)
{
	// get some info about the screen size
	unsigned sw      =  glutGet(GLUT_WINDOW_WIDTH);
	unsigned sh      =  glutGet(GLUT_WINDOW_HEIGHT);
	unsigned bpp     =  3;//glutGet(GLUT_WINDOW_RGBA) ? 4 : 3;
	GLenum   format  =  (bpp==4) ? GL_RGBA : GL_RGB;
	
	int w1 = w;//sw;
	int h1 = h;//sh;
	/*
	cout << "Screen Grab:" << endl;
	cout << "   width: " << w1 << endl;
	cout << "   height: " << h1 << endl;
	cout << "   bpp: " << bpp << endl;
	cout << "   format: " << format << endl;
	*/
	// allocate memory to store image data
	unsigned char* pdata = new unsigned char[w1*h1*bpp];

	// read from front buffer
	glReadBuffer(GL_FRONT);

	// read pixel data
	glReadPixels(0,0,w1,h1,format,GL_UNSIGNED_BYTE,pdata);

	// write data as a tga file
	bool ret;
	if (!rle)
		ret = WriteTga(filename,w1,h1,bpp,pdata);
	else ret = WriteTgaRLE(filename,w1,h1,bpp,pdata);

	// clean up
	delete [] pdata;

	// done
	return ret;
}
