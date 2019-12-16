#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <cstdint>
 

using Byte = uint8_t; // byte is unsigned 8 bits

class Pixel {
public:
	Pixel();
	Pixel(Byte red, Byte green, Byte blue);
	
	Byte getRed();
	Byte getGreen();
	Byte getBlue();
private:
	uint32_t value; // leftmost 8 bytes hold red, then green, then blue, then 8 rightmost hold "alpha" unused
};

class Bitmap {

public:
	Bitmap(int width, int height);
	~Bitmap();
	
	Bitmap(const Bitmap&) = delete; // copy constructor
	Bitmap& operator = (const Bitmap&) = delete; // copy assignment
	
	
	bool load(const std::string& fileName); // open bmp file and load data
	bool save(const std::string& fileName); // save data in bmp file
	void destroy();	// explicit deallocation of heap 
		
	void  setPixel(int x, int y, Pixel p); // bitmap rows: 0 is bottom, up to top is height-1
										   // columns: 0 is leftmost, width-1 is rightmost
									       // set column, row, pixel
	Pixel getPixel(int x, int y);		   // get column, row
	
	int getHeight() { return height; }
	int getWidth()  { return width;  }
	
private:
	int width, height;
	Pixel* data;	// Don't need capabilities of vector class, and this is simpler to write to binary file
};



#endif