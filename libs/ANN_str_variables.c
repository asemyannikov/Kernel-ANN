#include "./ANN_lib_graph.h"

void 
declare_variables (Variables_t *variables) {
	variables->variable = NULL;
	variables->rows = (size_t) 0;
}

void 
initialize_variables (Variables_t *variables, Variable_t variable) {
	if (variables->rows == (size_t) 0) {
		variables->variable = Calloc((size_t) 1, sizeof(Variable_t));
	} else {
		variables->variable = Realloc(variables->variable, (variables->rows + (size_t) 1) * sizeof(Variable_t));
	}
	variables->variable[variables->rows] = variable;
	variables->rows = variables->rows + (size_t) 1;
}

void 
free_variables (Variables_t *variables) {
	size_t index;

	for (index = (size_t) 0; index < variables->rows; index = index + (size_t) 1) {
		free_obj(&variables->variable[index]);
	}
	variables->rows = (size_t) 0;
	free(variables->variable);
}