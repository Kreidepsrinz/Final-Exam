#ifndef SCARA_SHAPES_H_
#define SCARA_SHAPES_H_

/*|Includes|-------------------------------------------------------------------*/
#include "robot.h"  // NOTE: DO NOT REMOVE.
#include "scara.h"
#include "scarashapes.h"

#define MAX_POINTS 20

struct point {
	double x;
	double y;
};

struct square {
	point position;
	int sides;
	RGB_COLOR color;
};

struct hexagon {
	point position;
	int sides;
	RGB_COLOR color;
};

struct polygon {
	point position;
	int points;
	int sides;
	RGB_COLOR color;
};

struct octogon {
	point position;
	double radius;
	RGB_COLOR color;
};

point calcCentroid(point pts[], int nPts);
square initSquare(point position, int sides, RGB_COLOR color);
void drawSquare(SCARA_ROBOT* robt, square sq);
hexagon initHexagon(point pnt, int sds, RGB_COLOR clr);
void drawHexagon(SCARA_ROBOT* robot, hexagon hexa);
octogon initOctogon(point pnt, int rad, RGB_COLOR clr);
void drawOctogon(SCARA_ROBOT* robot, octogon octo);
polygon initPolygon(point pnt, int pts, int sds, RGB_COLOR clr);
void drawPolygon(SCARA_ROBOT* robot, polygon poly);
void shapesSizes(SCARA_ROBOT robot, point position, int scale);

#endif /* SCARA_SHAPES_H_ */