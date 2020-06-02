#include "./ANN_lib_settings.h"

#ifdef PATH_TO_MAP_FILE
#undef PATH_TO_MAP_FILE
#endif
#define PATH_TO_MAP_FILE "./libs/map.cfg"

#ifdef PATH_TO_CONFIGURE_FILE
#undef PATH_TO_CONFIGURE_FILE
#endif
#define PATH_TO_CONFIGURE_FILE "./settings.cfg"

#ifdef PATH_TO_SETTINGS_LIBRARY
#undef PATH_TO_SETTINGS_LIBRARY
#endif
#define PATH_TO_SETTINGS_LIBRARY "./libs/settings.h"

#ifdef LEAST_SQUARES_METHOD
#undef LEAST_SQUARES_METHOD
#endif
#define LEAST_SQUARES_METHOD "(#-RESULTDATA)^2"

typedef enum { ALIAS_COLUMN, DEFINITIVE_COLUMN, TYPE_COLUMN, VALUE_COLUMN, NUMBER_OF_COLUMNS_IN_A_SETTINGS_TABLE } Columns_of_settings_table_t;

void replace_sharp (char *str_method, Message_t *clean_str);
void clean_of_spaces (Message_t dest, Message_t *clean_str);
/* Одна удельная настройка состоит из четырех служебных полей. Таблица полей 
представлена ниже:

ИДЕНТИФИКАТОР	СЛУЖЕБНОЕ НАИМЕНОВАНИЕ	ПОЯСНЕНИЕ
0				alias					псевдоним настройки
1				definitive				закрепленный за настройкой исходный код
2				type					тип настройки: числовой или строковый
3				value					значение

Файл mapping.cfg хранит зарезервированную настройку для каждого псевдонима из 
файла settings.cfg. Изменение структуры mapping.cfg является критичным 
вмешательством и не гарантирует в дальнейшем корректную работу ядра ANN. */

void 
declare_setting (Setting_t *setting) {
	declare_messages(&setting->customs);
}

void 
initialize_setting (Setting_t *setting, Messages_t messages) {
	size_t index;

	if (messages.rows != NUMBER_OF_COLUMNS_IN_A_SETTINGS_TABLE) {
		print_error_message("[ERROR MESSAGE] initialize_setting() : A mismatch in input dimensions from configure file.\n");
		exit(EXIT_FAILURE);
	} else {
		setting->customs = messages;
	}
}

void 
free_setting (Setting_t *setting) {
	free_messages(&setting->customs);
}

void
read_map (Settings_t *settings) {
	size_t index;
	char *str, *ptr;
	Message_t message_from_map_file;
	Message_t customs[NUMBER_OF_COLUMNS_IN_A_SETTINGS_TABLE];
	Messages_t messages;
	Setting_t setting;

	get_data(PATH_TO_MAP_FILE, &message_from_map_file);
	declare_obj(settings);
	ptr = message_from_map_file.text;
	index = (size_t) 0;
	while (str = strtok_r(ptr, "-", &ptr)) {
		declare_obj(&customs[index]);
		initialize_message(&customs[index], str, strnlen(str, message_from_map_file.length));
		index = index + (size_t) 1;
		if (index == VALUE_COLUMN) {
			declare_obj(&messages);
			initialize_messages(&messages, customs[ALIAS_COLUMN]);
			initialize_messages(&messages, customs[DEFINITIVE_COLUMN]);
			initialize_messages(&messages, customs[TYPE_COLUMN]);
			declare_obj(&customs[VALUE_COLUMN]);
			initialize_messages(&messages, customs[VALUE_COLUMN]);
			declare_obj(&setting);
			initialize_setting(&setting, messages);
			initialize_settings(settings, setting);
			index = (size_t) 0;
		}
	}
	free_obj(&message_from_map_file);
}

