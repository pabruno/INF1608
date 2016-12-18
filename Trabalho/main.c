#include <stdio.h>
#include <math.h>
#include "pendulum.h"

int main() {

	RKAdaptiveStep(0.00001, 0.1, PI / 10, 3.0, 0.01, 0.2);
	return 0;
}