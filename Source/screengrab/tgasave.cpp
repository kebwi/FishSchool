//-------------------------------------------------------------
/// \file	tgasave.cpp
/// \date	9-feb-2005
/// \author	Rob Bateman
/// \brief	writes a 24 or 32 bit tga file to the disk.
//-------------------------------------------------------------

#include "tgasave.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

static bool sgDebug = false;

/// the tga header
#pragma pack(push,1)
struct TGAHeader
{
	// sometimes the tga file has a field with some custom info in. This 
	// just identifies the size of that field. If it is anything other
	// than zero, forget it.
	unsigned char m_iIdentificationFieldSize;
	
	// This field specifies if a colour map is present, 0-no, 1 yes...
	unsigned char m_iColourMapType;
	
	// only going to support RGB/RGBA/8bit - 2, colour mapped - 1
	unsigned char m_iImageTypeCode;
	
	// ignore this field....0
	unsigned short m_iColorMapOrigin;
	
	// size of the colour map
	unsigned short m_iColorMapLength;
	
	// bits per pixel of the colour map entries...
	unsigned char m_iColourMapEntrySize;
	
	// ignore this field..... 0
	unsigned short m_iX_Origin;
	
	// ignore this field..... 0
	unsigned short m_iY_Origin;
	
	// the image width....
	unsigned short m_iWidth;
	
	// the image height.... 
	unsigned short m_iHeight;
	
	// the bits per pixel of the image, 8,16,24 or 32
	unsigned char m_iBPP;
	
	// ignore this field.... 0
	unsigned char m_ImageDescriptorByte;
};
#pragma pack(pop)

bool WriteTga(const char* filename,
				const unsigned w,
				const unsigned h,
				const unsigned bpp,
				const unsigned char* pixels)
{
	// a flag to see if writing 32 bit image
	bool rgba=false;

	// make sure format is valid (allows bits or bytes per pixel)
	switch(bpp) {
	case 4: case 32:
		rgba = true;
		break;

	case 3: case 24:
		rgba = false;
		break;

	default:
		cerr << "[ERROR] Unsupported bit depth requested" << endl;
		return false;
	}
	
	cout << endl;
	cout << "Writing Tga:" << endl;
	cout << "   filename: " << filename << endl;
	cout << "   width: " << w << endl;
	cout << "   height: " << h << endl;
	cout << "   rgba: " << (rgba ? "true" : "false") << endl;
	
	FILE* fp = fopen(filename,"wb");

	if(!fp) {
		cerr << "[ERROR] could not save file \"" 
				  << filename << "\"" << endl;
		return false;
	}

	// fill the file header with correct info....
	TGAHeader header;

	// wipe to 0
	memset(&header,0,sizeof(TGAHeader));

	// rgb or rgba image
	header.m_iImageTypeCode = 2;

	// set image size
	//The code I downloaded does it this way.  However, it needs to be little endian...
	//header.m_iWidth = w;
	//header.m_iHeight = h;
	
	//...so I rewrote it this way:
	char* c;
	c = (char*)&header.m_iWidth;
	*c = (char)(w & 255);
	c++;
	*c = (char)(w >> 8);
	c = (char*)&header.m_iHeight;
	*c = (char)(h & 255);
	c++;
	*c = (char)(h >> 8);

	// set bits per pixel
	header.m_iBPP = rgba ? 32 : 24;

	cout << "Tga Header:" << endl;
	cout << "   m_iIdentificationFieldSize: " << (int)header.m_iIdentificationFieldSize << endl;
	cout << "   m_iColourMapType: " << (int)header.m_iColourMapType << endl;
	cout << "   m_iImageTypeCode: " << (int)header.m_iImageTypeCode << endl;
	cout << "   m_iColorMapOrigin: " << header.m_iColorMapOrigin << endl;
	cout << "   m_iColorMapLength: " << header.m_iColorMapLength << endl;
	cout << "   m_iColourMapEntrySize: " << (int)header.m_iColourMapEntrySize << endl;
	cout << "   m_iX_Origin: " << header.m_iX_Origin << endl;
	cout << "   m_iY_Origin: " << header.m_iY_Origin << endl;
	cout << "   m_iWidth: " << header.m_iWidth << endl;
	cout << "   m_iHeight: " << header.m_iHeight << endl;
	cout << "   m_iBPP: " << (int)header.m_iBPP << endl;
	cout << "   m_ImageDescriptorByte: " << (int)header.m_ImageDescriptorByte << endl;
	
	// write header as first 18 bytes of output file
	fwrite(&header,sizeof(TGAHeader),1,fp);

	// get num pixels
	unsigned int total_size = w * h * (3 + (rgba?1:0));
	unsigned int this_pixel_start = 0;

	// loop through each pixel
	for( ; this_pixel_start != total_size; this_pixel_start += 3 ) {

		// get address of pixel data
		const unsigned char* pixel = pixels + this_pixel_start;

		// write as BGR
		fputc(pixel[2], fp);
		fputc(pixel[1], fp);
		fputc(pixel[0], fp);

		// if RGBA also write alpha value
		if (rgba) {
			fputc(pixel[3],fp);
			++this_pixel_start;
		}
	}

	fclose(fp);
	
	///////////////////////////////////////////
	FILE* fp1 = fopen("./ScreenCaptures/FlockScreenCapture.raw","wb");
	fwrite(pixels,total_size,1,fp);
	fclose(fp1);

	return true;  
}

