#define kP 2
#define kD 0

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

int *drive(float speed, float pos, float setpoint) {
	float turn = pid(setpoint, pos);
	int speeds[2] = {round(speed - turn), round(speed + turn)};
	return speeds;
}
