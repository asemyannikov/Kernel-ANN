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

/* Удельная настройка характеризуется набором четырех сообщений - характеристик:
псевдонима, дефинитива, типа и значения. Объявление такой структуры преследует 
цель улучшить безопасность читаемых данных, а установка внутренних проверок 
обеспечивает формирование библиотеки settings.h, необходимой для процессов 
формирования графа и дифференцирующей машины.*/
typedef struct Setting {
	Messages_t customs;
} Setting_t;

void declare_setting (Setting_t *setting);
void initialize_setting (Setting_t *setting, Messages_t messages);
void free_setting (Setting_t *setting);

/* Настройки, получаемые из файла settings.cfg группируются в специальные 
структуры Setting_t, а те, в свою очередь, образуют кластеры данных типа 
Settings_t. Такая разгруппировка обеспечивает быстрый доступ к настройке 
выбранного файла, закрепляя за ним уникальную область памяти, а также 
оптимизирует процесс управления данными в процессе формирования библиотеки 
settings.h, очистки данных процесса, проверки целостности входных параметров. */
typedef struct Settings {
	Setting_t *setting;
	size_t rows;
} Settings_t;

void declare_settings (Settings_t *settings);
void initialize_settings (Settings_t *settings, Setting_t setting);
void free_settings (Settings_t *settings);
/* Следующие четыре процедуры реализованы в файле ANN_str_setting.c. Однако, 
так как логически эти параметры относятся к более целым кластерам настроек, мы 
размещаем их объявление ниже блока Setting_t, перенося их в блок объявлений типа
Settings_t. 

Процедура read_map необходима для чтения специального файла настроек ядра ANN
map.cfg. Редактирование этого файла влечет за собой неопределенные изменения в 
процессе чтения мэппигна и формирования файла библиотеки settings.h. 

Когда таблица настроек сформируется после прочтения мэппинга, каждой настройке
по псевдониму будет сопоставлено значение из файла settings.cfg с помощью 
функции insert_values_into_settings. 

Функция get_settings комбинирует пару процедур read_map и 
insert_values_into_settings, заполняя таблицу settings, передаваемую в аргрумент 
функции по указателю.

Функция set_settings формирует библиотеку settings.h по данным, размещенным в 
таблице settings. */
void read_map (Settings_t *settings);
void insert_values_into_settings (Settings_t *settings);
void get_settings (Settings_t *settings);
void set_settings (Settings_t *settings);

/* В коде используются простейшие дженерик-макросы, определяющие и заносящие 
блоки памяти из под объектов в списки повторно используемых областей. Этот шаг 
был выполнен с целью унификации исходного кода. */
#define declare_obj(A)				\
	_Generic(A,						\
	Message_t  *: declare_message,	\
	Messages_t *: declare_messages,	\
	Setting_t  *: declare_setting,	\
	Settings_t *: declare_settings	\
) (A)

#define free_obj(A)					\
	_Generic(A,						\
	Message_t  *: free_message,		\
	Messages_t *: free_messages,	\
	Setting_t  *: free_setting,		\
	Settings_t *: free_settings		\
) (A)

/*   */
Message_t get_explanation_of_error (int errnum);
void print_error_message (const char *buf, ...);
int replace_message_in_message (Message_t dest, Message_t repl, Message_t *with);

int Open (const char *pathname, int flags, mode_t mode);
ssize_t Read (int fd, char *buf, size_t len);
ssize_t Write (int fd, char *buf, size_t len);
ssize_t Lseek (int fd, off_t offset, int whence);
int Close (int fd);
int get_size_of_file (int fd, ssize_t *lenght);
void get_data (const char *path, Message_t *message);

void * Calloc (size_t nmemb, size_t size);
void * Malloc (size_t size);
void * Realloc (void *ptr, size_t size);

int run_mkgraph();
#endif