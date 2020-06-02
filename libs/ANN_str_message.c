#include "./ANN_lib_settings.h"

void
declare_message (Message_t *message) {
	message->text = NULL;
	message->length = (size_t) 0;
}

void
initialize_message (Message_t *message, char *text, size_t length) {
	if (!text) {
		print_error_message("[ERROR MESSAGE] initialize_message() : Input text to message is empty.\n");
		exit(EXIT_FAILURE);
	}
	if (!message->text) {
		message->text = Calloc(length + (size_t) 1, sizeof(char));
		strncpy(message->text, text, length);
		message->length = length;
	} else {
		message->text = Realloc(message->text, (message->length + length + (size_t) 1) * sizeof(char));
		strncat(message->text, text, length);
		message->length = message->length + length;
	}
}

void
free_message (Message_t *message) {
	free(message->text);
	message->length = (size_t) 0;
	declare_message(message);
}