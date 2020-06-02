#include "./ANN_lib_graph.h"

char operations[] = {
	':',		/* Операция аргументации */
	'^',		/* Операция возведения в степень */
	'*',		/* Операция умножения */
	'/',		/* Операция деления */
	'+',		/* Операция сложения */
	'-',		/* Операция вычитания */
	'\0'
};

size_t replace_minus (const Message_t source, Message_t *result);
size_t create_negative_operator (const Message_t source, Message_t *result);
size_t prepare_negative_parameters (const Message_t source, Message_t *result);
size_t prepare_functions (Message_t *result);

Boolean_t
check_symbol (char symbol) {
	size_t index;
	
	if (symbol == '(' || symbol == ')') {
		return TRUE;
	}
	for (index = (size_t) 0; operations[index] != '\0'; index = index + (size_t) 1) {
		if (symbol == operations[index]) {
			return TRUE;
		}
	}
	return FALSE;
}

size_t
get_quantity_of_operations (Message_t source) {
	size_t quantity_of_operations, index_by_operations;
	char *ptr, *tmp;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] get_quantity_operations() : A destination string are empty.\n");
		return (size_t) 0;
	}
	quantity_of_operations = index_by_operations = (size_t) 0;
	while (operations[index_by_operations] != '\0') {
		ptr = source.text;
		for (; tmp = strchr(ptr, operations[index_by_operations]); ++quantity_of_operations) {
			ptr = tmp + (size_t) 1;
		}
		index_by_operations = index_by_operations + (size_t) 1;
	}
	return quantity_of_operations;
}

void
make_label_of_middle_part (Message_t *result, size_t number) {
	result->length = snprintf(NULL, 0, "%ld", number) + (size_t) 6;
	result->text = Calloc(result->length + (size_t) 1, sizeof(char));
	snprintf(result->text, result->length + (size_t) 1, "buds[%ld]", number);
}

size_t
prepare_equation (Message_t *result) {
	Message_t equation;

	if (!result->text) {
		print_error_message("[WARNING MESSAGE] prepare_equation() : A equation string is empty.\n");
		return (size_t) 0;
	}
	declare_obj(&equation);
	initialize_message(&equation, result->text, result->length);
	free_obj(result);
	declare_obj(result);
	if (prepare_negative_parameters(equation, result) == (size_t) 0) {
		return (size_t) 0;
	}
	if (prepare_functions(result) == (size_t) 0) {
		return (size_t) 0;
	}
	free_obj(&equation);
	return (size_t) 1;
}

size_t
prepare_negative_parameters (const Message_t source, Message_t *result) {
	Message_t string_for_prepared_equation;
	
	if (!source.text) {
		print_error_message("[WARNING MESSAGE] prepare_negative_parameters() : A equation string is empty.\n");
		return (size_t) 0;
	}
	declare_obj(&string_for_prepared_equation);
	if (replace_minus(source, &string_for_prepared_equation) == (size_t) 0) {
		return (size_t) 0;
	}
	if (create_negative_operator(string_for_prepared_equation, result) == (size_t) 0) {
		return (size_t) 0;
	}
	free_obj(&string_for_prepared_equation);
	return (size_t) 1;
}

size_t
replace_minus (const Message_t source, Message_t *result) {
	Message_t minus;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] replace_minus() : A equation string is empty.\n");
		return (size_t) 0;
	}
	declare_obj(&minus);
	initialize_message(&minus, "(-1)*", (size_t) 5);
	declare_obj(result);
	initialize_message(result, "-", (size_t) 1);
	replace_message_in_message(source, minus, result);
	free_obj(&minus);
	return (size_t) 1;
}

size_t
create_negative_operator (const Message_t source, Message_t *result) {
	char *ptr;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] create_negative_operator() : A equation string is empty.\n");
		return (size_t) 0;
	}
	ptr = source.text;
	while (*ptr) {
		if (strstr(ptr, "-") == ptr) {
			if (source.length > strnlen(ptr, source.length)) {
				if ((ptr - (size_t) 1)[0] != '(' || check_symbol((ptr - (size_t) 1)[0]) == FALSE) {
					initialize_message(result, "+#NEGATIVE#*", (size_t) 12);
				} else {
					initialize_message(result, "#NEGATIVE#*", (size_t) 11);
				}
			} else {
				initialize_message(result, "#NEGATIVE#*", (size_t) 11);
			}
			ptr += (size_t) 1;
		} else {
			initialize_message(result, ptr, (size_t) 1);
			*ptr++;
		}
	}
	return (size_t) 1;
}

size_t
prepare_functions (Message_t *result) {
	size_t index;
	Message_t function;
	Message_t string_for_prepared_equation;

	if (!result->text) {
		print_error_message("[WARNING MESSAGE] prepare_functions() : A equation string is empty.\n");
		return (size_t) 0;
	}
	for (index = (size_t) 0; functions[index] != NULL; index = index + (size_t) 1) {
		declare_obj(&function);
		initialize_message(&function, functions[index], strlen(functions[index]));
		declare_obj(&string_for_prepared_equation);
		initialize_message(&string_for_prepared_equation, function.text, function.length);
		initialize_message(&string_for_prepared_equation, ":", (size_t) 1);
		replace_message_in_message(*result, function, &string_for_prepared_equation);
		free_obj(result);
		initialize_message(result, string_for_prepared_equation.text, string_for_prepared_equation.length);
		free_obj(&function);
		free_obj(&string_for_prepared_equation);
	}
	return (size_t) 1;
}

