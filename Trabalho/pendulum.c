#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pendulum.h"

static double acceleration(double theta) {
	return -(G / L)*theta;
}

static double analyticalResult(double theta0, double t) {
	return theta0*cos(sqrt((G*t) / L));
}

static double RKFixedStep(double h, double theta0, double *v) {
	double dTheta1, dTheta2, dTheta3, dTheta4;
	double dV1, dV2, dV3, dV4;

	dTheta1 = h*(*v);
	dV1 = h*acceleration(theta0);

	dTheta2 = h*(*v + (dV1 / 2.0));
	dV2 = h*acceleration(theta0 + (dTheta1 / 2.0));

	dTheta3 = h*(*v + (dV2 / 2.0));
	dV3 = h*acceleration(theta0 + (dTheta2 / 2.0));

	dTheta4 = h*(*v + dV3);
	dV4 = h*acceleration(theta0 + dTheta3);

	*v += ((dV1 + (2 * dV2) + (2 * dV3) + dV4) / 6.0);

	return theta0 + ((dTheta1 + (2 * dTheta2) + (2 * dTheta3) + dTheta4) / 6.0);
}

void RKAdaptiveStep(double e, double h, double theta0, double time, double Hmin, double Hmax) {
	double step = h, theta1, theta2_1, theta2_2, theta = theta0;
	double i = 0.0;
	double delta;
	double v1 = 0.0, v2 = 0.0;
	double aux1;
	int numPassos;
	FILE *analytical = fopen("analytical.txt", "w");
	FILE *calculated = fopen("calculated.txt", "w");

	if (analytical == NULL || calculated == NULL) {
		printf("ERRO NA ABERTURA DE ARQUIVO\n");
		exit(1);
	}

	fprintf(calculated, "c:=pointplot([");
	fprintf(analytical, "a:=pointplot([");
	fprintf(calculated, "[%.5g, %.5g]", 0.0, theta0);
	fprintf(analytical, "[%.5g, %.5g]", 0.0, analyticalResult(theta0, 0.0));

	while (i < time) {
		numPassos = 0;
		while (1) {
			aux1 = v1;
			v2 = v1;
			theta1 = RKFixedStep(step, theta, &v1);
			theta2_1 = RKFixedStep(step / 2, theta, &v2);
			theta2_2 = RKFixedStep(step / 2, theta2_1, &v2);

			delta = fabs(theta2_2 - theta1);
			numPassos++;

			if (step < Hmin)
				step = Hmin;
			else if (step > Hmax)
				step = Hmax;

			if (delta < e || numPassos > 100)
				break;

			v1 = aux1;
			step = sqrt(e / delta)*step;
		}

		i += step;
		theta = theta1;
		fprintf(calculated, ",[%.5g, %.5g]", i, theta);
		fprintf(analytical, ",[%.5g, %.5g]", i, analyticalResult(theta0, i));
	}

	fprintf(calculated, "],color=red):");
	fprintf(analytical, "],color=green):");

	fclose(calculated);
	fclose(analytical);
}