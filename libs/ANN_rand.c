#include "./ANN_lib_machine.h"

void 
Random_r(struct random_data *randomdata, int32_t *result) {
	int errno_saved;
	
	errno_saved = errno;
	errno = 0;
	if (random_r(randomdata, result) == -1) {
		errno = errno_saved;
		exit(EXIT_FAILURE);
	}
}

int 
Initstate_r(unsigned int seed, char *statebuf, size_t statelen, struct random_data *randomdata) {
	int errno_saved;
	int res;
	
	errno_saved = errno;
	errno = 0;
	if ((res = initstate_r(seed, statebuf, statelen, randomdata)) == -1) {
		errno = errno_saved;
		exit(EXIT_FAILURE);
	}
	return res;
}

void 
get_random_variable(char *statebuf, size_t statelen, struct random_data *randomdata, int32_t *result) {
	memset(randomdata, 0, sizeof(struct random_data));
	Initstate_r(time(NULL), statebuf, statelen, randomdata);
	Random_r(randomdata, result);
}

void 
get_random_variable_array(char *statebuf, size_t statelen, struct random_data *randomdata, int32_t *result, size_t len, int32_t rand_min, int32_t rand_max) {
	size_t index;
	
	memset(randomdata, 0, sizeof(struct random_data));
	Initstate_r(time(NULL), statebuf, statelen, randomdata);
	for (index = 0; index < len; index = index + 1) {
		Random_r(randomdata, &result[index]);
		result[index] = (result[index] % (rand_max - rand_min)) + rand_min;
	}
}

void 
random_uniform_array(char *statebuf, size_t statelen, struct random_data *randomdata, double *result, size_t len, double rand_min, double rand_max) {
	size_t index;
	int32_t random_variable;

	memset(randomdata, 0, sizeof(struct random_data));
	Initstate_r(time(NULL), statebuf, statelen, randomdata);
	for (index = 0; index < len; index = index + 1) {
		Random_r(randomdata, &random_variable);
		result[index] = rand_min + ((double)random_variable * (rand_max - rand_min)) / RAND_MAX;
	}
}

void 
random_normal_noise_algBoxMuller(char *statebuf, size_t statelen, struct random_data *randomdata, double *result, size_t len) {
	double uniform_variables[2];
	double square;
	size_t counter;
	size_t index;
	int32_t random_variable;

	counter = 0;
	memset(randomdata, 0, sizeof(struct random_data));
	Initstate_r(time(NULL), statebuf, statelen, randomdata);
	while (counter < len) {
		for (index = 0; index < 2; index = index + (size_t) 1) {
			Random_r(randomdata, &random_variable);
			uniform_variables[index] = ((double)random_variable * 2.0) / RAND_MAX - 1.0;
		}
		square = uniform_variables[0] * uniform_variables[0] + uniform_variables[1] * uniform_variables[1];
		if (square > 0 && square <= 1) {
			result[counter] = uniform_variables[0] * sqrt(-2.0 * log(square) / square);
			counter = counter + (size_t) 1;
		}
	}
}

void 
random_normal_array(char *statebuf, size_t statelen, struct random_data *randomdata, double *result, size_t len, double expected_value, double standard_deviation) {
	size_t index;
	double noise[len];

	random_normal_noise_algBoxMuller(statebuf, statelen, randomdata, noise, len);
	for (index = 0; index < len; index = index + (size_t) 1) {
		result[index] = expected_value + noise[index] * standard_deviation;
	}
}

void 
Random_choise (int32_t *result, size_t len, int32_t rand_min, int32_t rand_max) {
	#define STATEBUFFERLEN 256
	char* randomStateBuffer[STATEBUFFERLEN];
	struct random_data randomData;

	get_random_variable_array((char *)&randomStateBuffer, STATEBUFFERLEN, (struct random_data *)&randomData, result, len, rand_min, rand_max);
}

void
Uniform_dispersion (double *result, size_t len, double rand_min, double rand_max) {
	#define STATEBUFFERLEN 256
	char* randomStateBuffer[STATEBUFFERLEN];
	struct random_data randomData;

	random_uniform_array((char *)&randomStateBuffer, STATEBUFFERLEN, (struct random_data *)&randomData, result, len, rand_min, rand_max);
}

void
Normal_dispersion (double *result, size_t len, double rand_min, double rand_max) {
	#define STATEBUFFERLEN 256
	char* randomStateBuffer[STATEBUFFERLEN];
	struct random_data randomData;

	random_normal_array((char *)&randomStateBuffer, STATEBUFFERLEN, (struct random_data *)&randomData, result, len, rand_min, rand_max);
}

void
initial_weight_by_lecun_uniform (double *result, size_t len, int fan_in) {
	double limit;

	limit = sqrt(3.0 / fan_in);
	Uniform_dispersion(result, len, -limit, limit);
}

void
initial_weight_by_glorot_uniform (double *result, size_t len, int fan_in, int fan_out) {
	double limit;

	limit = sqrt(6.0 / (fan_in + fan_out));
	Uniform_dispersion(result, len, -limit, limit);
}

void
initial_weight_by_he_normal (double *result, size_t len, int fan_in) {
	Normal_dispersion(result, len, 0.0, sqrt(2.0 / fan_in));
}

void
initial_weight_by_lecun_normal (double *result, size_t len, int fan_in) {
	Normal_dispersion(result, len, 0.0, sqrt(1.0 / fan_in));
}