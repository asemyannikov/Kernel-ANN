#ifndef FILESYSTEM_LIBRARY
#define FILESYSTEM_LIBRARY

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include "./graph_parameters.h"
#include "./settings.h"

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum { FALSE, TRUE } Boolean_t;

/*  
  */
typedef struct variable {
	double value;
	double increment;
} variable;

/*  
  */
typedef struct vector {
	variable left_variable;
	variable right_variable;
} vector;

/*  
  */
typedef struct State {
	double *data;
	size_t length;
} State;

/*  
  */
typedef struct Learning_values {
	int *indices;
	size_t length;
} Learning_values;

extern double (*functions[QUANTITYOFBUDS])(vector);
extern int learning_variables_indices[QUANTITYOFLEARNINGVARIABLES];
extern char *paths_to_data[QUANTITYOFVARIABLES];

/* Внутри ANN модули ядра общаются друг с другом посредством сообщений типа 
Message_t. Тип Message_t, определенный в виде структуры, предназначен для 
хранения текста, а также длин произвольных строк. Такое определение строки 
сообщения потенциально должно сократить количество вызовов функций определяющих 
размеры текстовых строк, а также типологически определяет уникальную единицу 
сообщения. */
typedef struct Message {
	char *text;
	size_t length;
} Message_t;

void declare_message (Message_t *message);
void initialize_message (Message_t *message, char *text, size_t length);
void free_message (Message_t *message);

/* Множество сообщений типа Message_t хранятся структурах Messages_t. Такое
распределение информации позволяет разграничивать разнородные сообщения между
структурами и эффективно управлять данными между модулями ядра ANN. Структура
данных, которая соответствует модели типа Messages_t соответствует списку. */
typedef struct Messages {
	Message_t *message;
	size_t rows;
} Messages_t;

void declare_messages (Messages_t *messages);
void initialize_messages (Messages_t *messages, Message_t message);
void free_messages (Messages_t *messages);

/* В коде используются простейшие дженерик-макросы, определяющие и заносящие 
блоки памяти из под объектов в списки повторно используемых областей. Этот шаг 
был выполнен с целью унификации исходного кода. */
#define declare_obj(A)				\
	_Generic(A,						\
	Message_t  *: declare_message,	\
	Messages_t *: declare_messages	\
) (A)

#define free_obj(A)					\
	_Generic(A,						\
	Message_t  *: free_message,		\
	Messages_t *: free_messages		\
) (A)

/*   */
void Random_choise(int32_t *result, size_t len, int32_t rand_min, int32_t rand_max);
/*   */
void set_initial_states (State state[QUANTITYOFVARIABLES]);
void set_minibatch (State state[QUANTITYOFVARIABLES], size_t *dimension);
void free_state (State *state);
/*   */
Message_t get_explanation_of_error (int errnum);
void print_error_message (const char *buf, ...);
int replace_message_in_message (Message_t dest, Message_t repl, Message_t *with);

int Open (const char *pathname, int flags, mode_t mode);
ssize_t Read (int fd, char *buf, size_t len);
ssize_t Write (int fd, char *buf, size_t len);
ssize_t Lseek (int fd, off_t offset, int whence);
int Close (int fd);
int get_size_of_file (int fd, ssize_t *length);
void get_data (const char *path, Message_t *message);

void * Calloc (size_t nmemb, size_t size);
void * Malloc (size_t size);
void * Realloc (void *ptr, size_t size);

void init_vector (vector *point, variable left_variable, variable right_variable);
size_t check_dimension_of_input_data_vectors (State state[QUANTITYOFVARIABLES]);
void Gradient_descent_optimization_algorithm (double (*function[QUANTITYOFBUDS])(vector), State state[QUANTITYOFVARIABLES], size_t dimension, Learning_values learning_values, double learning_rate, double result_vector[]);
void init_learning_values (Learning_values *learning_values);
void add_index_of_learning_variable_to_list (Learning_values *learning_values, int index);
void add_data_to_learning_values (Learning_values *learning_values, int indices[], size_t lenght);
void free_learning_values (Learning_values *learning_values);

void init_state (State *state);
void add_record_to_state (State *state, double data);
void add_data_to_state (State *state, double data[], size_t lenght);
void free_state (State *state);
void set_initial_states (State state[QUANTITYOFVARIABLES]);
void set_minibatch (State state[QUANTITYOFVARIABLES], size_t *dimension);
void print_result (double *result);
/*   */
void init_buds (vector point[QUANTITYOFBUDS], variable buds[QUANTITYOFBUDS], variable variables[QUANTITYOFVARIABLES], int index);
#endif