void Assert(bool condition, const char* msg)
{
	if (!condition)
	{
		cout << endl << msg << endl;
		exit(1);
	}
}

void CalcRLElength(int w, bool rgba, int bpp, const unsigned char* pixel, int& column, int& runLength)
{
	Assert(column < w, "column < w at beginning of CalcRLElength");
	
	runLength = 1;
	unsigned char comparePixel3, comparePixel2, comparePixel1, comparePixel0, nextPixel3, nextPixel2, nextPixel1, nextPixel0;
	
	if (++column == w)
	{
		column = 0;
		return;
	}
	
	if (rgba)
		comparePixel3 = pixel[3];
	comparePixel2 = pixel[2];
	comparePixel1 = pixel[1];
	comparePixel0 = pixel[0];
	
	while (true)
	{
		Assert(column < w, "column < w at beginning of CalcRLElength while loop");
		
		if (rgba)
			nextPixel3 = pixel[3 + runLength * bpp];
		nextPixel2 = pixel[2 + runLength * bpp];
		nextPixel1 = pixel[1 + runLength * bpp];
		nextPixel0 = pixel[runLength * bpp];
		
		runLength++;
		column++;
		
		if ((!rgba && (nextPixel2 != comparePixel2 || nextPixel1 != comparePixel1 || nextPixel0 != comparePixel0)) ||
			(rgba && (nextPixel3 != comparePixel3 || nextPixel2 != comparePixel2 || nextPixel1 != comparePixel1 || nextPixel0 != comparePixel0)))
		{
			runLength--;
			column--;
			break;	//The RLE run is over because the next pixel doesn't match
		}
		
		if (column == w)
		{
			if (sgDebug)
				cout << "   edge " << runLength << flush;
			column = 0;
			break;	//Runs don't wrap image rows
		}
		
		if (runLength == 128)
		{
			if (sgDebug)
				cout << "   128 RLE" << flush;
			/*
			//Check if the next pixel matches the RLE and back up one pixel if so
			if (column + 1 < w)
			{
				if (sgDebug)
					cout << "   potential 129" << flush;
				if (rgba)
					nextPixel3 = pixel[3 + 128 * bpp];
				nextPixel2 = pixel[2 + 128 * bpp];
				nextPixel1 = pixel[1 + 128 * bpp];
				nextPixel0 = pixel[128 * bpp];
				
				if ((!rgba && nextPixel2 == comparePixel2 && nextPixel1 == comparePixel1 && nextPixel0 == comparePixel0) ||
					(rgba && nextPixel3 == comparePixel3 && nextPixel2 == comparePixel2 && nextPixel1 == comparePixel1 && nextPixel0 == comparePixel0))
				{
					if (sgDebug)
						cout << "   129 found" << flush;
					if (column + 2 < w)
					{
						if (sgDebug)
							cout << "   potential 130" << flush;
						if (rgba)
							nextPixel3 = pixel[3 + 129 * bpp];
						nextPixel2 = pixel[2 + 129 * bpp];
						nextPixel1 = pixel[1 + 129 * bpp];
						nextPixel0 = pixel[129 * bpp];
						
						if ((!rgba && (nextPixel2 != comparePixel2 || nextPixel1 != comparePixel1 || nextPixel0 != comparePixel0)) ||
							(rgba && (nextPixel3 != comparePixel3 || nextPixel2 != comparePixel2 || nextPixel1 != comparePixel1 || nextPixel0 != comparePixel0)))
						{
							if (sgDebug)
								cout << "   130 not found, backing up" << flush;
							runLength--;
							column--;
						}
					}
				}
			}
			*/
			break;	//Runs max out at 128 pixels long
		}
	}
}

