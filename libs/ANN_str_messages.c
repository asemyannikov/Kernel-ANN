#include "./ANN_lib_settings.h"

void
declare_messages (Messages_t *messages) {
	messages->message = NULL;
	messages->rows = (size_t) 0;
}

void
initialize_messages (Messages_t *messages, Message_t message) {
	if (messages->rows == (size_t) 0) {
		messages->message = Calloc((size_t) 1, sizeof(Message_t));
	} else {
		messages->message = Realloc(messages->message, (messages->rows + (size_t) 1) * sizeof(Message_t));
	}
	messages->message[messages->rows] = message;
	messages->rows = messages->rows + (size_t) 1;
}

void
free_messages (Messages_t *messages) {
	size_t index;

	for (index = (size_t) 0; index < messages->rows; index = index + (size_t) 1) {
		free_obj(&messages->message[index]);
	}
	messages->rows = (size_t) 0;
	free(messages->message);
}