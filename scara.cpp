/*|ROBT 1270: SCARA |----------------------------------------------------------
#
# Project: ROBT 1270 - SCARA Simulator Intermediate Control
# Program: scara.h
#
# Description:
#   This program contains code for drawing lines with the SCARA.
#
# Author: Isaiah Regacho
# Date Created: May 5, 2025
# Last Modified: May 5, 2025
# -----------------------------------------------------------------------------*/

/*|Includes|-------------------------------------------------------------------*/
#include "scara.h"

/*|Global Variables|-----------------------------------------------------------*/
CRobot robot;     // the global robot Class instance.  Can be used everywhere
                  // robot.Initialize()
                  // robot.Send()
                  // robot.Close()

/*|Function Definitions|-------------------------------------------------------*/
/****************************************************************************************
* Function: moveScaraJ
*
* Description:
*	This function will perform joint interpolated moves.
*
* Inputs:
*	scaraState  - Contains SCARA information
*
* Returns:
*	check		- Report if requested move was invalid
*
* Last Modified: April 30, 2021 by Isaiah Regacho
*****************************************************************************************/
int moveScaraJ(SCARA_ROBOT *scaraState){

	// Variable Declarations
	SCARA_POS arm = scaraState->armPos;
	int check;

	// Calculate the X,Y Coordinates
	check = scaraIK(arm.x, arm.y, &arm.theta1, &arm.theta2, arm.armSol);
	scaraState->armPos = arm;

	// Check if the Solution was valid
	if (check == 0) {
		// Move the Robot
		scaraSetState(*scaraState);
	}

	return check;
}

/****************************************************************************************
* Function: moveScaraL
*
* Description:
*	This function will perform a linear interpolated move.
*
* Inputs:
*	scaraState  - Contains SCARA information
*
* Returns:
*	check		- Report if requested move was invalid
*
* Last Modified: May 1, 2021 by Isaiah Regacho
*****************************************************************************************/
int moveScaraL(SCARA_ROBOT *scaraState, LINE_DATA line){
	int check = 0;
	double x, y;

	// Allocate Mememory for Move
	SCARA_ROBOT *robline = (SCARA_ROBOT*) malloc(sizeof(SCARA_ROBOT)*line.numPts);
	SCARA_TOOL tool = {'u', line.color};
	SCARA_POS arm;

	char speed = 'H';

	if (scaraState->armPos.x == line.xA && scaraState->armPos.y == line.yA ){
		tool = {'d', line.color};
		speed = 'L';
	}

	robline[0] = initScaraState(line.xA, line.yA, scaraState->armPos.armSol, tool, speed);

	// Switch Solution if the first point is invalid.
	if (moveScaraJ(&robline[0])) {
		robline[0].armPos.armSol =!(robline[0].armPos.armSol);
		moveScaraJ(&robline[0]);
	}
	*scaraState = robline[0];

	// Calculate Intermediate Points
	for (int i = 0; i < line.numPts && check == 0; i++) {
		x = i*(line.xB - line.xA)/(line.numPts -  1) + line.xA;
		y = i*(line.yB - line.yA)/(line.numPts - 1) + line.yA;

		robline[i] = *scaraState;
		robline[i].armPos.x = x;
		robline[i].armPos.y = y;

		robline[i].toolPos = tool;
		robline[i].motorSpeed = speed;

		// If the move fails...
		if (moveScaraJ(&robline[i])) {
			// Check if the other solution is valid.
			robline[i].armPos.armSol =!(robline[i].armPos.armSol);
			arm = robline[i].armPos;
			check = scaraIK(arm.x, arm.y, &arm.theta1, &arm.theta2, arm.armSol);
			if (check){
				check = 0;
				tool = {'u', line.color};
				speed = 'H';
				continue;
			}

			// Pick up the Pen and Switch Solution
			tool.penPos = robline[i-1].toolPos.penPos = 'u';
			speed = robline[i-1].motorSpeed = 'H';
			robline[i-1].armPos.armSol =!(robline[i-1].armPos.armSol);
			
			// If this move fails, the line cannot be drawn.
			if(!moveScaraJ(&robline[i-1])){
				i = i - 1; // Repeat the Original Move To Keep Solution
				tool = {'d', line.color};
				speed = 'L';
			}
		} else {
			tool = {'d', line.color};
			speed = 'L';
		}

		*scaraState = robline[i];
		//scaraState->toolPos.penPos = 'd';
		//scaraState->motorSpeed = 'L';
	}

	free(robline);
	return check;
}

