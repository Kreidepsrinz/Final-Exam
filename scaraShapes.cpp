#include "scarashapes.h"
#include <math.h>

point calcCentroid(point pts[], int nPts) {
	point centroid;
	centroid.x = 0; //set initial to 0
	centroid.y = 0; //set initial to 0

	for (int i = 0; i < nPts; i++) {
		centroid.x += pts[i].x; //sum all x coordinates
		centroid.y += pts[i].y; //sum all y coordinates
	}
	centroid.x /= nPts; //divide by num of points for avg
	centroid.y /= nPts; //divide by num of points for avg

	return centroid;
}

square initSquare(point pos, int sid, RGB_COLOR col) {
	square sq;
	sq.position = pos; //set based on input
	sq.sides = sid; //set based on input
	sq.color = col; //set based on input
	sq.rotation = PI / 4;
	return sq;
}

void drawSquare(SCARA_ROBOT* robot, square s) {
	point pts[4];

	for (int i = 0; i < 4; i++) {
		pts[i].x = s.position.x + (s.sides * cos(2 * PI / 4 * i + (s.rotation))); //find x position
		pts[i].y = s.position.y + (s.sides * sin(2 * PI / 4 * i + (s.rotation))); //find y position
	}
	LINE_DATA line[4];
	for (int i = 0; i < 4; i++) {
		line[i].xA = pts[i].x;
		line[i].yA = pts[i].y;
		if (i == 3) {
			line[i].xB = pts[0].x;
			line[i].yB = pts[0].y;
		}
		else {
			line[i].xB = pts[i + 1].x;
			line[i].yB = pts[i + 1].y;
		}
		line[i].numPts = 20;
		line[i].color = s.color;
		moveScaraL(robot, line[i]);
	}
}

hexagon initHexagon(point pst, int sds, RGB_COLOR clr) {
	hexagon hex;
	hex.position = pst; //set based on input
	hex.sides = sds; //set based on input
	hex.color = clr; //set based on input
	return hex;
}
 
void drawHexagon(SCARA_ROBOT* robot, hexagon h) {
	point pts[6];
	for (int i = 0; i < 6; i++) {
		pts[i].x = h.position.x + (h.sides * cos(2 * PI * i / 6));
		pts[i].y = h.position.y + (h.sides * sin(2 * PI * i / 6));
	}
	LINE_DATA line[6];
	for (int i = 0; i < 6; i++) {
		line[i].xA = pts[i].x; //set to initial point
		line[i].yA = pts[i].y; //set to initial point
		if (i == 5) {
			line[i].xB = pts[0].x; //when reaches the last point, return to 0
			line[i].yB = pts[0].y; //when reaches the last point, return to 0
		}
		else {
			line[i].xB = pts[i+1].x; //moves to the next point
			line[i].yB = pts[i+1].y; //moves to the next point
		}
		line[i].numPts = 20;
		line[i].color = h.color;
		moveScaraL(robot, line[i]);
	}
}

octogon initOctogon(point pst, double rad, RGB_COLOR col) {
	octogon oct;
	oct.position = pst;
	oct.radius = rad;
	oct.color = col;
	return oct;
}

void drawOctogon(SCARA_ROBOT* robot, octogon o) {
	point pts[8];
	for (int i = 0; i < 8; i++) {
		pts[i].x = o.position.x + (o.radius * cos(2 * PI * i / 8));
		pts[i].y = o.position.y + (o.radius * sin(2 * PI * i / 8));
	}
	LINE_DATA line[8];
	for (int i = 0; i < 8; i++) {
		line[i].xA = pts[i].x;
		line[i].yA = pts[i].y;
		if (i == 7) {
			line[i].xB = pts[0].x;
			line[i].yB = pts[0].y;
		}
		else {
			line[i].xB = pts[i+1].x;
			line[i].yB = pts[i+1].y;
		}
		line[i].numPts = 20;
		line[i].color = o.color;
		moveScaraL(robot, line[i]);
	}
}

polygon initPolygon(point pst, int poin, int sid, RGB_COLOR clr) {
	polygon pol;
	pol.position = pst;
	pol.points = poin;
	pol.sides = sid;
	pol.color = clr;
	return pol;
}

void drawPolygon(SCARA_ROBOT* robot, polygon p) {
	point pts[MAX_POINTS];
	for (int i = 0; i < p.points; i++) {
		pts[i].x = p.position.x + (p.sides * cos(2 * PI * i / p.points));
		pts[i].y = p.position.y + (p.sides * sin(2 * PI * i / p.points));
	}
	LINE_DATA line[MAX_POINTS];
	for (int i = 0; i < p.points; i++) {
		line[i].xA = pts[i].x;
		line[i].yA = pts[i].y;
		if (i == p.points - 1) {
			line[i].xB = pts[0].x;
			line[i].yB = pts[0].y;
		}
		else {
			line[i].xB = pts[i+1].x;
			line[i].yB = pts[i+1].y;
		}
		line[i].numPts = 20;
		line[i].color = p.color;
		moveScaraL(robot, line[i]);
	}
}

void shapeSizes(SCARA_ROBOT* robot, point position, double scale) {
	RGB_COLOR blue = { 0, 0, 225 };
	double sides = 100;
	for (int i = 0; i < 5; i++) {
		position.x -= 100;
		polygon shapes = initPolygon(position, i + 3, sides - (20 * i), blue);
		drawPolygon(robot, shapes);
	}
}