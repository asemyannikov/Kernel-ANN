#include "./ANN_lib_graph.h"

void make_label_of_bud (Flower_t *flower);
void make_label_of_variable (Message_t *variable, size_t number);
void make_links_to_source_in_petals (Flower_t *flower);

void
declare_petal (Petal_t *petal) {
	declare_obj(&petal->operand);
	petal->source = NULL;
}

void
initialize_petal (Flower_t *flower, Petal_t *petal, Message_t operand, Variables_t *variables) {
	size_t index;
	size_t index_in_flower;
	Variable_t variable;

	index_in_flower = flower->number_of_variables;
	if ((is_function(&operand) == FALSE) && (is_number(&operand) == FALSE) && (strstr(operand.text, "buds") == NULL)) {
		if (strstr(operand.text, "#NEGATIVE#") == NULL) {
			for (index = (size_t) 0; index < variables->rows; index = index + (size_t) 1) {
				if (strncmp(variables->variable[index].alias.text, operand.text, variables->variable[index].alias.length) == 0) {
					index_in_flower = index;
					break;
				}
			}
			make_label_of_variable(&petal->operand, index_in_flower);
			if (index_in_flower == flower->number_of_variables) {
				declare_obj(&variable);
				initialize_message(&variable.alias, operand.text, operand.length);
				variable.index_in_flower = index_in_flower;
				if (strncmp(operand.text, "RESULTDATA", operand.length) == 0) {
					variable.result = TRUE;
				} else {
					variable.result = FALSE;
				}
				if (strstr(LEARNINGVARIABLES, operand.text) != NULL) {
					variable.learning = TRUE;
				} else {
					variable.learning = FALSE;
				}
				initialize_variables(variables, variable);
				flower->number_of_variables = flower->number_of_variables + (size_t) 1;
			}
		} else {
			initialize_message(&petal->operand, "(-1)", (size_t) 4);
		}
	} else {
		initialize_message(&petal->operand, operand.text, operand.length);
	}
}

void
free_petal (Petal_t *petal) {
	free_obj(&petal->operand);
}

void
declare_bud (Bud_t *bud) {
	bud->visited = FALSE;
	declare_obj(&bud->label);
	bud->operation = '\0';
	bud->neighbours[PREV] = NULL;
	bud->neighbours[NEXT] = NULL;
	declare_obj(&bud->petals[LEFT]);
	declare_obj(&bud->petals[RIGHT]);
}

void
initialize_bud (Flower_t *flower, Bud_t *bud, char operation, Message_t left_operand, Message_t right_operand, Variables_t *variables) {
	bud->operation = operation;
	initialize_petal(flower, &bud->petals[LEFT], left_operand, variables);
	initialize_petal(flower, &bud->petals[RIGHT], right_operand, variables);
}

void
free_bud (Bud_t *bud) {
	free_obj(&bud->label);
	free_obj(&bud->petals[LEFT]);
	free_obj(&bud->petals[RIGHT]);
}

void
declare_flower (Flower_t *flower) {
	flower->number_of_buds = (size_t) 0;
	flower->number_of_variables = (size_t) 0;
	flower->root = NULL;
}

void
initialize_flower (Flower_t *flower, char operation, Message_t left_operand, Message_t right_operand, Variables_t *variables) {
	Bud_t *bud;
	
	bud = Calloc((size_t) 1, sizeof(Bud_t));
	declare_obj(bud);
	initialize_bud(flower, bud, operation, left_operand, right_operand, variables);
	if (flower->number_of_buds == (size_t) 0) {
		bud->neighbours[PREV] = NULL;
	} else {
		bud->neighbours[PREV] = flower->root;
	}
	bud->neighbours[NEXT] = bud->neighbours[PREV];
	flower->root = bud;
	flower->root->neighbours[NEXT] = NULL;
	make_label_of_bud(flower);
	make_links_to_source_in_petals(flower);
	flower->number_of_buds = flower->number_of_buds + (size_t) 1;
}

