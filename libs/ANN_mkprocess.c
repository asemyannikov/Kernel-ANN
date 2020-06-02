#include "./ANN_lib_graph.h"
#include "./ANN_lib_settings.h"

int 
run_clean() {
	sigset_t blockMask, origMask;
	struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
	int status, savedErrno;
	pid_t childPid;

	/* Установка блокировки сигнала SIGCHLD в основном процессе: */
	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &blockMask, &origMask);
	/* Установка игнорирования сигналов SIGINT и SIGQUIT основным процессом: */
	saIgnore.sa_handler = SIG_IGN;
	saIgnore.sa_flags = 0;
	sigemptyset(&saIgnore.sa_mask);
	sigaction(SIGINT, &saIgnore, &saOrigInt);
	sigaction(SIGQUIT, &saIgnore, &saOrigQuit);
	switch (childPid = fork()) {
	case -1:
		/* Ветка выполнения ошибки с формированием потомка процесса */
		status = -1;
		break;
	case 0:
		/* Ветка выполнения сценария потомка основного процесса: */
		saDefault.sa_handler = SIG_DFL;
		saDefault.sa_flags = 0;
		sigemptyset(&saDefault.sa_mask);
		if (saOrigInt.sa_handler != SIG_IGN) {
			sigaction(SIGINT, &saDefault, NULL);
		}
		if (saOrigQuit.sa_handler != SIG_IGN) {
			sigaction(SIGQUIT, &saDefault, NULL);
		}
		sigprocmask(SIG_SETMASK, &origMask, NULL);
		execl("/bin/sh", "sh", "-c", "(truncate -s 0 ./libs/functions.c ./libs/graph_parameters.h ./libs/settings.h && make clean)", (const char *)NULL);
		/* Некорректное выполнение execl приводит к критическому завершению: */
		_exit(127);
	default:
		/* Ветка выполнения сценария основного процесса */
		while (waitpid(childPid, &status, 0) == -1) {
			if (errno != EINTR) {
				status = -1;
				break;
			}
		}
		break;
	}
	savedErrno = errno;
	sigprocmask(SIG_SETMASK, &origMask, NULL);
	sigaction(SIGINT, &saIgnore, NULL);
	sigaction(SIGQUIT, &saIgnore, NULL);
	return status;
}

int 
run_mksettings() {
	sigset_t blockMask, origMask;
	struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
	int status, savedErrno;
	pid_t childPid;

	/* Установка блокировки сигнала SIGCHLD в основном процессе: */
	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &blockMask, &origMask);
	/* Установка игнорирования сигналов SIGINT и SIGQUIT основным процессом: */
	saIgnore.sa_handler = SIG_IGN;
	saIgnore.sa_flags = 0;
	sigemptyset(&saIgnore.sa_mask);
	sigaction(SIGINT, &saIgnore, &saOrigInt);
	sigaction(SIGQUIT, &saIgnore, &saOrigQuit);
	switch (childPid = fork()) {
	case -1:
		/* Ветка выполнения ошибки с формированием потомка процесса */
		status = -1;
		break;
	case 0:
		/* Ветка выполнения сценария потомка основного процесса: */
		saDefault.sa_handler = SIG_DFL;
		saDefault.sa_flags = 0;
		sigemptyset(&saDefault.sa_mask);
		if (saOrigInt.sa_handler != SIG_IGN) {
			sigaction(SIGINT, &saDefault, NULL);
		}
		if (saOrigQuit.sa_handler != SIG_IGN) {
			sigaction(SIGQUIT, &saDefault, NULL);
		}
		sigprocmask(SIG_SETMASK, &origMask, NULL);
		execl("/bin/sh", "sh", "-c", "(make mksettings && ./mksettings)", (const char *)NULL);
		/* Некорректное выполнение execl приводит к критическому завершению: */
		_exit(127);
	default:
		/* Ветка выполнения сценария основного процесса */
		while (waitpid(childPid, &status, 0) == -1) {
			if (errno != EINTR) {
				status = -1;
				break;
			}
		}
		break;
	}
	savedErrno = errno;
	sigprocmask(SIG_SETMASK, &origMask, NULL);
	sigaction(SIGINT, &saIgnore, NULL);
	sigaction(SIGQUIT, &saIgnore, NULL);
	return status;
}

