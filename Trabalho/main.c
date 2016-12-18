#include <stdio.h>
#include <math.h>
#include "pendulum.h"

int main() {

	RKAdaptiveStep(0.00001, 0.1, PI / 20, 3.0, 0.01, 0.2);
	RKAdaptiveStep(0.00001, 0.1, PI / 30, 3.0, 0.01, 0.2);
	RKAdaptiveStep(0.00001, 0.1, PI / 6, 3.0, 0.01, 0.2);
	RKAdaptiveStep(0.00001, 0.1, PI / 3, 3.0, 0.01, 0.2);
	return 0;
}