void
free_flower (Flower_t *flower) {
	Bud_t *ptr;
	Bud_t *prv;

	if (flower->number_of_buds > (size_t) 0) {
		ptr = flower->root;
		prv = ptr->neighbours[PREV];
		while (ptr != NULL) {
			free_obj(ptr);
			free(ptr);
			ptr = prv;
			if (prv != NULL) {
				prv = prv->neighbours[PREV];
			}
		}
		declare_obj(flower);
	}
}

void
make_links_to_source_in_petals (Flower_t *flower) {
	Bud_t *ptr;
	Bud_t *bud;

	if (flower->number_of_buds > (size_t) 0) {
		bud = flower->root;
		ptr = flower->root;
		while (bud != NULL) {
			if (strncmp(bud->label.text, ptr->petals[LEFT].operand.text, bud->label.length) == 0) {
				ptr->petals[LEFT].source = bud;
				bud->neighbours[NEXT] = ptr;
			}
			if (strncmp(bud->label.text, ptr->petals[RIGHT].operand.text, bud->label.length) == 0) {
				ptr->petals[RIGHT].source = bud;
				bud->neighbours[NEXT] = ptr;
			}
			bud = bud->neighbours[PREV];
		}
	}
}

void
make_label_of_bud (Flower_t *flower) {
	flower->root->label.length = snprintf(NULL, 0, "%ld", flower->number_of_buds) + (size_t) 6;
	flower->root->label.text = Calloc(flower->root->label.length + (size_t) 1, sizeof(char));
	snprintf(flower->root->label.text, flower->root->label.length + (size_t) 1, "buds[%ld]", flower->number_of_buds);
}

void
make_label_of_variable (Message_t *variable, size_t number) {
	variable->length = snprintf(NULL, 0, "%ld", number) + (size_t) 11;
	variable->text = Calloc(variable->length + (size_t) 1, sizeof(char));
	snprintf(variable->text, variable->length + (size_t) 1, "variables[%ld]", number);
}

void 
get_info_about_bud (Bud_t *bud) {
	if (bud->neighbours[NEXT] != NULL) {
		printf("BUD: {%s[%s|%c|%s]} -> %s\n", bud->label.text, bud->petals[LEFT].operand.text, bud->operation, bud->petals[RIGHT].operand.text, bud->neighbours[NEXT]->label.text);
	} else {
		printf("BUD: {%s[%s|%c|%s]} -> ROOT\n", bud->label.text, bud->petals[LEFT].operand.text, bud->operation, bud->petals[RIGHT].operand.text);
	}
	if (bud->petals[LEFT].operand.text != NULL) {
		printf("\tLEFT PETAL : %s\n", bud->petals[LEFT].operand.text);
	}
	if (bud->petals[RIGHT].operand.text != NULL) {
		printf("\tRIGHT PETAL: %s\n", bud->petals[RIGHT].operand.text);
	}
}

void 
get_info_about_flower (Flower_t *flower) {
	Bud_t *bud;
	
	if (flower->number_of_buds != (size_t) 0) {
		printf("Flower has %ld buds and %ld variables.\n", flower->number_of_buds, flower->number_of_variables);
		bud = flower->root;
		while (bud != NULL) {
			get_info_about_bud(bud);
			bud = bud->neighbours[PREV];
		}
	} else {
		printf("Flower is empty.\n");
	}
}

void 
traversal_info_about_flower (Flower_t *flower) {
	Bud_t *bud;

	bud = flower->root;
	while (bud != NULL) {
		if (bud->petals[RIGHT].source != NULL && bud->petals[RIGHT].source->visited == FALSE) {
			bud = bud->petals[RIGHT].source;
		} else if (bud->petals[LEFT].source != NULL && bud->petals[LEFT].source->visited == FALSE) {
			bud = bud->petals[LEFT].source;
		} else if (bud->visited == FALSE) {
			get_info_about_bud(bud);
			bud->visited = TRUE;
		} else {
			bud = bud->neighbours[NEXT];
		}
	}
	reset_traversal(flower);
	
}

void
reset_traversal (Flower_t *flower) {
	Bud_t *bud;
	
	bud = flower->root;
	while (bud != NULL) {
		bud->visited = FALSE;
		bud = bud->neighbours[PREV];
	}
}