#define kP 1
#define kD 10

double lastPosition = 0;

double pid(double setpoint, double position) {
	double error = setpoint - position;

	double prop = kP * error;
	double deriv = kD * (position - lastPosition);
	//check if first pass through
	if (!floor(lastPosition)) {
		deriv = 0;
	}
	return prop - deriv;

}

int *drive(int speed, double pos, double setpoint){
	int turn = pid(setpoint, pos);
	int speeds[2] = {speed + turn, speed - turn};
	return speeds;
}
