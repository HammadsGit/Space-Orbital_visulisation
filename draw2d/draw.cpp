#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

typedef int OutCode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000



OutCode ComputeOutCode(float x, float y, float x_min, float y_min, float x_max, float y_max) {
	OutCode code = INSIDE;

	if (x < x_min) code |= LEFT;
	else if (x > x_max) code |= RIGHT;

	if (y < y_min) code |= BOTTOM;
	else if (y > y_max) code |= TOP;

	return code;
}

bool CohenSutherlandLineClip(float &x0, float &y0, float&x1, float&y1, float x_min, float y_min, float x_max, float y_max) {
	OutCode outcode0 = ComputeOutCode(x1, y1, x_min, y_min, x_max, y_max);
	OutCode outcode1 = ComputeOutCode(x1, y1, x_min, y_min, x_max, y_max);
	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) {
			break;
		}
		else {
			double x, y;
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			if (outcodeOut & TOP) {
				x = x0 + (x1 - x0) * (y_max - y0) / (y1 - y0);
				y = y_max;
			}
			else if (outcodeOut & BOTTOM) {
				x = x0 + (x1 - x0) * (y_min - y0) / (y1 - y0);
				y = y_min;
			}
			else if (outcodeOut & RIGHT) {
				y = y0 + (y1 - y0) * (x_max - x0) / (x1 - x0);
				x = x_max;
			}
			else {
				y = y0 + (y1 - y0) * (x_min - x0) / (x1 - x0);
				x = x_min;
			}

			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(x1, y1, x_min, y_min, x_max, y_max);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(x1, y1, x_min, y_min, x_max, y_max);
			}
		}
	}
	return accept;
}



void draw_line_solid(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor)
{

	float x_min = 0, y_min = 0, x_max = aSurface.get_width(), y_max = aSurface.get_height();

	if (CohenSutherlandLineClip(aBegin.x, aBegin.y, aEnd.x, aEnd.y, x_min, y_min, x_max, y_max)) {
		// converts to integers the beginning and end point coordinates. 
		int x1 = static_cast<int>(aBegin.x);
		int y1 = static_cast<int>(aBegin.y);
		int x2 = static_cast<int>(aEnd.x);
		int y2 = static_cast<int>(aEnd.y);


		// Calcualte the difference between x and y coordinates.
		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);

		// Calculate the direction of the line.
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;

		// Calculate the error.
		int err = dx - dy;


		// Draw the line.
		while (true)


		{

			//print the pixel with sout



			// apply clipping algorithm

			aSurface.set_pixel_srgb(x1, y1, aColor);

			if (x1 == x2 && y1 == y2) break;
			int e2 = 2 * err;
			if (e2 > -dy)
			{
				err -= dy;
				x1 += sx;
			}
			if (e2 < dx)
			{
				err += dx;
				y1 += sy;
			}
		}
	}

}





void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}


void draw_triangle_solid(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor) {
	// Step 1: Sort vertices by y-coordinate
	if (aP1.y < aP0.y) {
		Vec2f temp = aP0;
		aP0 = aP1;
		aP1 = temp;
	}
	if (aP2.y < aP0.y) {
		Vec2f temp = aP0;
		aP0 = aP2;
		aP2 = temp;
	}
	if (aP2.y < aP1.y) {
		Vec2f temp = aP1;
		aP1 = aP2;
		aP2 = temp;
	}

	// Check for horizontal edges
	if (aP1.y == aP0.y) { // Horizontal top edge
		// Handle this case here if needed
	}
	if (aP2.y == aP1.y) { // Horizontal bottom edge
		// Handle this case here if needed
	}

	// Compute slopes for each edge
	float m0 = (aP1.y - aP0.y != 0) ? (aP1.x - aP0.x) / (aP1.y - aP0.y) : 0;
	float m1 = (aP2.y - aP1.y != 0) ? (aP2.x - aP1.x) / (aP2.y - aP1.y) : 0;
	float m2 = (aP2.y - aP0.y != 0) ? (aP2.x - aP0.x) / (aP2.y - aP0.y) : 0;

	// Fill top part of triangle
	for (float y = aP0.y; y <= aP1.y; y++) {
		float startX = aP0.x + m0 * (y - aP0.y);
		float endX = aP0.x + m2 * (y - aP0.y);
		if (startX > endX) std::swap(startX, endX);  // Ensure startX is always to the left of endX
		for (float x = startX; x <= endX; x++) {
			if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
				aSurface.set_pixel_srgb(x, y, aColor);
			}
		}
	}

	// Fill bottom part of triangle
	for (float y = aP1.y; y <= aP2.y; y++) {
		float startX = aP1.x + m1 * (y - aP1.y);
		float endX = aP0.x + m2 * (y - aP0.y);
		if (startX > endX) std::swap(startX, endX);  // Ensure startX is always to the left of endX
		for (float x = startX; x <= endX; x++) {
			if (x >= 0 && x < aSurface.get_width() && y >= 0 && y < aSurface.get_height()) {
				aSurface.set_pixel_srgb(x, y, aColor);
			}
		}
	}
}


void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: Remove the following when you start your implementation!
	//TODO: This draws a solid triangle until you implement the function
	//TODO: properly. (This is such that you can see the asteroids with solid
	//TODO: shading until that point.)
	draw_triangle_solid( aSurface, aP0, aP1, aP2, linear_to_srgb( aC0 ) );
	(void)aC1;
	(void)aC2;
}


void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
