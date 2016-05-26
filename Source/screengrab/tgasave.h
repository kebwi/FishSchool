//-------------------------------------------------------------
/// \file	tgasave.h
/// \date	9-feb-2005
/// \author	Rob Bateman
/// \brief	writes a 24 or 32 bit tga file to the disk.
//-------------------------------------------------------------

#ifndef TGA_SAVE__H__
#define TGA_SAVE__H__

bool WriteTga(const char* filename,
				const unsigned w,
				const unsigned h,
				const unsigned bpp,
				const unsigned char* pixels);

bool WriteTgaRLE(const char* filename,
				const unsigned w,
				const unsigned h,
				const unsigned bpp,
				const unsigned char* pixels);

#endif
