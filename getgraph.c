#include "./libs/ANN_lib_graph.h"

int
main (int argc, char *argv[]) {
	Flower_t flower;
	Variables_t variables;

	declare_obj(&flower);
	declare_obj(&variables);

/*[BLOCK: Запуск формирования графа-цветка]---------------------------------->*/

	prepare_graph(&flower, &variables);

/*<----------------------------------[BLOCK: Запуск формирования графа-цветка]*/

	printf("Traversal flower:\n");
	traversal_info_about_flower(&flower);
	get_paths(&variables);
	printf("Variables table:\n");
	for (int i = 0; i < variables.rows; i++)
	{
		printf("name: %s; path: %s; indx: %ld; lrng: %d; rslt: %d;\n", 
			variables.variable[i].alias.text, 
			variables.variable[i].path_to_inital_state.text,
			variables.variable[i].index_in_flower,
			variables.variable[i].learning,
			variables.variable[i].result);
	}

/*[BLOCK: Создание динамической библиотеки]---------------------------------->*/

	if (create_dynamic_library(&flower, variables) == -1) {
		free_obj(&flower);
		_exit(EXIT_FAILURE);
	}

/*<----------------------------------[BLOCK: Создание динамической библиотеки]*/
/*[BLOCK: Освобождение памяти]----------------------------------------------->*/

	free_obj(&variables);
	free_obj(&flower);

/*<-----------------------------------------------[BLOCK: Освобождение памяти]*/
/*[BLOCK: Запуск машины дифференцирования]----------------------------------->*/
	
	printf("Flower is ready. Do you want to start a machine? [y/n]\n");
	if (getchar() == 'y') {
		run_mkmachine();
	}

/*<-----------------------------------[BLOCK: Запуск машины дифференцирования]*/
	exit(EXIT_SUCCESS);
}