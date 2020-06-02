#include "./ANN_lib_settings.h"

int 
Strerror (int errnum, Message_t message) {
	int ret;
	int errno_saved;
	
	errno_saved = errno;
	errno = 0;
	if ((ret = strerror_r(errnum, message.text, message.length)) == -1) {
		if (errno == EINVAL) {
			print_error_message("[ERROR MESSAGE] strerror_r() [22] : Invalid argument\n");
		}
		if (errno == ERANGE) {
			print_error_message("[ERROR MESSAGE] strerror_r() [34] : Math result not representable\n");
		}
	}
	return ret;
}

ssize_t 
get_count_of_str_in_message (Message_t dest, Message_t repl) {
	size_t count;
	char *ptr, *tmp;

	if (!dest.text || !repl.text) {
		print_error_message("[ERROR MESSAGE] get_count_of_str_in_message() : A destination string or a replacement string are empty.\n");
		return (ssize_t) -1;
	}
	ptr = dest.text;
	for (count = 0; tmp = strstr(ptr, repl.text); ++count) {
		ptr = tmp + repl.length;
	}
	return count;
}

int
replace_message_in_message (Message_t srce, Message_t repl, Message_t *with) {
	ssize_t count;
	ssize_t left_part;
	ssize_t new_length;
	char *ptr, *tmp, *res;

	if (!with->text) {
		declare_obj(with);
		initialize_message(with, "", (size_t) 0);
	}
	if ((count = get_count_of_str_in_message(srce, repl)) >= (ssize_t) 0) {
		new_length = srce.length + (with->length - repl.length) * count;
		tmp = res = Malloc(new_length + (size_t) 1);
		if (!res) {
			print_error_message("[ERROR MESSAGE] strreplc() : Not enoth a free space for temp messages.\n");
			return -1;
		}
		while (count > (size_t) 0) {
			ptr = strstr(srce.text, repl.text);
			left_part = ptr - srce.text;
			tmp = strncpy(tmp, srce.text, left_part) + left_part;
			tmp = strcpy(tmp, with->text) + with->length;
			srce.text += left_part + repl.length;
			count = count - (size_t) 1;
		}
		strcpy(tmp, srce.text);
		free_obj(with);
		declare_obj(with);
		initialize_message(with, res, new_length);
		free(res);
	} else {
		return -1;
	}
	return 0;
}

Message_t
get_explanation_of_error (int errnum) {
	#define BUF_SIZE 256
	
	int errno_saved;
	Message_t message;

	errno_saved = errno;
	errno = 0;
	declare_obj(&message);
	initialize_message(&message, "\0", BUF_SIZE);
	while (Strerror(errnum, message) == -1 && errno == ERANGE) {
		errno_saved = errno;
		if (message.length < _POSIX_STREAM_MAX) {
			message.length = 2 * message.length;
		} else {
			break;
		}
		initialize_message(&message, NULL, message.length);
	}
	return message;
}

void
print_error_message (const char *buffer, ...) {
	#define BUF_SIZE 256

	va_list parameters;
	Message_t message;
	
	va_start(parameters, buffer);
	fflush(stderr);
	declare_obj(&message);
	initialize_message(&message, "\0", BUF_SIZE);
	message.length = vsnprintf(message.text, BUF_SIZE, buffer, parameters);
	Write(STDERR_FILENO, message.text, message.length);
	free_message(&message);
	fflush(NULL);
	va_end(parameters);
}