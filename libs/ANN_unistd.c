#include "./ANN_lib_settings.h"

int 
Open (const char *pathname, int flags, mode_t mode) {
	int fd;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	if ((fd = open(pathname, flags, mode)) == -1) {
		if (errno == EINTR) {
			errno = errno_saved;
		}
		message = get_explanation_of_error(errno);
		print_error_message("[ERROR MESSAGE] open() [%d] : %s\n", errno, message.text);
		free_message(&message);
	}
	return fd;
}

ssize_t 
Read (int fd, char *buf, size_t len) {
	ssize_t ret;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	while (len != (size_t) 0 && (ret = read(fd, buf, len)) != (ssize_t) 0) {
		if (ret == (ssize_t) -1) {
			if (errno == EINTR) {
				errno = errno_saved;
				continue;
			}
			message = get_explanation_of_error(errno);
			print_error_message("[ERROR MESSAGE] read() [%d] : %s\n", errno, message.text);
			free_message(&message);
			break;
		}
		len = len - ret;
		buf = buf + ret;
	}
	return ret;
}

ssize_t 
Write (int fd, char *buf, size_t len) {
	ssize_t ret;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	while (len != (size_t) 0 && (ret = write(fd, buf, len)) != (ssize_t) 0) {
		if (ret == (ssize_t) -1) {
			if (errno == EINTR) {
				errno = errno_saved;
				continue;
			}
			message = get_explanation_of_error(errno);
			print_error_message("[ERROR MESSAGE] write() [%d] : %s\n", errno, message.text);
			free_message(&message);
			break;
		}
		len = len - ret;
		buf = buf + ret;
	}
	return ret;
}

ssize_t 
Lseek (int fd, off_t offset, int whence) {
	ssize_t pos;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	if ((pos = lseek(fd, offset, whence)) == (ssize_t) -1) {
		if (errno == EINTR) {
			errno = errno_saved;
		}
		message = get_explanation_of_error(errno);
		print_error_message("[ERROR MESSAGE] lseek() [%d] : %s\n", errno, message.text);
		free_message(&message);
	}
	return pos;
}

int 
Close (int fd) {
	int ret;
	int errno_saved;
	Message_t message;
	
	errno_saved = errno;
	errno = 0;
	if ((ret = close(fd)) == -1) {
		if (errno == EINTR) {
			errno = errno_saved;
		}
		message = get_explanation_of_error(errno);
		print_error_message("[ERROR MESSAGE] close() [%d] : %s\n", errno, message.text);
		free_message(&message);
	}
	return ret;
}

int 
get_size_of_file (int fd, ssize_t *length) {
	ssize_t position;
	
	if ((position = Lseek(fd, (off_t) 0, SEEK_END)) != (ssize_t) -1) {
		*length = position;
		if ((position = Lseek(fd, (off_t) 0, SEEK_SET)) == (ssize_t) -1) {
			return -1;
		}
	} else {
		return -1;
	}
	return 0;
}

void 
get_data (const char *path, Message_t *message) {
	int fd;

	declare_obj(message);
	if((fd = Open(path, O_RDWR, S_IRUSR | S_IRGRP | S_IROTH)) == -1) {
		print_error_message("[ERROR MESSAGE] get_data() : Cannot open settings file.\n");
		exit(EXIT_FAILURE);
	}
	if (get_size_of_file(fd, &message->length) == -1) {
		print_error_message("[ERROR MESSAGE] get_data() : Cannot get size of settings file.\n");
		exit(EXIT_FAILURE);
	}
	message->text = Calloc(message->length + (size_t) 1, sizeof(char));
	if (Read(fd, message->text, message->length) == (ssize_t) -1) {
		print_error_message("[ERROR MESSAGE] get_data() : Cannot read settings file.\n");
		exit(EXIT_FAILURE);
	}
	if (Close(fd) == -1) {
		print_error_message("[ERROR MESSAGE] get_data() : Cannot close settings file.\n");
		exit(EXIT_FAILURE);
	}
}

Boolean_t
is_number (Message_t *message) {
	char *str;

	for (str = message->text; *str; str++) {
		if (!(isdigit(*str)) && *str != '.' && strncmp(message->text, "(-1)", message->length) != 0) {
			return FALSE;
		}
	}
	return TRUE;
}

Boolean_t
is_function (Message_t *message) {
	size_t index;

	for (index = (size_t) 0; functions[index] != NULL; index = index + (size_t) 1) {
		if (strncmp(message->text, functions[index], message->length) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

void
set_number (Message_t *variable, size_t number) {
	declare_obj(variable);
	variable->length = snprintf(NULL, 0, "%ld", number);
	variable->text = Calloc(variable->length + (size_t) 1, sizeof(char));
	snprintf(variable->text, variable->length + (size_t) 1, "%ld", number);
}