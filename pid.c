#define kP 1
#define kD 10

float lastPosition = 0;

float pid(float setpoint, float position) {
	float error = setpoint - position;

	float prop = kP * error;
	float deriv = kD * (position - lastPosition);
	//check if first pass through
	if (!floor(lastPosition)) {
		deriv = 0;
	}
	return prop - deriv;

}

int *drive(int speed, float pos, float setpoint) {
	int turn = pid(setpoint, pos);
	int speeds[2] = {speed + turn, speed - turn};
	return speeds;
}
