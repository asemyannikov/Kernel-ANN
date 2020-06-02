#include "./ANN_lib_graph.h"

size_t
make_masks (Bud_t *bud, Message_t *source, size_t number_of_bud) {
	Boolean_t isFunction, isLArgNumber, isRArgNumber;
	Message_t number;

	initialize_message(source, "double mask_", (size_t) 12);
	set_number(&number, number_of_bud);
	initialize_message(source, number.text, number.length);
	free_obj(&number);
	initialize_message(source, " (vector point) { return ", (size_t) 25);
	isFunction = is_function(&bud->petals[LEFT].operand);
	isLArgNumber = is_number(&bud->petals[LEFT].operand);
	isRArgNumber = is_number(&bud->petals[RIGHT].operand);
	if (isFunction == FALSE) {
		if (bud->operation != '^') {
			if (isLArgNumber == FALSE) {
				initialize_message(source, "point.left_variable.value", (size_t) 25);
			} else {
				initialize_message(source, bud->petals[LEFT].operand.text, bud->petals[LEFT].operand.length);
			}
			initialize_message(source, &(bud->operation), (size_t) 1);
			if (isRArgNumber == FALSE) {
				initialize_message(source, "point.right_variable.value", (size_t) 26);
			} else {
				initialize_message(source, bud->petals[RIGHT].operand.text, bud->petals[RIGHT].operand.length);
			}
		} else {
			initialize_message(source, "pow(", (size_t) 4);
			if (isLArgNumber == FALSE) {
				initialize_message(source, "point.left_variable.value", (size_t) 25);
			} else {
				initialize_message(source, bud->petals[LEFT].operand.text, bud->petals[LEFT].operand.length);
			}
			initialize_message(source, ",", (size_t) 1);
			if (isRArgNumber == FALSE) {
				initialize_message(source, "point.right_variable.value", (size_t) 26);
			} else {
				initialize_message(source, bud->petals[RIGHT].operand.text, bud->petals[RIGHT].operand.length);
			}
			initialize_message(source, ")", (size_t) 1);
		}
	} else {
		initialize_message(source, bud->petals[LEFT].operand.text, bud->petals[LEFT].operand.length);
		initialize_message(source, "(point.right_variable.value)", (size_t) 28);
	}
	initialize_message(source, "; }\n", (size_t) 4);
}

void
make_map (Flower_t *flower, Message_t *source) {
	size_t index;
	Message_t number;
	Bud_t *bud;

	initialize_message(source, "void init_buds (vector point[QUANTITYOFBUDS], variable buds[QUANTITYOFBUDS], variable variables[QUANTITYOFVARIABLES], int index) {\n\tswitch (index) {\n", (size_t) 149);
	index = (size_t) 0;
	bud = flower->root;
	while (bud != NULL) {
		if (bud->petals[RIGHT].source != NULL && bud->petals[RIGHT].source->visited == FALSE) {
			bud = bud->petals[RIGHT].source;
		} else if (bud->petals[LEFT].source != NULL && bud->petals[LEFT].source->visited == FALSE) {
			bud = bud->petals[LEFT].source;
		} else if (bud->visited == FALSE) {
			initialize_message(source, "\t\tcase ", (size_t) 7);
			set_number(&number, index);
			initialize_message(source, number.text, number.length);
			initialize_message(source, ": init_vector(&point[", (size_t) 21);
			initialize_message(source, number.text, number.length);
			free_obj(&number);
			initialize_message(source, "], ", (size_t) 3);
			if ((strstr(bud->petals[LEFT].operand.text, "variables") != NULL) || (strstr(bud->petals[LEFT].operand.text, "bud") != NULL)) {
				initialize_message(source, bud->petals[LEFT].operand.text, bud->petals[LEFT].operand.length);
			} else {
				initialize_message(source, "ZERO", (size_t) 4);
			}
			initialize_message(source, ", ", (size_t) 2);
			if ((strstr(bud->petals[RIGHT].operand.text, "variables") != NULL) || (strstr(bud->petals[RIGHT].operand.text, "bud") != NULL)) {
				initialize_message(source, bud->petals[RIGHT].operand.text, bud->petals[RIGHT].operand.length);
			} else {
				initialize_message(source, "ZERO", (size_t) 4);
			}
			initialize_message(source, "); break;\n", (size_t) 10);
			index = index + (size_t) 1;
			bud->visited = TRUE;
		} else {
			bud = bud->neighbours[NEXT];
		}
	}
	reset_traversal(flower);
	initialize_message(source, "}};", (size_t) 3);
}

void
make_vector_mask (Message_t *source, size_t number_of_bud) {
	size_t index;
	Message_t number;

	initialize_message(source, "double (*functions[QUANTITYOFBUDS])(vector) = {", (size_t) 47);
	for (index = (size_t) 1; index < number_of_bud; index = index + (size_t) 1) {
		initialize_message(source, "&mask_", (size_t) 6);
		set_number(&number, index);
		initialize_message(source, number.text, number.length);
		free_obj(&number);
		initialize_message(source, ", ", (size_t) 2);
	}
	initialize_message(source, "&mask_", (size_t) 6);
	set_number(&number, number_of_bud);
	initialize_message(source, number.text, number.length);
	free_obj(&number);
	initialize_message(source, "};\n", (size_t) 3);
}

