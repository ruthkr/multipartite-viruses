#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float uniform_dist(float, float);
void ssa_gillespie(float[], float *);

// Random Uniform Distribution
float uniform_dist(float min, float max) {
	rand(); // Fix for random
	float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
	return min + scale * ( max - min );      /* [min, max] */
}

void ssa_gillespie(float X[], float *params) {
	float r1 = uniform_dist(0, 1);
	float r2 = uniform_dist(0, 1);

	// X[1] = R1, X[2] = R2, X[3] = p, X[4] = s, X[5] = v1, X[6] = v2

	// Transition rates
	float *w = calloc(23, sizeof(float));
	float *wsum = calloc(23, sizeof(float));

	// Current capacity variables
	float one_over_cc = 1 / params[6];
	float one_over_cc_sqr = 1 / pow(params[6], 2);

	// Reaction Rates for RNA1
	w[1] = params[0] * X[1] * X[3] * one_over_cc;
	w[2] = params[0] * X[1] * X[1] * X[3] * one_over_cc_sqr;
	w[3] = params[0] * X[1] * X[2] * X[3] * one_over_cc_sqr;
	w[4] = params[4] * X[1] * X[4] * one_over_cc;
	w[5] = params[2] * X[1];

	// Reaction Rates for RNA2
	w[6] = params[0] * X[2] * X[3] * one_over_cc;
	w[7] = params[0] * X[2] * X[2] * X[3] * one_over_cc_sqr;
	w[8] = params[0] * X[1] * X[2] * X[3] * one_over_cc_sqr;
	w[9] = params[4] * X[2] * X[4] * one_over_cc;
	w[10] = params[2] * X[2];

	// Reaction Rates for viral replicase
	w[11] = params[1] * X[1];
	w[12] = params[1] * X[1] * X[3] * one_over_cc;
	w[13] = params[1] * X[1] * X[4] * one_over_cc;
	w[14] = params[3] * X[3];

	// Reaction Rates for coat protein
	w[15] = params[1] * X[2];
	w[16] = params[1] * X[2] * X[3] * one_over_cc;
	w[17] = params[1] * X[2] * X[4] * one_over_cc;
	w[18] = params[3] * X[4];

	// Reaction Rates for virion1
	w[19] = params[4] * X[1] * X[4] * one_over_cc;
	w[20] = params[5] * X[5];

	// Reaction Rates for virion2
	w[21] = params[4] * X[2] * X[4] * one_over_cc;
	w[22] = params[5] * X[6];


	// Total Number of all of the transition rates above
	for (int i = 1; i <= 22; i++) {
		w[0] += w[i];
	}

	// Partial sums of the rates (to be evaluated in the conditionals)
	wsum[0] = 0;

	for (int i = 1; i <= 22; i++) {
		for (int j = 1; j <= i; j++) {
			wsum[i] += w[j];
		}
		wsum[i] /= w[0];
	}

	// Timestep tau
	float tau = 1/w[0] * log(1/r1);

	// Compute at time + tau
	if ((wsum[0] <= r2) & (r2 < wsum[1])) {
		X[1] += 1;
	} else if ( (wsum[1] <= r2) & (r2 < wsum[2]) ) {
		X[1] -= 1;
	} else if ( (wsum[2] <= r2) & (r2 < wsum[3]) ) {
		X[1] -= 1;
	} else if ( (wsum[3] <= r2) & (r2 < wsum[4]) ) {
		X[1] -= 1;
	} else if ( (wsum[4] <= r2) & (r2 < wsum[5]) ) {
		X[1] -= 1;
	} else if ( (wsum[5] <= r2) & (r2 < wsum[6]) ) {
		X[2] += 1;
	} else if ( (wsum[6] <= r2) & (r2 < wsum[7]) ) {
		X[2] -= 1;
	} else if ( (wsum[7] <= r2) & (r2 < wsum[8]) ) {
		X[2] -= 1;
	} else if ( (wsum[8] <= r2) & (r2 < wsum[9]) ) {
		X[2] -= 1;
	} else if ( (wsum[9] <= r2) & (r2 < wsum[10]) ) {
		X[2] -= 1;
	} else if ( (wsum[10] <= r2) & (r2 < wsum[11]) ) {
		X[3] += 1;
	} else if ( (wsum[11] <= r2) & (r2 < wsum[12]) ) {
		X[3] -= 1;
	} else if ( (wsum[12] <= r2) & (r2 < wsum[13]) ) {
		X[3] -= 1;
	} else if ( (wsum[13] <= r2) & (r2 < wsum[14]) ) {
		X[3] -= 1;
	} else if ( (wsum[14] <= r2) & (r2 < wsum[15]) ) {
		X[4] += 1;
	} else if ( (wsum[15] <= r2) & (r2 < wsum[16]) ) {
		X[4] -= 1;
	} else if ( (wsum[16] <= r2) & (r2 < wsum[17]) ) {
		X[4] -= 1;
	} else if ( (wsum[17] <= r2) & (r2 < wsum[18]) ) {
		X[4] -= 1;
	} else if ( (wsum[18] <= r2) & (r2 < wsum[19]) ) {
		X[5] += 1;
	} else if ( (wsum[19] <= r2) & (r2 < wsum[20]) ) {
		X[5] -= 1;
	} else if ( (wsum[20] <= r2) & (r2 < wsum[21]) ) {
		X[6] += 1;
	} else if ( (wsum[21] <= r2) & (r2 < wsum[22]) ) {
		X[6] -= 1;
	}
	// Update time step
	X[0] += tau;

	free(w);
	free(wsum);
}

int main(int argc, char const *argv[]) {

	// Maximum Time
	float max_time = atof(argv[1]);

	// Initial Condition
	float X[] = {0, 0, 0, 0, 0, 0, 0};

	// Parameters
	float *params = calloc(7, sizeof(float));
	params[0] = atof(argv[8]);   // kappa            : default 1
	params[1] = atof(argv[9]);   // omega            : default 1
	params[2] = atof(argv[10]);  // gamma
	params[3] = atof(argv[11]);  // sigma            : default 0.1
	params[4] = atof(argv[12]);  // epsilon          : default 0.1
	params[5] = atof(argv[13]);  // delta            : default 0.1
	params[6] = atof(argv[14]);  // current capacity : default 1000

	// Seed
	srand((unsigned)time(NULL));

	int j;
	int count_prob = 0;
	int nsim = atoi(argv[15]);

	// Loop
	for(j = 0; j < nsim; j++) {
		unsigned long int step = 0;


		// Reset Initial Condition
		X[0] = 0;
		X[1] = atof(argv[2]);
		X[2] = atof(argv[3]);

		while (X[0] < max_time) {
			// printf("Step: %ld\n", step);
			ssa_gillespie(X, params);
			step += 1;
			// printf("%f, %f, %f, %f, %f, %f, %f\n", X[0], X[1], X[2], X[3], X[4], X[5], X[6]);
		}

		if ((X[1] <= 0) & (X[2] <= 0)) {
			count_prob += 1;
		}
		printf("%f, %d, %d, %lu\n", params[2], count_prob, j, step);
	}

	float prob_exit = (float) count_prob / nsim;
	printf("%f\n", prob_exit);

	free(params);
	return 0;
}


