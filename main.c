#pragma config(Sensor, S1,     line,           sensorEV3_Color)
#pragma config(Sensor, S2,     gyro,           sensorEV3_Gyro)
#pragma config(Motor,  motorB,          left,          tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          right,         tmotorEV3_Large, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#define threshold 30
#define STATE_CHARGE 0
#define STATE_TURN   1


#include "pid.c"
int targetAngle = 0;
int state = STATE_CHARGE;
int gyroRead=0;
int colorRead=0;

task sensorRead() {
	int lastRead=1000;
	while(true) {
		gyroRead=getGyroHeading(S2);
		colorRead=getColorReflected(line);
		if(colorRead<threshold&&lastRead>threshold){
			//go into turning state
			state=STATE_TURN;
			targetAngle+=100;
		}
		if(gyroRead>=targetAngle){
			state=STATE_CHARGE;
		}
		lastRead=colorRead;
	}
}

task main() {
	//reset gyro
	sensorReset(S2);
	resetGyro(S2);

	//start state controller
	startTask(sensorRead);



	while (true) {
		switch (state) {
		case STATE_TURN:

			int *speeds = drive(0, gyroRead, targetAngle);
			setMotorSpeed(motorB, speeds[0]);
			setMotorSpeed(motorC, speeds[1]);
			break;

		case STATE_CHARGE:
			setMotorSpeed(motorB, 100);
			setMotorSpeed(motorC, 100);
			break;
		default:
			writeDebugStreamLine("Entered unknown state %d, aborting...", state);
			stopAllTasks();
		}
	}
}
