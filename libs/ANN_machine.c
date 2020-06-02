#include "./ANN_lib_machine.h"

void
init_vector (vector *point, variable left_variable, variable right_variable) {
	point->left_variable  = left_variable;
	point->right_variable = right_variable;
}

/*
	Файл <>
	
	double get_increment (double (*function)(vector), vector point)
	Параметры:
	double (*function)(vector): вектор функций, хранимых в узлах графа.
	vector point: вектор переменных, хранимых в лепестках графа.
	Функция производит вычисление симметричной разности на трех узлах в N-мерном
	пространстве для вектора переменной point относительно вектора функций, 
	на который указывает переменная *function.
	Так как каждый узел в графе характеризуется парой операндов, каждый из 
	которых может не быть константой, то приращение в двумерном пространстве 
	бутона должно выполняться последовательно, с сохранением порядка. К примеру,
	мы имеем произвольную функцию f(x, y), зависящую от пары переменных x и y.
	Тогда приращение функции в точке (x_0, y_0) имеет следующий простой вид

	delta f(x_0, y_0) = f(x_0 + delta x, y_0) + f(x_0, y_0 + delta y) - 2 * f(x_0, y_0),
	delta f(x_0, y_0) = f(x_0 + delta x, y_0 + delta y) + f(x_0, y_0),

	где delta x, delta y - соответствующие смещения по переменным x и y.

	double void initial_condition (variable variables[QUANTITYOFVARIABLES], double initial_state[QUANTITYOFVARIABLES])
	Параметры:
	variable variables[QUANTITYOFVARIABLES]: буффер переменных.
	double initial_state[QUANTITYOFVARIABLES]: вектор начального состояния сети, 
	который подается на вход. Переменные и функции N-мерного пространства 
	генерируются автоматически, и размещаются в отдельном временном файле. 
	QUANTITYOFVARIABLES - константа, характеризующая количество считанных 
	переменных по графу. Чтобы заполнить начальными значениями вектор координат,
	используется цикл, в котором значения из initial_state помещаются в 
	variables.value, а смещения по у данных переменных инициализируются нулями.

	int differentiation (double (*function[QUANTITYOFBUDS])(vector), double initial_state[QUANTITYOFVARIABLES], double final_state[QUANTITYOFVARIABLES], const double accuracy)
	void *Differentiation (double (*function[QUANTITYOFBUDS])(vector), double initial_state[QUANTITYOFVARIABLES], double final_state[QUANTITYOFVARIABLES], const double accuracy)
	Differentiation - процедура обёртка функции differentiation.
	Параметры:
	double (*function[QUANTITYOFBUDS])(vector): массив функций, сформированных графом вычислений.
	double initial_state[QUANTITYOFVARIABLES]: вектор начального состояния переменных, хранимых в лепестках графа.
	double final_state[QUANTITYOFVARIABLES]: вектор конечного состояния.
	const double accuracy: точность вычисления.
	Функция возвращает в случае ошибки отрицательное число -1. В противном случае,
	производится вычисление производных от функций, хранимых в узлах графа-цветка
	по методу центральных конечных разностей с точностью accuracy.
*/

size_t
check_dimension_of_input_data_vectors (State state[QUANTITYOFVARIABLES]) {
	size_t index;
	size_t dimension;

	dimension = (size_t) 1;
	for (index = (size_t) 0; index < QUANTITYOFVARIABLES; index = index + (size_t) 1) {
		if (dimension < state[index].length) {
			if (dimension == (size_t) 1) {
				dimension = state[index].length;
			} else {
				print_error_message("[ERROR MESSAGE] check_dimension_of_input_data_vectors() : A mismatch in the dimension input data vectors!\n");
				_exit(EXIT_FAILURE);
			}
		}
	}
	return dimension;
}

double
get_increment (
	double (*function)(vector),
	vector point) {
	vector groupVectors[2] = {
		{{point.left_variable.value + point.left_variable.increment, }, {point.right_variable.value + point.right_variable.increment, }},
		{{point.left_variable.value, }, {point.right_variable.value, }}
	};
	return (*function)(groupVectors[0]) - (*function)(groupVectors[1]);
}

void
initial_condition (
	variable variables[QUANTITYOFVARIABLES], 
	State state[QUANTITYOFVARIABLES],
	int condition_of_variable_index,
	int condition_index) {
	int variable_index;

	if (state[condition_of_variable_index].length == 1) {
		variables[condition_of_variable_index].value = state[condition_of_variable_index].data[0];
	} else {
		variables[condition_of_variable_index].value = state[condition_of_variable_index].data[condition_index];
	}
	variables[condition_of_variable_index].increment = 0.0;
}

Boolean_t
find_index (int variable_index, Learning_values learning_values) {
	size_t index;

	for (index = (size_t) 0; index < learning_values.length; index = index + (size_t) 1) {
		if (variable_index == learning_values.indices[index]) {
			return TRUE;
		}
	}
	return FALSE;
}