void
make_paths_record (Message_t *source, Variables_t variables) {
	size_t index_i;
	size_t index_j;
	size_t rows;
	Message_t number;

	initialize_message(source, "char *paths_to_data[QUANTITYOFVARIABLES] = {", (size_t) 44);
	rows = variables.rows - (size_t) 1;
	for (index_i = (size_t) 0; index_i < rows; index_i = index_i + (size_t) 1) {
		for (index_j = (size_t) 0; index_j < rows; index_j = index_j + (size_t) 1) {
			if (index_i == variables.variable[index_j].index_in_flower) {
				break;
			}
		}
		initialize_message(source, "\"", (size_t) 1);
		initialize_message(source, variables.variable[index_j].path_to_inital_state.text, variables.variable[index_j].path_to_inital_state.length);
		initialize_message(source, "\", ", (size_t) 3);
	}
	initialize_message(source, "\"", (size_t) 1);
	initialize_message(source, variables.variable[rows].path_to_inital_state.text, variables.variable[rows].path_to_inital_state.length);
	initialize_message(source, "\"};\n", (size_t) 4);
}

void
make_learning_indices_record (Message_t *source, Variables_t variables) {
	size_t index;
	size_t rows;
	Message_t number;
	
	initialize_message(source, "int learning_variables_indices[QUANTITYOFLEARNINGVARIABLES] = {", (size_t) 63);
	rows = variables.rows - (size_t) 1;
	for (index = (size_t) 0; index < rows; index = index + (size_t) 1) {
		if (variables.variable[index].learning == TRUE) {
			set_number(&number, variables.variable[index].index_in_flower);
			initialize_message(source, number.text, number.length);
			free_obj(&number);
			initialize_message(source, ", ", (size_t) 2);
		}
	}
	set_number(&number, variables.variable[rows].index_in_flower);
	initialize_message(source, number.text, number.length);
	free_obj(&number);
	initialize_message(source, "};\n", (size_t) 3);
}

void
write_functions_into_file (int fd, Flower_t *flower, Variables_t variables) {
	Bud_t *bud;
	Message_t source;
	size_t index_of_bud;

	index_of_bud = (size_t) 0;
	declare_obj(&source);
	initialize_message(&source, "#include \"./ANN_lib_machine.h\"\nconst variable ZERO = {0., 0.};\n", (size_t) 63);
	bud = flower->root;
	while (bud != NULL) {
		if (bud->petals[RIGHT].source != NULL && bud->petals[RIGHT].source->visited == FALSE) {
			bud = bud->petals[RIGHT].source;
		} else if (bud->petals[LEFT].source != NULL && bud->petals[LEFT].source->visited == FALSE) {
			bud = bud->petals[LEFT].source;
		} else if (bud->visited == FALSE) {
			index_of_bud = index_of_bud + (size_t) 1;
			make_masks(bud, &source, index_of_bud);
			bud->visited = TRUE;
		} else {
			bud = bud->neighbours[NEXT];
		}
	}
	reset_traversal(flower);
	make_vector_mask(&source, index_of_bud);
	make_paths_record(&source, variables);
	make_learning_indices_record(&source, variables);
	make_map(flower, &source);
	Write(fd, source.text, source.length - (size_t) 2);
	free_obj(&source);
}

void
write_constants_into_file (int fd, Flower_t *flower, Variables_t variables) {
	Message_t source;
	Message_t number;
	size_t index;
	size_t quantity_of_learning_variables;

	declare_obj(&source);

	initialize_message(&source, "#define QUANTITYOFVARIABLES ", (size_t) 28);
	set_number(&number, flower->number_of_variables);
	initialize_message(&source, number.text, number.length);
	initialize_message(&source, "\n", (size_t) 1);
	free_obj(&number);

	initialize_message(&source, "#define QUANTITYOFLEARNINGVARIABLES ", (size_t) 36);
	quantity_of_learning_variables = (size_t) 0;
	for (index = (size_t) 0; index < variables.rows; index = index + (size_t) 1) {
		if (variables.variable[index].learning == TRUE) {
			quantity_of_learning_variables = quantity_of_learning_variables + (size_t) 1;
		}
	}
	set_number(&number, quantity_of_learning_variables);
	initialize_message(&source, number.text, number.length);
	initialize_message(&source, "\n", (size_t) 1);
	free_obj(&number);

	initialize_message(&source, "#define QUANTITYOFBUDS ", (size_t) 23);
	set_number(&number, flower->number_of_buds);
	initialize_message(&source, number.text, number.length);
	initialize_message(&source, "\n", (size_t) 1);
	free_obj(&number);

	Write(fd, source.text, source.length);

	free_obj(&source);
}

int
create_functions_library (Flower_t *flower, Variables_t variables) {
	int fd;
	Message_t path;
	
	declare_obj(&path);
	initialize_message(&path, "./libs/", (size_t) 7);
	initialize_message(&path, "functions.c", (size_t) 11);
	if ((fd = Open(path.text, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
		return -1;
	} else {
		write_functions_into_file(fd, flower, variables);
	}
	free_obj(&path);
	if (Close(fd) == -1) {
		return -1;
	}
	return 0;
}

int
create_constants_library (Flower_t *flower, Variables_t variables) {
	int fd;
	Message_t path;
	
	declare_obj(&path);
	initialize_message(&path, "./libs/", (size_t) 7);
	initialize_message(&path, "graph_parameters.h", (size_t) 18);
	if ((fd = Open(path.text, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1) {
		return -1;
	} else {
		write_constants_into_file(fd, flower, variables);
	}
	free_obj(&path);
	if (Close(fd) == -1) {
		return -1;
	}
	return 0;
}

int
create_dynamic_library (Flower_t *flower, Variables_t variables) {
	int status;
	
	status = create_functions_library(flower, variables);
	status = create_constants_library(flower, variables);
	return status;
}