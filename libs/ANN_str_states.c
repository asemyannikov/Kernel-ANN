#include "./ANN_lib_machine.h"

void 
init_state (State *state) {
	state->data  = NULL;
	state->length = (size_t) 0;
}

void 
add_record_to_state (State *state, double data) {
	if (state->length > 0) {
		state->data = Realloc(state->data, (state->length + (size_t) 1) * sizeof(double));
	} else {
		state->data = Calloc((size_t) 1, sizeof(double));
	}
	state->data[state->length] = data;
	state->length = state->length + (size_t) 1;
}

void 
add_data_to_state (State *state, double data[], size_t length) {
	size_t index;
	size_t new_length;

	new_length = state->length + length;
	if (state->length > 0) {
		state->data = Realloc(state->data, new_length * sizeof(double));
	} else {
		state->data = Calloc(length, sizeof(double));
	}
	for (index = state->length; index < new_length; index = index + (size_t) 1) {
		state->data[index] = data[index - state->length];
	}
	state->length = new_length;
}

void
free_state (State *state) {
	free(state->data);
	init_state(state);
}

size_t
get_dimension (const char *vector, const size_t length) {
	size_t dimension;
	int index;

	dimension = (size_t) 0;
	for (index = (size_t) 0; index < length; index = index + (size_t) 1) {
		if (vector[index] == '\n') {
			dimension = dimension + (size_t) 1;
		}
	}
	return dimension;
}

void
set_initial_states (State state[QUANTITYOFVARIABLES]) {
	size_t index;
	size_t dimension;
	size_t index_value;
	char *value;
	Message_t data;
	double *initial_data[QUANTITYOFVARIABLES];
	char *str;
	for (index = 0; index < QUANTITYOFVARIABLES; index = index + (size_t) 1) {
		get_data(paths_to_data[index], &data);
		dimension = get_dimension(data.text, data.length);
		if (dimension == (size_t) 0) {
			dimension = dimension + (size_t) 1;
		}
		initial_data[index] = Calloc(dimension + (size_t) 1, sizeof(double));
		str = data.text;
		index_value = (size_t) 0;
		while (value = strtok_r(str, "\n", &str)) {
			initial_data[index][index_value] = strtod(value, NULL);
			index_value = index_value + (size_t) 1;
		}
		free_obj(&data);
		init_state(&state[index]);
		add_data_to_state(&state[index], initial_data[index], dimension);
		free(initial_data[index]);
	}
}

void
set_minibatch (State state[QUANTITYOFVARIABLES], size_t *dimension) {
	size_t index_of_variable;
	size_t index_of_value;
	double batch[BATCHSIZE];
	int32_t indices[BATCHSIZE];
	
	if (*dimension > BATCHSIZE) {
		*dimension = BATCHSIZE;
		Random_choise(indices, BATCHSIZE, 0, *dimension);
		for (index_of_variable = (size_t) 0; index_of_variable < QUANTITYOFVARIABLES; index_of_variable = index_of_variable + (size_t) 1) {
			if (state[index_of_variable].length > BATCHSIZE) {
				for (index_of_value = (size_t) 0; index_of_value < BATCHSIZE; index_of_value = index_of_value + (size_t) 1) {
					batch[index_of_value] = state[index_of_variable].data[indices[index_of_value]];
				}
				free_state(&state[index_of_variable]);
				init_state(&state[index_of_variable]);
				add_data_to_state(&state[index_of_variable], batch, BATCHSIZE);
			}
		}
	} else {
		print_error_message("[WARNING MESSAGE] A size of minibatch is bigger than dimension initial state vectors.\n");
	}
}