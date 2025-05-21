/*|SCARA Simulator|------------------------------------------------------------
#
# Project: ROBT 1270 - Final Exam
# Program: scara_main.c
#
# Description:
#   
#
# Other Programs:
#   ScaraRobotSimulator.exe (Version 4.3)
#
# Simulator Commands:
#  - PEN_UP
#  - PEN_DOWN
#  - PEN_COLOR <r> <g> <b>
#  - CYCLE_PEN_COLORS ON/OFF
#  - ROTATE_JOINT ANG1 <deg1> ANG2 <deg2>
#  - CLEAR_TRACE
#  - CLEAR_REMOTE_COMMAND_LOG
#  - CLEAR_POSITION_LOG
#  - SHUTDOWN_SIMULATION
#  - MOTOR_SPEED HIGH/MEDIUM/LOW
#  - MESSAGE <"string">
#  - HOME
#  - END
#
# Other Information:
#  - IP Address: 127.0.0.1 Port 1270
#
# Author: <Name>
# Date Created: <Date>
# Last Modified: <Today>
# -----------------------------------------------------------------------------*/
#pragma warning(disable:4996)  // get rid of some microsoft-specific warnings.

/*|Includes|-------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "scaraShapes.h"

/*|Enumerations|---------------------------------------------------------------*/

/*|Global Variables|-----------------------------------------------------------*/
extern CRobot robot;

/*|Structures|-----------------------------------------------------------------*/

/*|Function Declarations|------------------------------------------------------*/
void moveScaraLTest(void);

int main(){
	// Variables
	SCARA_ROBOT sRobot;

	// Initialize SCARA Simulator V3
	if (!robot.Initialize()) exit(0);
	robot.Send("PEN_UP\n");
	robot.Send("HOME\n");
	robot.Send("CLEAR_TRACE\n");

    // Initialize Scara Robot
	sRobot = initScaraState(0, 600, 0, { 'd', {0, 0, 0} }, 'h');
	scaraSetState(sRobot);

	moveScaraLTest();

    //robot.Send("END\n");
    printf("\n\nPress ENTER to end the program...\n");
    getchar();
   
    // Close Remote Connection
    robot.Close(); 
    return 0;
}

void moveScaraLTest(void) {
	LINE_DATA lineData;
	RGB_COLOR black = { 0, 0, 0 };
	RGB_COLOR blue = { 0, 0, 225 };
	RGB_COLOR red = { 225, 0, 0 };
	RGB_COLOR green = { 0, 225, 0 };

	// Initialized Simulator from this Point
	SCARA_ROBOT robot = initScaraState(300, 300, RIGHT_ARM_SOLUTION, { 'd', {0, 0, 255} }, 'H');
	int sides = 150;
	
	square s = initSquare({300,300}, sides, black);
	drawSquare(&robot, s);

	hexagon h = initHexagon({-300,300}, sides, red);
	drawHexagon(&robot, h);
	
	octogon o = initOctogon({-300,-300}, sides, green);
	drawOctogon(&robot, o);
	
	shapeSizes(&robot, {400,-300}, sides);
}