#include "bitmap.h"

#include <string>
#include <fstream>
#include <array>
#include <cstring> // for memset


Pixel::Pixel() 
	: value{0}
		{
		}
	
Pixel::Pixel(Byte red, Byte green, Byte blue) {
	value = 0;
	value |= red;
	value <<= 8;
	value |= green;
	value <<= 8;
	value |= blue;
	value <<= 8;
}
	
Byte Pixel::getRed() {
	return (value >> 24) & 0XFF;
}

Byte Pixel::getGreen() {
	return (value >> 16) & 0XFF;
}

Byte Pixel::getBlue() {
	return (value >> 8) & 0XFF;
}



Bitmap::Bitmap(int w, int h)
	: width{w}, height{h}, data{new Pixel[w * h]}
		{
		}
		
Bitmap::~Bitmap() {
	delete[] data;
	data = nullptr;
}

void  Bitmap::setPixel(int x, int y, Pixel p) {
	data[y * width + x] = p;
}

Pixel Bitmap::getPixel(int x, int y) {
	return data[y * width + x];
}

// bitmap header struct

#pragma pack(push, 1)
struct BitmapHeader {
	uint16_t signature; 	// 0X4D42
	uint32_t fileSize;   	// file size in bytes, total file 
	uint32_t reserved;   	// unused, 0
	uint32_t dataOffset; 	// Offset from beginning of file to beginning of bitmap data
	
	// 14 bytes total
	
	uint32_t infoSize; 		// size of InfoHeader, which is 40 bytes
	uint32_t width; 		// Horizontal width of bitmap in pixels
	uint32_t height; 		// vertical height of bitmap in pixels
	uint16_t planes; 		// number of planes = 1
	uint16_t bitsPerPixel; 	// here will be 24 bits per pixel, which stores 3 bytes per pixel RGB
	uint32_t compression; 	// no compression = 0
	uint32_t imageSize;		// file size after compression. if Compression is 0, this can be set to 0
							// as it will be same as fileSize - 54 (sizeof Header + sizeof InfoHeader)
	uint32_t xResolution;
	uint32_t yResolution; 	// Pixels/meter. default to 1000
	uint32_t colorsUsed; 	// number of colors used. Don't have to set
	uint32_t importantColors; // 0 = all
	
	// 54 bytes total
};
#pragma pack(pop)

void Bitmap::destroy() {
	delete[] data;
	data = nullptr;
}
	
bool Bitmap::load(const std::string& fileName) {
	BitmapHeader header;
	
	std::ifstream bitmapFile;
	bitmapFile.open(fileName, std::ios::binary | std::ios::in);
	
	bitmapFile.read((char*)&header, sizeof(header));
	width  = header.width;
	height = header.height;
	delete[] data;
	data = new Pixel[width * height];
	
	// need to read and discard padding
	int padLength = (width * 3) % 4 == 0 ? 0 : 4 - ((width * 3) % 4);
	
	for (int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			Byte blue, green, red;
			bitmapFile.read((char*)&blue, sizeof(Byte));
			bitmapFile.read((char*)&green, sizeof(Byte));
			bitmapFile.read((char*)&red, sizeof(Byte));
			setPixel(x, y, Pixel{red, green, blue});
		}
		Byte junk;
		bitmapFile.read((char*)&junk, padLength);
	}
	
	
	bitmapFile.close();
	return true;
}


bool Bitmap::save(const std::string& fileName) {
	BitmapHeader header;
	std::memset(&header, 0, sizeof(header)); // memset so entire struct is initially 0
	// fill in fields
	header.signature 	= 0X4D42;
	header.fileSize 	= sizeof(header) + (width * height) * 3; // Each pixel will be converted to 24 bit value, or 3 byte value
	header.dataOffset 	= sizeof(header);
	header.infoSize 	= 40;
	header.width 		= width;
	header.height 		= height;
	header.planes 		= 1;
	header.bitsPerPixel = 24;
	header.compression 	= 0;
	header.imageSize 	= width * height * 3;
	header.xResolution 	= header.yResolution = 1000;
	
	// pad length. Width of bitmap must be written to file as multiple of 4
	// Note that the width is really width * 3, as we will write each pixel as Blue, Green, Red byte (bitmap format)
	// So our file width is width * 3
	int padLength = (width * 3) % 4 == 0 ? 0 : 4 - ((width * 3) % 4);
	//open binary file to write output
	std::ofstream bitmapFile;
	bitmapFile.open(fileName, std::ios::binary | std::ios::out);
	
	bitmapFile.write((char*)&header, sizeof(header));
	
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Pixel p = getPixel(x, y);
			Byte blue = p.getBlue(), green = p.getGreen(), red = p.getRed();
			bitmapFile.write((char*)&blue, sizeof(Byte));
			bitmapFile.write((char*)&green, sizeof(Byte));
			bitmapFile.write((char*)&red, sizeof(Byte));
		}
		// padlength
		Byte zero = 0;
		bitmapFile.write((char*)&zero, padLength);
	}
	
	bitmapFile.close();
	return true;
}
		