void CalcRawLength(int w, bool rgba, int bpp, const unsigned char* pixel, int& column, int& runLength)
{
	Assert(column < w, "column < w at beginning of CalcRawLength");
	
	runLength = 1;
	unsigned char comparePixel3, comparePixel2, comparePixel1, comparePixel0, nextPixel3, nextPixel2, nextPixel1, nextPixel0;
	
	if (++column == w)
	{
		column = 0;
		return;
	}
	
	if (rgba)
		comparePixel3 = pixel[3];
	comparePixel2 = pixel[2];
	comparePixel1 = pixel[1];
	comparePixel0 = pixel[0];
	
	while (true)
	{
		Assert(column < w, "column < w at beginning of CalcRLElength while loop");
		
		if (rgba)
			nextPixel3 = pixel[3 + runLength * bpp];
		nextPixel2 = pixel[2 + runLength * bpp];
		nextPixel1 = pixel[1 + runLength * bpp];
		nextPixel0 = pixel[runLength * bpp];
		
		runLength++;
		column++;
		
		if ((!rgba && nextPixel2 == comparePixel2 && nextPixel1 == comparePixel1 && nextPixel0 == comparePixel0) ||
			(rgba && nextPixel3 == comparePixel3 && nextPixel2 == comparePixel2 && nextPixel1 == comparePixel1 && nextPixel0 == comparePixel0))
		{
			runLength -= 2;
			column -= 2;
			break;	//Detected the next RLE, which ends this raw packet
		}
		
		if (column == w)
		{
			if (sgDebug)
				cout << "   edge " << runLength << flush;
			column = 0;
			break;	//Runs don't wrap image rows
		}
		
		if (runLength == 128)
		{
			if (sgDebug)
				cout << "   128 raw" << flush;
			/*
			//Check if the next pixel matches the last pixel and the next next pixel doesn't match the last pixel, and back up one pixel if so
			if (column + 1 < w)
			{
				if (sgDebug)
					cout << "   potential raw to RLE" << flush;
				if (rgba)
					comparePixel3 = nextPixel3;
				comparePixel2 = nextPixel2;
				comparePixel1 = nextPixel1;
				comparePixel0 = nextPixel0;
				
				if (rgba)
					nextPixel3 = pixel[3 + 128 * bpp];
				nextPixel2 = pixel[2 + 128 * bpp];
				nextPixel1 = pixel[1 + 128 * bpp];
				nextPixel0 = pixel[128 * bpp];
				
				if ((!rgba && nextPixel2 == comparePixel2 && nextPixel1 == comparePixel1 && nextPixel0 == comparePixel0) ||
					(rgba && nextPixel3 == comparePixel3 && nextPixel2 == comparePixel2 && nextPixel1 == comparePixel1 && nextPixel0 != comparePixel0))
				{
					if (sgDebug)
						cout << "   raw to RLE found, backing up" << flush;
					runLength--;
					column--;
				}
			}
			*/
			break;	//Runs max out at 128 pixels long
		}
		
		if (rgba)
			comparePixel3 = nextPixel3;
		comparePixel2 = nextPixel2;
		comparePixel1 = nextPixel1;
		comparePixel0 = nextPixel0;
	}
}

