#ifndef SCARA_SHAPES_H_
#define SCARA_SHAPES_H_
#define MAX_POINTS 20

/*|Includes|-------------------------------------------------------------------*/
#include "robot.h"  // NOTE: DO NOT REMOVE.
#include "scara.h"
#include "scarashapes.h"

struct point {
	double x;
	double y;
};

struct square {
	point position;
	int sides;
	RGB_COLOR color;
	double rotation;
};

struct hexagon {
	point position;
	int sides;
	RGB_COLOR color;
};

struct octogon {
	point position;
	double radius;
	RGB_COLOR color;
};

struct polygon {
	point position;
	int points;
	int sides;
	RGB_COLOR color;
};

point calcCentroid(point pts[], int nPts);
square initSquare(point pos, int sid, RGB_COLOR col);
void drawSquare(SCARA_ROBOT* robot, square s);
hexagon initHexagon(point pst, int sds, RGB_COLOR clr);
void drawHexagon(SCARA_ROBOT* robot, hexagon h);
octogon initOctogon(point pst, double rad, RGB_COLOR col);
void drawOctogon(SCARA_ROBOT* robot, octogon o);
polygon initPolygon(point pst, int poin, int sid, RGB_COLOR clr);
void drawPolygon(SCARA_ROBOT* robot, polygon p);
void shapeSizes(SCARA_ROBOT* robot, point position, double scale);

#endif /* SCARA_SHAPES_H_ */