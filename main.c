#pragma config(Sensor, S1,     line,           sensorEV3_Color)
#pragma config(Sensor, S2,     gyro,           sensorEV3_Gyro)
#pragma config(Motor,  motorB,          left,          tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          right,         tmotorEV3_Large, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
#define threshold 25
#define ANGLE_CHANGE -50
#define STATE_CHARGE 0
#define STATE_TURN   1
#define STATE_REVERSE 2

#define DEBUG 1

int targetAngle = 0;
int state = STATE_CHARGE;
int gyroRead = 0;
int colorRead = 0;

task sensorRead() {
	int lastRead = 1000;
	while (true) {
		gyroRead = getGyroDegrees(gyro);
		colorRead = getColorReflected(line);

		if (DEBUG) {
			displayCenteredBigTextLine(0, "ANGLE:  %d", gyroRead);
			displayCenteredBigTextLine(2, "TARGET: %d", targetAngle);
			displayCenteredBigTextLine(4, "LS CUR: %d", colorRead);
			displayCenteredBigTextLine(6, "LS LST: %d", lastRead);
		}

		if (colorRead < threshold && lastRead > threshold) {
			// go into turning state
			if (DEBUG) displayCenteredBigTextLine(8, "STATE: %s (*)", state == STATE_CHARGE ? "C" : "T");
			state = STATE_REVERSE;
			targetAngle += ANGLE_CHANGE;
		}

		if (gyroRead <= targetAngle) {
			targetAngle = gyroRead;
			if (DEBUG) displayCenteredBigTextLine(8, "STATE: %s (*)", state == STATE_CHARGE ? "C" : "T");
			state = STATE_CHARGE;
		}

		lastRead = colorRead;
	}
}

task main() {
	//reset gyro
	sensorReset(gyro);
	resetGyro(gyro);

	//start state controller
	startTask(sensorRead);

	while (true) {
		if (DEBUG) displayCenteredBigTextLine(8, "STATE: %s", state == STATE_CHARGE ? "C" : "T");
		switch (state) {
		case STATE_TURN:
			setMotorSpeed(motorB, 0);
			setMotorSpeed(motorC, -100);
			break;
		case STATE_CHARGE:
			setMotorSpeed(motorB, 80);
			setMotorSpeed(motorC, 80);
			break;
		case STATE_REVERSE:
			setMotorSpeed(motorB, -100);
			setMotorSpeed(motorC, -100);
			sleep(500);
			state = STATE_TURN;
			break;
		default:
			writeDebugStreamLine("Entered unknown state %d, aborting...", state);
			stopAllTasks();
		}
	}
}