bool WriteTgaRLE(const char* filename,
				const unsigned w,
				const unsigned h,
				const unsigned bpp,
				const unsigned char* pixels)
{
	// a flag to see if writing 32 bit image
	bool rgba=false;

	// make sure format is valid (allows bits or bytes per pixel)
	switch(bpp) {
	case 4: case 32:
		rgba = true;
		break;

	case 3: case 24:
		rgba = false;
		break;

	default:
		cerr << "[ERROR] Unsupported bit depth requested" << endl;
		return false;
	}
	
	cout << endl << "Writing RLE Tga of dimension " << w << "x" << h << " with rgba of "
		<< (rgba ? "true" : "false") << " to filename " << filename << "." << endl;
	
	FILE* fp = fopen(filename,"wb");

	if(!fp) {
		cerr << "[ERROR] could not save file \"" 
				  << filename << "\"" << endl;
		return false;
	}

	// fill the file header with correct info....
	TGAHeader header;

	// wipe to 0
	memset(&header,0,sizeof(TGAHeader));

	// rgb or rgba image
	header.m_iImageTypeCode = 10;

	// set image size
	//The code I downloaded does it this way.  However, it needs to be little endian...
	//header.m_iWidth = w;
	//header.m_iHeight = h;
	
	//...so I rewrote it this way:
	char* c;
	c = (char*)&header.m_iWidth;
	*c = (char)(w & 255);
	c++;
	*c = (char)(w >> 8);
	c = (char*)&header.m_iHeight;
	*c = (char)(h & 255);
	c++;
	*c = (char)(h >> 8);

	// set bits per pixel
	header.m_iBPP = rgba ? 32 : 24;
	
	if (sgDebug)
	{
		cout << "Tga RLE Header:" << endl;
		cout << "   m_iIdentificationFieldSize: " << (int)header.m_iIdentificationFieldSize << endl;
		cout << "   m_iColourMapType: " << (int)header.m_iColourMapType << endl;
		cout << "   m_iImageTypeCode: " << (int)header.m_iImageTypeCode << endl;
		cout << "   m_iColorMapOrigin: " << header.m_iColorMapOrigin << endl;
		cout << "   m_iColorMapLength: " << header.m_iColorMapLength << endl;
		cout << "   m_iColourMapEntrySize: " << (int)header.m_iColourMapEntrySize << endl;
		cout << "   m_iX_Origin: " << header.m_iX_Origin << endl;
		cout << "   m_iY_Origin: " << header.m_iY_Origin << endl;
		cout << "   m_iWidth: " << header.m_iWidth << endl;
		cout << "   m_iHeight: " << header.m_iHeight << endl;
		cout << "   m_iBPP: " << (int)header.m_iBPP << endl;
		cout << "   m_ImageDescriptorByte: " << (int)header.m_ImageDescriptorByte << endl << endl;
	}
	
	// write header as first 18 bytes of output file
	fwrite(&header,sizeof(TGAHeader),1,fp);

	// get num pixels
	unsigned int total_size = w * h * (3 + (rgba?1:0));
	unsigned int this_pixel_start = 0;
	
	int column = 0;
	int numRLEpackets = 0, numRawPackets = 0;
	int RLEaccumBytes = 0, rawAccumBytes = 0;
	while (this_pixel_start < total_size)
	{
		if (sgDebug)
			cout << "New run at col: " << column << flush;
		
		//Get the address of the next pixel
		const unsigned char* pixel = pixels + this_pixel_start;
		
		//Determine whether to RLE or raw encode this packet, and how long it should be if it is RLE encoded
		int runLength = 0, columnNew = column;
		
		bool forceRawPacket = false;
		//if (w - column <= 2)
		//	forceRawPacket = true;
		
		if (!forceRawPacket)
		{
			CalcRLElength(w, rgba, bpp, pixel, columnNew, runLength);
			if (sgDebug)
				cout << "   RLE result " << runLength << flush;
		}
		
		if (!forceRawPacket && runLength >= 2)	//Run length encode this packet
		{
			if (sgDebug)
				cout << "   RLE run:   length: " << runLength << flush;
			
			//Write the repetition count field, one pixel value for the entire RLE packet
			unsigned char repetitionCountField = (1 << 7) | (unsigned char)(runLength - 1);
			fputc(repetitionCountField, fp);
			
			//Write the pixel value field
			fputc(pixel[2], fp);
			fputc(pixel[1], fp);
			fputc(pixel[0], fp);
			if (rgba)
				fputc(pixel[3], fp);
			
			numRLEpackets++;
			RLEaccumBytes += 1 + (rgba ? 4 : 3);
		}
		else	//Raw encode this packet
		{
			columnNew = column;
			if (sgDebug)
				cout << "   raw run: col: " << column << flush;
			
			CalcRawLength(w, rgba, bpp, pixel, columnNew, runLength);
			
			if (sgDebug)
				cout << "   length: " << runLength << flush;
			
			Assert(runLength >= 1, "runLength >= 1 after CalcRawLength");
			
			//Write the repetition count field
			unsigned char repetitionCountField = (unsigned char)(runLength - 1);
			fputc(repetitionCountField, fp);
			
			//Write the pixel value field, one pixel for every raw pixel encoded
			for (int i = 0; i < runLength * bpp; i += bpp)
			{
				fputc(pixel[2 + i], fp);
				fputc(pixel[1 + i], fp);
				fputc(pixel[i], fp);
				if (rgba)
					fputc(pixel[3 + i], fp);
			}
			
			numRawPackets++;
			rawAccumBytes += 1 + runLength * (rgba ? 4 : 3);
		}
		
		column = columnNew % w;
		this_pixel_start += runLength * bpp;
		
		if (sgDebug)
			cout << "   next run at col: " << column << endl;
	}
	
	cout << "Tga done: "
		<< numRLEpackets << " RLE packets of " << RLEaccumBytes << " bytes and "
		<< numRawPackets << " raw packets of " << rawAccumBytes << " bytes, for a total filesize of "
		<< sizeof(TGAHeader) + RLEaccumBytes + rawAccumBytes << " bytes." << endl;
	
	fclose(fp);

	return true;
}