void
insert_values_into_settings (Settings_t *settings) {
	size_t index;
	char *str, *ptr, *end_of_str;
	size_t lenght_of_value;
	Message_t message_from_configure_file;
	Message_t value;
	Message_t tmp;

	get_data(PATH_TO_CONFIGURE_FILE, &message_from_configure_file);
	ptr = message_from_configure_file.text;
	for (index = (size_t) 0; index < settings->rows; index = index + (size_t) 1) {
		if (str = strstr(ptr, settings->setting[index].customs.message[ALIAS_COLUMN].text)) {
			if (end_of_str = strstr(str, ";")) {
				lenght_of_value = strnlen(str, message_from_configure_file.length) - strnlen(end_of_str, message_from_configure_file.length) - settings->setting[index].customs.message[ALIAS_COLUMN].length - 1;
				declare_obj(&settings->setting[index].customs.message[VALUE_COLUMN]);
				if (strncmp(settings->setting[index].customs.message[TYPE_COLUMN].text, "s", (size_t) 1) == (ssize_t) 0) {
					declare_obj(&value);
					if (strncmp(settings->setting[index].customs.message[ALIAS_COLUMN].text, "PATHTOINITIALDATA", (size_t) 17) != (ssize_t) 0) {
						declare_obj(&tmp);
						initialize_message(&tmp, str + settings->setting[index].customs.message[ALIAS_COLUMN].length + 1, lenght_of_value);
						if (strncmp(settings->setting[index].customs.message[ALIAS_COLUMN].text, "EQUATION", (size_t) 8) == (ssize_t) 0) {
							replace_sharp(LEAST_SQUARES_METHOD, &tmp);
						}
						clean_of_spaces(tmp, &value);
						free_obj(&tmp);
					} else {
						initialize_message(&value, str + settings->setting[index].customs.message[ALIAS_COLUMN].length + 1, lenght_of_value);
					}
					initialize_message(&settings->setting[index].customs.message[VALUE_COLUMN], "\"", (size_t) 1);
					initialize_message(&settings->setting[index].customs.message[VALUE_COLUMN], value.text, value.length);
					initialize_message(&settings->setting[index].customs.message[VALUE_COLUMN], "\"", (size_t) 1);
					free_obj(&value);
				} else {
					initialize_message(&settings->setting[index].customs.message[VALUE_COLUMN], str + settings->setting[index].customs.message[ALIAS_COLUMN].length + 1, lenght_of_value);
				}
			}
		}
	}
	free_obj(&message_from_configure_file);
}

void
get_settings (Settings_t *settings) {
	read_map(settings);
	insert_values_into_settings(settings);
}

void
set_settings (Settings_t *settings) {
	int fd;
	size_t index;
	Message_t message;

	if((fd = Open(PATH_TO_SETTINGS_LIBRARY, O_CREAT | O_WRONLY | O_TRUNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH)) != -1) {
		declare_obj(&message);
		for (index = (size_t) 0; index < settings->rows; index = index + (size_t) 1) {
			initialize_message(&message, settings->setting[index].customs.message[DEFINITIVE_COLUMN].text, settings->setting[index].customs.message[DEFINITIVE_COLUMN].length);
			initialize_message(&message, " ", (size_t) 1);
			initialize_message(&message, settings->setting[index].customs.message[VALUE_COLUMN].text, settings->setting[index].customs.message[VALUE_COLUMN].length);
			initialize_message(&message, "\n", (size_t) 1);
		}
		if (Write(fd, message.text, message.length) == (ssize_t) -1) {
			print_error_message("[ERROR MESSAGE] set_settings() : Cannot write into settings.h file.\n");
			exit(EXIT_FAILURE);
		}
		if (Close(fd) == -1) {
			print_error_message("[ERROR MESSAGE] set_settings() : Cannot close settings.h file.\n");
			exit(EXIT_FAILURE);
		}
		free_obj(&message);
	} else {
		print_error_message("[ERROR MESSAGE] set_settings() : Cannot to open settings.h file.\n");
		exit(EXIT_FAILURE);
	}
}

void
replace_sharp (char *str_method, Message_t *clean_str) {
	Message_t sharp;
	Message_t method;

	declare_obj(&method);
	initialize_message(&method, str_method, strlen(str_method));
	declare_obj(&sharp);
	initialize_message(&sharp, "#", (size_t) 1);
	replace_message_in_message(method, sharp, clean_str);
	free_obj(&sharp);
	free_obj(&method);
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