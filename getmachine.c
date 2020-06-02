#include "./libs/ANN_lib_machine.h"

int 
main(int argc, char *argv[]) {
/*[BLOCK: Объявление переменных]--------------------------------------------->*/

	size_t index;
	size_t dimension;
	double *result;
	Learning_values learning_values;
	State state[QUANTITYOFVARIABLES];

/*<---------------------------------------------[BLOCK: Объявление переменных]*/
/*[BLOCK: Выбор обучаемых переменных]---------------------------------------->*/

	init_learning_values(&learning_values);
	for (index = (size_t) 0; index < QUANTITYOFLEARNINGVARIABLES; index = index + (size_t) 1) {
		add_index_of_learning_variable_to_list(&learning_values, learning_variables_indices[index]);
	}
	result = Calloc(learning_values.length, sizeof(double));

/*<----------------------------------------[BLOCK: Выбор обучаемых переменных]*/
/*[BLOCK: Установка начального распределения]-------------------------------->*/

	set_initial_states(state);

/*<--------------------------------[BLOCK: Установка начального распределения]*/
/*[BLOCK: Установка минибатча]----------------------------------------------->*/

	dimension = check_dimension_of_input_data_vectors(state);
	set_minibatch(state, &dimension);

/*<-----------------------------------------------[BLOCK: Установка минибатча]*/
/*[BLOCK: Градиентный спуск]------------------------------------------------->*/

	Gradient_descent_optimization_algorithm(functions, state, dimension, learning_values, LEARNINGRATE, result);

/*<-------------------------------------------------[BLOCK: Градиентный спуск]*/
/*[BLOCK: Простое представление данных]-------------------------------------->*/

	print_result(result);

/*<--------------------------------------[BLOCK: Простое представление данных]*/
/*[BLOCK: Освобождение памяти]----------------------------------------------->*/

	free(result);
	for (index = (size_t) 0; index < QUANTITYOFVARIABLES; index = index + (size_t) 1) {
		free_state(&state[index]);
	}
	free_learning_values(&learning_values);

/*<-----------------------------------------------[BLOCK: Освобождение памяти]*/
	exit(EXIT_SUCCESS);
}