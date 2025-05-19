#include "scaraShapes.h"
#include <math.h>

//PART A NUMBER 1
point calcCentroid(point pts[], int nPts) {
	point centroid;
	centroid.x = 0; //sets initial value of x to 0
	centroid.y = 0; //sets initial value of y to 0

	for (int i = 0; i < nPts; i++) {
		centroid.x += pts[i].x; //sum all the x coordinates    
		centroid.y += pts[i].y; //sum all the y coordinates
	}

	centroid.x /= nPts; //divide all the x coordinates by the number of points to get the average
	centroid.y /= nPts; //divide all the y coordinates by the number of points to get the average
	return centroid;
}

//PART A NUMBER 2
square initSquare(point pos, int sid, RGB_COLOR col) {
	square s;
	s.position = pos; //setting the position based on user's input
	s.sides = sid; //setting the number of sides based on user's input
	s.color = col; //setting the color based on user's input
	return s;
}

//PART A NUMBER 3
void drawSquare(SCARA_ROBOT* robt, square sq) {
	LINE_DATA lineA;
	lineA.xA = sq.position.x;
	lineA.yA = sq.position.y;
	lineA.xB = sq.position.x;
	lineA.yB = sq.position.y + sq.sides;
	lineA.numPts = 20;
	lineA.color = sq.color;
	moveScaraL(robt, lineA);

	LINE_DATA lineB;
	lineB.xA = sq.position.x;
	lineB.yA = sq.position.y + sq.sides;
	lineB.xB = sq.position.x + sq.sides;
	lineB.yB = sq.position.y + sq.sides;
	lineB.numPts = 20;
	lineB.color = sq.color;
	moveScaraL(robt, lineB);

	LINE_DATA lineC;
	lineC.xA = sq.position.x + sq.sides;
	lineC.yA = sq.position.y + sq.sides;
	lineC.xB = sq.position.x + sq.sides;
	lineC.yB = sq.position.y;
	lineC.numPts = 30;
	lineC.color = sq.color;
	moveScaraL(robt, lineC);

	LINE_DATA lineD;
	lineD.xA = sq.position.x + sq.sides;
	lineD.yA = sq.position.y;
	lineD.xB = sq.position.x;
	lineD.yB = sq.position.y;
	lineD.numPts = 20;
	lineD.color = sq.color;
	moveScaraL(robt, lineD);
}

//PART B (2) NUMBER 6
hexagon initHexagon(point pnt, int sds, RGB_COLOR clr) {
	hexagon hex;
	hex.position = pnt; //assigning to the inputs
	hex.sides = sds; //assigning to the inputs
	hex.color = clr; //assigning to the inputs

	return hex;
}

//PART B (2) NUMBER 7
void drawHexagon(SCARA_ROBOT* robot, hexagon hexa) {
	point pts[6];
	for (int i = 0; i < 6; i++) {
		pts[i].x = hexa.position.x + (hexa.sides * cos(2 * PI * i/6));
		pts[i].y = hexa.position.y + (hexa.sides * sin(2 * PI * i/6));
	}
	LINE_DATA lines[6];
	for (int i = 0; i < 6; i++) {
		lines[i].xA = pts[i].x;
		lines[i].yA = pts[i].y;
		if (i == 5) {
			lines[i].xB = pts[0].x;
			lines[i].yB = pts[0].y;
		}
		else {
			lines[i].xB = pts[i+1].x;
			lines[i].yB = pts[i+1].y;
		}
		
		lines[i].numPts = 20;
		lines[i].color = hexa.color;

		moveScaraL(robot, lines[i]);
	}
}

//PART B (2) NUMBER 8
octogon initOctogon(point pnt, int rad, RGB_COLOR clr) {
	octogon oct;
	oct.position = pnt; //assigning to the inputs
	oct.radius = rad; //assigning to the inputs
	oct.color = clr; //assigning to the inputs

	return oct;
}

//PART B (2) NUMBER 9
void drawOctogon(SCARA_ROBOT* robot, octogon octo) {
	point pts[8];
	for (int i = 0; i < 8; i++) {
		pts[i].x = octo.position.x + (octo.radius * cos(2 * PI * i / 8));
		pts[i].y = octo.position.y + (octo.radius * sin(2 * PI * i / 8));
	}
	LINE_DATA lines[8];
	for (int i = 0; i < 8; i++) {
		lines[i].xA = pts[i].x;
		lines[i].yA = pts[i].y;
		if (i == 7) {
			lines[i].xB = pts[0].x;
			lines[i].yB = pts[0].y;
		}
		else {
			lines[i].xB = pts[i + 1].x;
			lines[i].yB = pts[i + 1].y;
		}

		lines[i].numPts = 20;
		lines[i].color = octo.color;

		moveScaraL(robot, lines[i]);
	}
}

//PART B (2) NUMBER 10
polygon initPolygon(point pnt, int pts, int sds, RGB_COLOR clr) {
	polygon pol;
	pol.position = pnt; //assigning to the inputs
	pol.points = pts; //assigning to the inputs
	pol.sides = sds; //assigning to the inputs
	pol.color = clr; //assigning to the inputs

	return pol;
}

void drawPolygon(SCARA_ROBOT* robot, polygon poly) {
	point pts[MAX_POINTS];
	for (int i = 0; i < poly.points; i++) {
		pts[i].x = poly.position.x + (poly.sides * cos(2 * PI * i / poly.points + (PI / 2)));
		pts[i].y = poly.position.y + (poly.sides * sin(2 * PI * i / poly.points + (PI / 2)));
	}
	LINE_DATA lines[MAX_POINTS];
	for (int i = 0; i < poly.points; i++) {
		lines[i].xA = pts[i].x;
		lines[i].yA = pts[i].y;
		if (i == poly.points-1) {
			lines[i].xB = pts[0].x;
			lines[i].yB = pts[0].y;
		}
		else {
			lines[i].xB = pts[i + 1].x;
			lines[i].yB = pts[i + 1].y;
		}

		lines[i].numPts = 20;
		lines[i].color = poly.color;

		moveScaraL(robot, lines[i]);
	}
}

void shapesSizes(SCARA_ROBOT robot, point position, int scale) {
	RGB_COLOR blue = { 0, 0, 225 };
	int sides = 200;
	for (int i = 0; i < 5; i++) {
		position.x -= 80;
		polygon poli = initPolygon(position, i+3, sides-(30*i), blue);
		drawPolygon(&robot, poli);
	}
}