ssize_t
get_left_operand (const Message_t source, Message_t *result, size_t position_of_operation) {
	ssize_t size_of_word;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] get_previous_operand() : A destination string are empty.\n");
		return (ssize_t) -1;
	}
	size_of_word = (ssize_t) 0;
	while (position_of_operation > (size_t) 0) {
		if (check_symbol(source.text[position_of_operation - (size_t) 1]) == FALSE) {
			position_of_operation = position_of_operation - (size_t) 1;
			size_of_word = size_of_word + (ssize_t) 1;
		} else {
			break;
		}
	}
	declare_obj(result);
	initialize_message(result, source.text + position_of_operation, size_of_word);
	return size_of_word;
}

ssize_t
get_right_operand (const Message_t source, Message_t *result, size_t position_of_operation) {
	ssize_t size_of_word;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] get_next_operand() : A destination string are empty.\n");
		return (ssize_t) -1;
	}
	size_of_word = (ssize_t) 0;
	while (position_of_operation < source.length) {
		if (check_symbol(source.text[position_of_operation + (size_t) 1]) == FALSE) {
			position_of_operation = position_of_operation + (size_t) 1;
			size_of_word = size_of_word + (ssize_t) 1;
		} else {
			break;
		}
	}
	declare_obj(result);
	initialize_message(result, source.text + position_of_operation - size_of_word + (size_t) 1, size_of_word);
	return size_of_word;
}

size_t
get_middle_part (const Message_t source, Message_t *result) {
	char tmp[source.length];
	char *ptr;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] get_middle_part() : A destination string are empty.\n");
		return (size_t) 0;
	}
	sscanf(source.text, "%[^\\)]", tmp);
	ptr = strrchr(tmp, '(') + (size_t) 1;
	declare_obj(result);
	initialize_message(result, ptr, strnlen(ptr, source.length));
	return strlen(tmp) - strlen(ptr);
}

size_t
make_expr_of_pare (const Message_t source, Message_t *result, char *operation, Message_t *left_operand, Message_t *right_operand) {
	char *ptr;
	size_t position;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] make_expr_of_pare() : A destination string are empty.\n");
		return (size_t) 0;
	}
	declare_obj(result);
	if ((ptr = strchr(source.text, *operation)) != NULL) {
		position = source.length - strnlen(ptr, source.length);
		if (get_left_operand(source, left_operand, position) == (ssize_t) -1) {
			return (size_t) 0;
		}
		if (get_right_operand(source, right_operand, position) == (ssize_t) -1) {
			return (size_t) 0;
		}
		initialize_message(result, left_operand->text, left_operand->length);
		initialize_message(result, operation, (size_t) 1);
		initialize_message(result, right_operand->text, right_operand->length);
	} else {
		return (size_t) 0;
	}
	return (size_t) 1;
}

size_t
get_side_parts (const Message_t source, const Message_t middle_part, Message_t *first_part, Message_t *second_part) {
	size_t position;

	if (!source.text) {
		print_error_message("[WARNING MESSAGE] get_side_parts() : A destination string are empty.\n");
		return (size_t) 0;
	}
	position = strnlen(strstr(source.text, middle_part.text), source.length);
	declare_obj(first_part);
	initialize_message(first_part, source.text, source.length - position);
	declare_obj(second_part);
	if (source.length >= first_part->length + middle_part.length) {
		initialize_message(second_part, source.text + (first_part->length + middle_part.length), position - middle_part.length);
	} else {
		initialize_message(second_part, "", (size_t) 0);
	}
	return (size_t) 1;
}

void
construct_equation (Message_t *equation, Message_t middle_part, Message_t first_part, Message_t second_part) {
	if (first_part.text) {
		initialize_message(equation, first_part.text, first_part.length);
		free_obj(&first_part);
	}
	if (middle_part.text) {
		initialize_message(equation, middle_part.text, middle_part.length);
		free_obj(&middle_part);
	}
	if (second_part.text) {
		initialize_message(equation, second_part.text, second_part.length);
		free_obj(&second_part);
	}
}

void
squeeze_by_operations (Message_t *equation, size_t *index_of_layer, char *operation, Message_t *left_operand, Message_t *right_operand) {
	Message_t label_of_layer;
	Message_t first_part;
	Message_t middle_part;
	Message_t second_part;
	size_t index;

	for (index = (size_t) 0; operations[index] != '\0'; index = index + (size_t) 1) {
		while ((make_expr_of_pare(*equation, &middle_part, &operations[index], left_operand, right_operand)) != (size_t) 0) {
			make_label_of_middle_part(&label_of_layer, *index_of_layer);
			if (get_side_parts(*equation, middle_part, &first_part, &second_part) == (size_t) 0) {
				return;
			}
			free_obj(&middle_part);
			free_obj(equation);
			declare_obj(equation);
			construct_equation(equation, label_of_layer, first_part, second_part);
			*index_of_layer = *index_of_layer + (size_t) 1;
			*operation = operations[index];
			return;
		}
	}
}