/****************************************************************************************
* Function: initLine
*
* Description:
*	This function creates a LINE_DATA object.
*
* Inputs:
*	xA		- Starting x-coordinate
*	yA		- Starting y-coordinate
*	xB		- End x-coordinate
*	yB		- End y-coordinate
*	numPts	- Number of points to make on the line.
*
* Returns:
*	line		- Contains line information
*
* Last Modified: May 1, 2021 by Isaiah Regacho
*****************************************************************************************/
LINE_DATA initLine(double xA, double yA, double xB, double yB, int numPts, RGB_COLOR color) {
	// Variables
	LINE_DATA line;

	line.xA = xA;
	line.yA = yA;
	line.xB = xB;
	line.yB = yB;
	line.numPts = numPts;

	line.color.r = color.r;
	line.color.g = color.g;
	line.color.b = color.b;

	/*if (fabs(xB - xA) < SLOPE_TOL) {
		line.color.r = 0;
		line.color.g = 0;
		line.color.b = 0;
	} else if (fabs(yB - yA) < SLOPE_TOL) {
		line.color.g = 255;
	} else if ((yB - yA) / (xB - xA) > 0) {
		line.color.b = 255;
	} else if ((yB - yA) / (xB - xA) < 0) {
		line.color.r = 255;
	}*/
	
	return line;
}

/****************************************************************************************
* Function: initScaraState
*
* Description:
*	This function will assign the provided values to a SCARA_ROBOT structure.
*
* Inputs:
*	x			- X-Coordinate of the SCARA
*	y			- Y-Coordinate of the SCARA
*	armSol		- Left or Right Arm Solution for IK
*	penState	- Color and Position of the pen
*	mtrSpeed	- SCARA Speed Setting 
*
* Returns:
*	robot		- Contains all provided information
*
* Last Modified: April 30, 2021 by Isaiah Regacho
*****************************************************************************************/
SCARA_ROBOT initScaraState(double x, double y, int armSol, SCARA_TOOL penState, char mtrSpeed) {
	
	// Variables
	SCARA_ROBOT robot;

	// Assign Values to SCARA ROBOT
	robot.armPos.x = x;
	robot.armPos.y = y;
	robot.armPos.armSol = armSol;
	robot.toolPos = penState;
	robot.motorSpeed = mtrSpeed;

	return robot;
}

/****************************************************************************************
* Function: scaraSetState
*
* Description:
*	Moves the robot.
*
* Inputs:
*	scaraState  - Contains SCARA information
*
* Returns: void
*
* Last Modified: April 30, 2021 by Isaiah Regacho
*****************************************************************************************/
void scaraSetState(SCARA_ROBOT scaraState) {
	// Previous State
	static SCARA_ROBOT prev = initScaraState(600, 0, 0, {'u', {255, 0, 0}}, 'H');
	RGB_COLOR curColor, prevColor;
	curColor = scaraState.toolPos.penColor;
	prevColor = prev.toolPos.penColor;
	
	// Change Tool/Pen Color
	if (curColor.r != prevColor.r || curColor.g != prevColor.g || curColor.b != prevColor.b) {
		robot.Send("PEN_UP\n");
		robot.Send("MOTOR_SPEED HIGH\n");
		setScaraAngles(0, 0);
		setScaraColor(curColor.r, curColor.g, curColor.b);
		if (scaraState.toolPos.penPos == 'd') {
			setScaraAngles(prev.armPos.theta1, prev.armPos.theta2);
		} else {
			prev.armPos.theta1 = 0;
			prev.armPos.theta2 = 0;
		}
	}

	// Set Pen Position
	if (scaraState.toolPos.penPos != prev.toolPos.penPos) {
		setScaraPen(scaraState.toolPos.penPos);
	}

	// Set Scara Speed
	if (scaraState.motorSpeed != prev.motorSpeed) {
		setScaraSpeed(scaraState.motorSpeed);
	}

	// Rotate the Arm
	if (scaraState.armPos.theta1 != prev.armPos.theta1 || scaraState.armPos.theta2 != prev.armPos.theta2) {
		setScaraAngles(scaraState.armPos.theta1, scaraState.armPos.theta2);
	}
	prev = scaraState;
}

/****************************************************************************************
* Function: scaraDisplayState
*
* Description:
*	Displays the SCARA information. Feel free to modify.
*
* Inputs:
*	scaraState  - Contains SCARA information
*
* Returns: void
*
* Last Modified: May 1, 2021 by Isaiah Regacho
*****************************************************************************************/
void scaraDisplayState(SCARA_ROBOT scaraState) {
	SCARA_POS arm = scaraState.armPos;
	SCARA_TOOL tool = scaraState.toolPos;

	printf("|SCARA STATE|\n");

	// Display Position
	printf("| Theta 1 | Theta 2 |    X    |    Y    |   Arm   |\n");
	printf("|%9.2lf|%9.2lf|%9.2lf|%9.2lf|    %d    |\n", arm.theta1, arm.theta2, arm.x, arm.y, arm.armSol);

	// Display Tool
	printf("|Position |   RED   |  GREEN  |   BLUE  |\n");
	printf("|    %c    |   %3d   |   %3d   |   %3d   |\n", tool.penPos, tool.penColor.r, tool.penColor.g, tool.penColor.b);
}

