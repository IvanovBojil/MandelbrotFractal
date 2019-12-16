#include "bitmap.h"

#include <iostream>

Pixel coordinateToPixel(double x, double y, int maxIter) {
	double x0 = x;
	double y0 = y;
	x = 0.0;
	y = 0.0;
	int currentIter = 0;
	while (x * x + y * y <= 2*2 && currentIter < maxIter) {
		double xTemp = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = xTemp;
		++currentIter;
	}
	
	Byte grayScale = 255 * currentIter / maxIter;
	return Pixel{grayScale, 0, 0};
}

int main() {
	
	int width, height, maxIter;
	double realStart, realEnd, compStart, compEnd;
	
	std::cout << "Width Height MaxIter RealStart RealEnd CompStart CompEnd\n";
	
	try {
		std::cin >> width >> height >> maxIter;
		if (std::cin.fail()) throw "Input Error";
		std::cin >> realStart >> realEnd >> compStart >> compEnd;
		if (std::cin.fail()) throw "Input Error";
	}
	catch (const char* messg) {
		std::cout << messg;
		return 1;
	}
	
	Bitmap bmp{width, height};
	
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			double x = realStart + ((double)col / (double)width) * (realEnd - realStart);
			double y = compStart + ((double)row / (double)height) * (compEnd - compStart);
			bmp.setPixel(col, row, coordinateToPixel(x, y, maxIter));
		}
	}
	
	bmp.save("Mandelbrot.bmp");
	
	return 0;
}