#include "./ANN_lib_settings.h"

void *
Calloc (size_t nmemb, size_t size) {
	void *array;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	if ((array = calloc(nmemb, size)) == NULL) {
		errno = errno_saved;
		message = get_explanation_of_error(errno);
		print_error_message("[ERROR MESSAGE] calloc() [%d] : %s\n", errno, message.text);
		free_message(&message);
		_exit(EXIT_FAILURE);
	}
	return array;
}

void *
Malloc (size_t size) {
	void *array;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	if ((array = malloc(size)) == NULL) {
		errno = errno_saved;
		message = get_explanation_of_error(errno);
		print_error_message("[ERROR MESSAGE] malloc() [%d] : %s\n", errno, message.text);
		free_message(&message);
		_exit(EXIT_FAILURE);
	}
	return array;
}

void *
Realloc (void *ptr, size_t size) {
	void *array;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	if ((array = realloc(ptr, size)) == NULL) {
		errno = errno_saved;
		message = get_explanation_of_error(errno);
		print_error_message("[ERROR MESSAGE] malloc() [%d] : %s\n", errno, message.text);
		free_message(&message);
		_exit(EXIT_FAILURE);
	}
	return array;
}