void
set_composition_to_flower (Flower_t *flower, Message_t *middle_part, size_t *index_of_layer, Variables_t *variables) {
	Message_t right_operand;
	Message_t left_operand;
	char operation;

	declare_obj(&left_operand);
	declare_obj(&right_operand);
	squeeze_by_operations(middle_part, index_of_layer, &operation, &left_operand, &right_operand);
	initialize_flower(flower, operation, left_operand, right_operand, variables);
	free_obj(&left_operand);
	free_obj(&right_operand);
}

void
prepare_graph (Flower_t *flower, Variables_t *variables) {
	size_t quantity_of_operations;
	size_t position;
	size_t size_of_first_part;
	size_t size_of_second_part;
	size_t index_of_layer;
	Message_t equation;
	Message_t first_part;
	Message_t middle_part;
	Message_t second_part;

	declare_obj(&equation);
	initialize_message(&equation, EQUATION, strlen(EQUATION));
	prepare_equation(&equation);
	printf("Modify equation:\n%s\n", equation.text);
	index_of_layer = (size_t) 0;

	while ((quantity_of_operations = get_quantity_of_operations(equation)) != (size_t) 0) {
		if (strstr(equation.text, "(") != NULL) {
			position = get_middle_part(equation, &middle_part);
			declare_obj(&first_part);
			initialize_message(&first_part, equation.text, position);
			declare_obj(&second_part);
			initialize_message(&second_part, equation.text + first_part.length + middle_part.length, equation.length - (first_part.length + middle_part.length));
			if ((quantity_of_operations = get_quantity_of_operations(middle_part)) == (size_t) 0) {
				size_of_first_part = first_part.length;
				size_of_second_part = second_part.length;
				free_obj(&first_part);
				free_obj(&second_part);
				if (size_of_first_part > (size_t) 0) {
					size_of_first_part = size_of_first_part - (size_t) 1;
				}
				if (size_of_second_part > (size_t) 0) {
					size_of_second_part = size_of_second_part - (size_t) 1;
				}
				position = position + middle_part.length + (size_t) 1;
				initialize_message(&first_part, equation.text, size_of_first_part);
				initialize_message(&second_part, equation.text + position, size_of_second_part);
			} else {
				set_composition_to_flower(flower, &middle_part, &index_of_layer, variables);
			}
			free_obj(&equation);
			declare_obj(&equation);
			construct_equation(&equation, middle_part, first_part, second_part);
		} else {
			set_composition_to_flower(flower, &equation, &index_of_layer, variables);
		}
	}
	free_obj(&equation);
}

void
clean_of_spaces (Message_t source, Message_t *clean_str) {
	Message_t space;

	declare_obj(&space);
	initialize_message(&space, " ", (size_t) 1);
	if (clean_str->text) {
		free_obj(clean_str);
		declare_obj(clean_str);
	}
	replace_message_in_message(source, space, clean_str);
	free_obj(&space);
}

void
clean_aliases (Message_t *source, char *text) {
	Message_t cln;

	declare_obj(source);
	initialize_message(source, text, strlen(text));
	declare_obj(&cln);
	clean_of_spaces(*source, &cln);
	free_obj(source);
	*source = cln;
}

void 
get_paths (Variables_t *variables) {
	size_t index;
	Message_t str;
	Message_t aliases;
	char *ptr;
	char *path;
	char *alias;

	declare_obj(&str);
	initialize_message(&str, PATHTOINITIALDATA, strlen(PATHTOINITIALDATA));
	ptr = str.text;
	while (path = strtok_r(ptr, ",", &ptr)) {
		if (alias = strtok_r(path, ":", &path)) {
			clean_aliases(&aliases, alias);
			for (index = (size_t) 0; index < variables->rows; index = index + (size_t) 1) {
				if (strncmp(variables->variable[index].alias.text, aliases.text, variables->variable[index].alias.length) == 0) {
					initialize_message(&variables->variable[index].path_to_inital_state, path, strlen(path));
				}
			}
			free_obj(&aliases);
		}
	}
	free_obj(&str);
	declare_obj(&str);
	initialize_message(&str, RESULTDATA, strlen(RESULTDATA));
	ptr = str.text;
	while (path = strtok_r(ptr, ",", &ptr)) {
		if (alias = strtok_r(path, ":", &path)) {
			clean_aliases(&aliases, alias);
			for (index = (size_t) 0; index < variables->rows; index = index + (size_t) 1) {
				if (strncmp(variables->variable[index].alias.text, "RESULTDATA", variables->variable[index].alias.length) == 0) {
					initialize_message(&variables->variable[index].path_to_inital_state, path, strlen(path));
					free_obj(&variables->variable[index].alias);
					initialize_message(&variables->variable[index].alias, aliases.text, aliases.length);
				}
			}
			free_obj(&aliases);
		}
	}
	free_obj(&str);
}


//