/****************************************************************************************
* Function: scaraFK
*
* Description:
*	This function will calculate the x,y coordinates given two joint angles.
*
* Inputs:
*	ang1	- Angle of joint 1 in degrees.
*	ang2	- Angle of joint 2 in degrees.
*	toolX	- The tool position along the x-axis. Pointer
*	toolY	- The tool position along the y-axis. Pointer
*
* Returns:
*	inRange	- (0) in range, (-1) out of range
*
* Last Modified: April 23, 2021 by Isaiah Regacho
*****************************************************************************************/
int scaraFK(double ang1, double ang2, double* toolX, double* toolY) {
	// Default in range
	int inRange = 0;
	
	// Check if angles are valid
	if (fabs(ang1) <= MAX_ABS_THETA1_DEG && fabs(ang2) <= MAX_ABS_THETA2_DEG) {
		// Calculate the X, Y positions
		*toolX = L1*cos((ang1)*PI/180.0) + L2*cos((ang2 + ang1)*PI/180.0);
		*toolY = L1*sin((ang1)*PI/180.0) + L2*sin((ang2 + ang1)*PI/180.0);
	} else {
		// Not in range
		inRange = -1;
	}

	return inRange;
}


/****************************************************************************************
* Function: scaraIK
*
* Description:
*	This function will calculate two joint angles given the x,y coordinates given.
*
* Inputs:
*	toolX	- The tool position along the x-axis.
*	toolY	- The tool position along the y-axis.
*	ang1	- Angle of joint 1 in degrees. Pointer
*	ang2	- Angle of joint 2 in degrees. Pointer
*	arm		- Selects which solution to try.
*
* Returns:
*	inRange	- (0) in range, (-1) out of range
*
* Last Modified: May 1, 2021 by Isaiah Regacho
*****************************************************************************************/
int scaraIK(double toolX, double toolY, double* ang1, double* ang2, int arm) {
	// Variables
	int inRange = 0;
	double r, beta, alpha;

	// Polar Coordinates
	r  = sqrt(toolX*toolX + toolY*toolY);
	beta = atan2(toolY, toolX);

	// Check if Within Range of Fixed Lengths
	if (fabs(r) < L1 - L2 || fabs(r) > L1 + L2) {
		inRange = -1;
	}

	// Cosine Law
	alpha = acos((L2*L2 - r*r - L1*L1)/(-2*r*L1));
	
	// Theta 1 and 2
	*ang1 = beta + (arm ? -alpha : alpha);
	*ang2 = atan2(toolY - L1*sin(*ang1), toolX - L1*cos(*ang1)) - *ang1;

	*ang1 = (*ang1)*180.0/PI;
	*ang2 = (*ang2)*180.0/PI;

	// Limit Rotation
	if (fabs(*ang1) > 180.0) *ang1 -= (*ang1 > 0) ? 360.0 : -360.0;
	if (fabs(*ang2 )> 180.0) *ang2 -= (*ang2 > 0) ? 360.0 : -360.0;

	// Check if angles are valid
	if (fabs(*ang1) > MAX_ABS_THETA1_DEG || fabs(*ang2) > MAX_ABS_THETA2_DEG) {
		inRange = -1;
	}

	return inRange;
}

/****************************************************************************************
* Function: setScaraAngles
*
* Description:
*	This function will rotate the SCARA joints.
*
* Inputs:
*	ang1	- Angle of joint 1 in degrees.
*	ang2	- Angle of joint 2 in degrees.
*
* Returns: void
*
* Last Modified: April 23, 2021 by Isaiah Regacho
*****************************************************************************************/
void setScaraAngles(double ang1, double ang2) {
	char cmd[MAX_STRING];
	sprintf(cmd,"ROTATE_JOINT ANG1 %lf ANG2 %lf\n", ang1, ang2); 
	robot.Send(cmd);
}

/****************************************************************************************
* Function: setScaraSpeed
*
* Description:
*	This function sets the Speed of the SCARA motors.
*
* Inputs:
*	speed - H/M/L
*
* Returns: void
*
* Last Modified: April 30, 2021 by Isaiah Regacho
*****************************************************************************************/
void setScaraSpeed(char speed) {
	// Send one of 3 speeds
	switch(speed) {
		case 'H':
			robot.Send("MOTOR_SPEED HIGH\n");
			break;
		case 'M':
			robot.Send("MOTOR_SPEED MEDIUM\n");
			break;
		case 'L':
		default:
			robot.Send("MOTOR_SPEED LOW\n");	
	}
}

/****************************************************************************************
* Function: setScaraPen
*
* Description:
*	This function sets the pen of the SCARA.
*
* Inputs:
*	pen - 'd' for pen down, otherwise lift pen.
*
* Returns: void
*
* Last Modified: April 30, 2021 by Isaiah Regacho
*****************************************************************************************/
void setScaraPen(char pen) {
	// Default Pen UP
	(pen == 'd') ? robot.Send("PEN_DOWN\n") : robot.Send("PEN_UP\n");
}

/****************************************************************************************
* Function: setScaraColor
*
* Description:
*	This function will change the pen color.
*
* Inputs:
*	rgb	- Pen color
*
* Returns: void
*
* Last Modified: April 30, 2021 by Isaiah Regacho
*****************************************************************************************/
void setScaraColor(int r, int g, int b) {
	char cmd[MAX_STRING];
	sprintf(cmd,"PEN_COLOR %d %d %d\n", r, g, b); 
	robot.Send(cmd);
}
