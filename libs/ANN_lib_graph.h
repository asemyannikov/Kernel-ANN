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
#include "./settings.h"

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum { FALSE, TRUE } Boolean_t;

extern char *functions[];

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
void initialize_message (Message_t *message, char *text, size_t lenght);
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

/* При чтении данных из файла конфигурации требуется определить какие из 
переменных уравнения будут относиться к дифференцируемым и дифференцирующим
классам. Чтобы хранить эту информацию, был разработан тип Variable_t, который
имеет поля псевдонима переменной, пути к файлу с ее начальным распределением, а
также уникального идентификатора в графе, который определяет соответсвие между
двумя классами переменных и их значениями. */
typedef struct Variable {
	Message_t alias;
	Message_t path_to_inital_state;
	ssize_t index_in_flower;
	Boolean_t learning;
	Boolean_t result;
} Variable_t;

void declare_variable (Variable_t *variable);
void initialize_variable (Variable_t *variable, Message_t alias, Message_t path_to_inital_state, ssize_t index_in_flower, Boolean_t learning, Boolean_t result);
void free_variable (Variable_t *variable);

/* Переменные группируются в списки Vatiables_t. Такое распределение позволяет
однозначно закрепить уникальный набор переменных, их начальных распределений,
а также идентификаторов за одним уравнением. */
typedef struct Variables {
	Variable_t *variable;
	size_t rows;
} Variables_t;

void declare_variables (Variables_t *variables);
void initialize_variables (Variables_t *variables, Variable_t variable);
void free_variables (Variables_t *variables);

/* Модель хранения данных в ядре ANN разработана в виде модифицированного 
ориентированного графа, являющегося расширением бинарных деревьев. Подробная 
информация по графу представлена в соотвествующем приложении к данному 
исходному коду [XXXX]. Каждый бутон-узел типа Bud_t способен хранить с два 
операнда, представленных в виде сообщений Message_t, операции между ними, а 
также указатели на следующий узел в графе и на тот, что хранится в одном из 
'лепестков' узла: лепесток бутона может хранить как простое сообщение, так и 
указатель на источник информации в виде другого бутона. В качестве пояснения, 
приведем иллюстрацию N-го бутона, хранящего операцию сложений двух операндов 
A и B.

    ╔════╩════╗
    ║   {N}   ║
    ╠═══╦═╦═══╣
    ║ A ║+║ B ║
    ╚═══╩═╩═══╝

А также пример графа-цветка для уравнения f = sin(x*k+b):

      ╔════════════╗
      ║ sin(x*k+b) ║
      ╚══════╦═════╝
             ║
             ║
             ║
        ╔════╩════╗
        ║   {3}   ║
        ╠═══╦═╦═══╣
        ║sin║:║{2}║
        ╚═══╩═╩═╦═╝
                ║
                ╚═════════════╗
                              ║
                         ╔════╩════╗
                         ║   {2}   ║
                         ╠═══╦═╦═══╣
                         ║{1}║+║ b ║
                         ╚═╦═╩═╩═══╝
                           ║
         ╔═════════════════╝
         ║
    ╔════╩════╗
    ║   {1}   ║
    ╠═══╦═╦═══╣
    ║ x ║*║ k ║
    ╚═══╩═╩═══╝
*/

typedef enum { LEFT, RIGHT, QUANTITY_OF_PETALS } Quantity_of_petals_t;
typedef enum { PREV, NEXT, QUANTITY_OF_NEIGHBOURS } Quantity_of_neighbours_t;

typedef struct Petal {
	Message_t operand;
	struct Bud *source;
} Petal_t;

void declare_petal (Petal_t *petal);
void free_petal (Petal_t *petal);

typedef struct Bud {
	Boolean_t visited;
	Message_t label;
	char operation;
	struct Bud *neighbours[QUANTITY_OF_NEIGHBOURS];
	Petal_t petals[QUANTITY_OF_PETALS];
} Bud_t;

void declare_bud (Bud_t *bud);
void free_bud (Bud_t *bud);

typedef struct Flower {
	size_t number_of_buds;
	size_t number_of_variables;
	Bud_t *root;
} Flower_t;

void declare_flower (Flower_t *flower);
void initialize_petal (Flower_t *flower, Petal_t *petal, Message_t operand, Variables_t *variables);
void initialize_bud (Flower_t *flower, Bud_t *bud, char operation, Message_t left_operand, Message_t right_operand, Variables_t *variables);
void initialize_flower (Flower_t *flower, char operation, Message_t left_operand, Message_t right_operand, Variables_t *variables);
void free_flower (Flower_t *flower);
void traversal_info_about_flower (Flower_t *flower);
void reset_traversal (Flower_t *flower);

/* В коде используются простейшие дженерик-макросы, определяющие и заносящие 
блоки памяти из под объектов в списки повторно используемых областей. Этот шаг 
был выполнен с целью унификации исходного кода. */
#define declare_obj(A)					\
	_Generic(A,							\
	Message_t   *: declare_message,		\
	Messages_t  *: declare_messages,	\
	Variable_t  *: declare_variable,	\
	Variables_t *: declare_variables,	\
	Petal_t     *: declare_petal,		\
	Bud_t       *: declare_bud,			\
	Flower_t    *: declare_flower		\
) (A)

#define free_obj(A)					\
	_Generic(A,						\
	Message_t   *: free_message,	\
	Messages_t  *: free_messages,	\
	Variable_t  *: free_variable,	\
	Variables_t *: free_variables,	\
	Petal_t     *: free_petal,		\
	Bud_t       *: free_bud,		\
	Flower_t    *: free_flower		\
) (A)

/*   */
size_t prepare_equation (Message_t *result);
void prepare_graph (Flower_t *flower, Variables_t *variables);
void get_paths (Variables_t *variables);
/*   */
int create_dynamic_library (Flower_t *flower, Variables_t variables);
/*   */
int Strerror (int errnum, Message_t message);
Message_t get_explanation_of_error (int errnum);
void print_error_message (const char *buf, ...);
int replace_message_in_message (Message_t dest, Message_t repl, Message_t *with);
/*   */
int Open (const char *pathname, int flags, mode_t mode);
ssize_t Write (int fd, char *buf, size_t len);
int Close (int fd);
Boolean_t is_number (Message_t *message);
Boolean_t is_function (Message_t *message);
void set_number (Message_t *variable, size_t number);
/*   */
void * Calloc (size_t nmemb, size_t size);
void * Malloc (size_t size);
void * Realloc (void *ptr, size_t size);

int run_mkmachine();
#endif