int
differentiation (
		double (*function[QUANTITYOFBUDS])(vector),
		State state[QUANTITYOFVARIABLES],
		size_t dimension,
		Learning_values learning_values,
		double learning_rate,
		double result_vector[]) {
	vector point[QUANTITYOFBUDS];
	variable buds[QUANTITYOFBUDS];
	variable variables[QUANTITYOFVARIABLES];
	double gradient[learning_values.length];
	int bud_index;
	int variable_index;
	int value_index;
	int condition_index;
	int learning_variable_index;
	int condition_of_variable_index;
	int epoch;


	for (epoch = 0; epoch < LEARNINGEPOCHS; epoch++) {
		for (learning_variable_index = 0; learning_variable_index < learning_values.length; learning_variable_index++) {
			gradient[learning_variable_index] = 0.0;
			for (condition_index = 0; condition_index < dimension; condition_index = condition_index + 1) {
				variable_index = learning_values.indices[learning_variable_index];
				for (condition_of_variable_index = 0; condition_of_variable_index < QUANTITYOFVARIABLES; condition_of_variable_index = condition_of_variable_index + 1) {
					if (variable_index != condition_of_variable_index) {
						initial_condition(variables, state, condition_of_variable_index, condition_index);
					}
				}

				variables[variable_index].increment = DIFFERENTIATIONACCURACY;
				variables[variable_index].value = state[variable_index].data[0];
				init_buds(point, buds, variables, 0);

				for (bud_index = 0; bud_index < QUANTITYOFBUDS; bud_index = bud_index + 1) {
					buds[bud_index].increment = get_increment(*function[bud_index], point[bud_index]);
					if (bud_index < QUANTITYOFBUDS - 1) {
						buds[bud_index].value = (*function[bud_index])(point[bud_index]);
						init_buds(point, buds, variables, bud_index + 1);
					}
				}

				gradient[learning_variable_index] += (buds[QUANTITYOFBUDS - 1].increment) / (variables[variable_index].increment);
				variables[variable_index].value += variables[variable_index].increment;
				init_buds(point, buds, variables, 0);
			}
			
		}
		for (learning_variable_index = 0; learning_variable_index < learning_values.length; learning_variable_index++) {
			variable_index = learning_values.indices[learning_variable_index];
			state[variable_index].data[0] = state[variable_index].data[0] - learning_rate * gradient[learning_variable_index];
		}
	}
	for (learning_variable_index = 0; learning_variable_index < learning_values.length; learning_variable_index++) {
		variable_index = learning_values.indices[learning_variable_index];
		result_vector[learning_variable_index] = state[variable_index].data[0];
	}
	return 0;
}

void
Differentiation (
		double (*function[QUANTITYOFBUDS])(vector),
		State state[QUANTITYOFVARIABLES],
		size_t dimension,
		Learning_values learning_values,
		double learning_rate, 
		double result_vector[]) {
	int errno_saved;
	
	errno_saved = errno;
	errno = 0;
	if (differentiation(function, state, dimension, learning_values, learning_rate, result_vector) != 0) {
		print_error_message("[ERROR MESSAGE] Differentiation() :  \n");
		errno = errno_saved;
		_exit(EXIT_FAILURE);
	}
}

void
Gradient_descent_optimization_algorithm (
		double (*function[QUANTITYOFBUDS])(vector),
		State state[QUANTITYOFVARIABLES],
		size_t dimension,
		Learning_values learning_values,
		double learning_rate, 
		double result_vector[]) {
	Differentiation(functions, state, dimension, learning_values, learning_rate, result_vector);
}

void
init_learning_values (Learning_values *learning_values) {
	learning_values->indices = NULL;
	learning_values->length = (size_t) 0;
}

void
add_index_of_learning_variable_to_list (Learning_values *learning_values, int index) {
	if (learning_values->length > 0) {
		learning_values->indices = Realloc(learning_values->indices, (learning_values->length + (size_t) 1) * sizeof(int));
	} else {
		learning_values->indices = Calloc((size_t) 1, sizeof(int));
	}
	learning_values->indices[learning_values->length] = index;
	learning_values->length = learning_values->length + (size_t) 1;
}

void
add_data_to_learning_values (Learning_values *learning_values, int indices[], size_t length) {
	size_t index;
	size_t new_length;

	new_length = learning_values->length + length;
	if (learning_values->length > 0) {
		learning_values->indices = Realloc(learning_values->indices, new_length * sizeof(int));
	} else {
		learning_values->indices = Calloc(length, sizeof(int));
	}
	for (index = learning_values->length; index < new_length; index = index + (size_t) 1) {
		learning_values->indices[index] = indices[index - learning_values->length];
	}
	learning_values->length = new_length;
}

void free_learning_values (Learning_values *learning_values) {
	free(learning_values->indices);
	init_learning_values(learning_values);
}

void
print_result (double *result) {
	printf("Result:\n");
	int learning_variable_index = 0;
	char str[] = LEARNINGVARIABLES;
	char * pch = strtok(str, ",");
	while (pch != NULL) {
		printf("variable: \"%s\" = %.6f\n", pch, result[learning_variable_index]);
		pch = strtok (NULL, ",");
		learning_variable_index++;
	}
}