int 
run_mkmachine() {
	sigset_t blockMask, origMask;
	struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
	int status, savedErrno;
	pid_t childPid;

	/* Установка блокировки сигнала SIGCHLD в основном процессе: */
	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &blockMask, &origMask);
	/* Установка игнорирования сигналов SIGINT и SIGQUIT основным процессом: */
	saIgnore.sa_handler = SIG_IGN;
	saIgnore.sa_flags = 0;
	sigemptyset(&saIgnore.sa_mask);
	sigaction(SIGINT, &saIgnore, &saOrigInt);
	sigaction(SIGQUIT, &saIgnore, &saOrigQuit);
	switch (childPid = fork()) {
	case -1:
		/* Ветка выполнения ошибки с формированием потомка процесса */
		status = -1;
		break;
	case 0:
		/* Ветка выполнения сценария потомка основного процесса: */
		saDefault.sa_handler = SIG_DFL;
		saDefault.sa_flags = 0;
		sigemptyset(&saDefault.sa_mask);
		if (saOrigInt.sa_handler != SIG_IGN) {
			sigaction(SIGINT, &saDefault, NULL);
		}
		if (saOrigQuit.sa_handler != SIG_IGN) {
			sigaction(SIGQUIT, &saDefault, NULL);
		}
		sigprocmask(SIG_SETMASK, &origMask, NULL);
		//execl("/bin/sh", "sh", "-c", "(cd ./diff/ && make && make valgrind)", (const char *)NULL);
		//execl("/bin/sh", "sh", "-c", "(./mkmachine)", (const char *)NULL);
		execl("/bin/sh", "sh", "-c", "(make mkmachine && ./mkmachine)", (const char *)NULL);
		/* Некорректное выполнение execl приводит к критическому завершению: */
		_exit(127);
	default:
		/* Ветка выполнения сценария основного процесса */
		while (waitpid(childPid, &status, 0) == -1) {
			if (errno != EINTR) {
				status = -1;
				break;
			}
		}
		break;
	}
	savedErrno = errno;
	sigprocmask(SIG_SETMASK, &origMask, NULL);
	sigaction(SIGINT, &saIgnore, NULL);
	sigaction(SIGQUIT, &saIgnore, NULL);
	return status;
}

int 
run_mkgraph() {
	sigset_t blockMask, origMask;
	struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
	int status, savedErrno;
	pid_t childPid;

	/* Установка блокировки сигнала SIGCHLD в основном процессе: */
	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &blockMask, &origMask);
	/* Установка игнорирования сигналов SIGINT и SIGQUIT основным процессом: */
	saIgnore.sa_handler = SIG_IGN;
	saIgnore.sa_flags = 0;
	sigemptyset(&saIgnore.sa_mask);
	sigaction(SIGINT, &saIgnore, &saOrigInt);
	sigaction(SIGQUIT, &saIgnore, &saOrigQuit);
	switch (childPid = fork()) {
	case -1:
		/* Ветка выполнения ошибки с формированием потомка процесса */
		status = -1;
		break;
	case 0:
		/* Ветка выполнения сценария потомка основного процесса: */
		saDefault.sa_handler = SIG_DFL;
		saDefault.sa_flags = 0;
		sigemptyset(&saDefault.sa_mask);
		if (saOrigInt.sa_handler != SIG_IGN) {
			sigaction(SIGINT, &saDefault, NULL);
		}
		if (saOrigQuit.sa_handler != SIG_IGN) {
			sigaction(SIGQUIT, &saDefault, NULL);
		}
		sigprocmask(SIG_SETMASK, &origMask, NULL);
		//execl("/bin/sh", "sh", "-c", "(cd ./diff/ && make && make valgrind)", (const char *)NULL);
		//execl("/bin/sh", "sh", "-c", "(./mkgraph)", (const char *)NULL);
		execl("/bin/sh", "sh", "-c", "(make mkgraph && ./mkgraph)", (const char *)NULL);
		/* Некорректное выполнение execl приводит к критическому завершению: */
		_exit(127);
	default:
		/* Ветка выполнения сценария основного процесса */
		while (waitpid(childPid, &status, 0) == -1) {
			if (errno != EINTR) {
				status = -1;
				break;
			}
		}
		break;
	}
	savedErrno = errno;
	sigprocmask(SIG_SETMASK, &origMask, NULL);
	sigaction(SIGINT, &saIgnore, NULL);
	sigaction(SIGQUIT, &saIgnore, NULL);
	return status;
}