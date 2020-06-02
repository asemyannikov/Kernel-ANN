ann: ann.o libann.so
	gcc -o ann ann.o -lm -L. -lann -Wl,-rpath,.
	rm -f *.o

mksettings: getsettings.o libsettings.so
	gcc -o mksettings getsettings.o -lm -L. -lsettings -Wl,-rpath,.
	rm -f *.o

mkgraph: getgraph.o libgraph.so
	gcc -o mkgraph getgraph.o -lm -L. -lgraph -Wl,-rpath,.
	rm -f *.o

mkmachine: getmachine.o libmachine.so
	gcc -o mkmachine getmachine.o -lm -L. -lmachine -Wl,-rpath,.
	rm -f *.o

ann.o: ann.c
	gcc -c ann.c

getsettings.o: getsettings.c
	gcc -c getsettings.c

getgraph.o: getgraph.c
	gcc -c getgraph.c

getmachine.o: getmachine.c
	gcc -c getmachine.c

mkann-valgrind:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./ann

mksettings-valgrind:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./mksettings

mkgraph-valgrind:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./mkgraph

mkmachine-valgrind:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./mkmachine

libann.so: ANN_mkprocess.o
	gcc -shared -o libann.so ANN_mkprocess.o

libsettings.so: ANN_def_functions.o ANN_mkprocess.o ANN_stdlib.o ANN_string.o ANN_str_message.o ANN_str_messages.o ANN_str_setting.o ANN_str_settings.o ANN_unistd.o
	gcc -shared -o libsettings.so ANN_def_functions.o ANN_mkprocess.o ANN_stdlib.o ANN_string.o ANN_str_message.o ANN_str_messages.o ANN_str_setting.o ANN_str_settings.o ANN_unistd.o

libgraph.so: ANN_handler.o ANN_mklib.o ANN_mkprocess.o ANN_def_functions.o ANN_stdlib.o ANN_str_graph.o ANN_string.o ANN_str_message.o ANN_str_messages.o ANN_str_variable.o ANN_str_variables.o ANN_unistd.o
	gcc -shared -o libgraph.so ANN_handler.o ANN_mklib.o ANN_mkprocess.o ANN_def_functions.o ANN_stdlib.o ANN_str_graph.o ANN_string.o ANN_str_message.o ANN_str_messages.o ANN_str_variable.o ANN_str_variables.o ANN_unistd.o

libmachine.so: ANN_rand.o ANN_stdlib.o ANN_string.o ANN_machine.o ANN_str_message.o ANN_str_messages.o functions.o ANN_str_states.o ANN_unistd.o
	gcc -shared -o libmachine.so ANN_rand.o ANN_stdlib.o ANN_string.o ANN_machine.o ANN_str_message.o ANN_str_messages.o functions.o ANN_str_states.o ANN_unistd.o -lm

ANN_def_functions.o: ./libs/ANN_def_functions.c
	gcc -c -fPIC ./libs/ANN_def_functions.c

ANN_handler.o: ./libs/ANN_handler.c
	gcc -c -fPIC ./libs/ANN_handler.c

ANN_mklib.o: ./libs/ANN_mklib.c
	gcc -c -fPIC ./libs/ANN_mklib.c

ANN_mkprocess.o: ./libs/ANN_mkprocess.c
	gcc -c -fPIC ./libs/ANN_mkprocess.c

ANN_machine.o: ./libs/ANN_machine.c
	gcc -c -fPIC ./libs/ANN_machine.c

ANN_rand.o: ./libs/ANN_rand.c
	gcc -c -fPIC ./libs/ANN_rand.c

ANN_stdlib.o: ./libs/ANN_stdlib.c
	gcc -c -fPIC ./libs/ANN_stdlib.c

ANN_str_graph.o: ./libs/ANN_str_graph.c
	gcc -c -fPIC ./libs/ANN_str_graph.c

ANN_string.o: ./libs/ANN_string.c
	gcc -c -fPIC ./libs/ANN_string.c

ANN_str_message.o: ./libs/ANN_str_message.c
	gcc -c -fPIC ./libs/ANN_str_message.c

ANN_str_messages.o: ./libs/ANN_str_messages.c
	gcc -c -fPIC ./libs/ANN_str_messages.c

ANN_str_setting.o: ./libs/ANN_str_setting.c
	gcc -c -fPIC ./libs/ANN_str_setting.c

ANN_str_settings.o: ./libs/ANN_str_settings.c
	gcc -c -fPIC ./libs/ANN_str_settings.c

ANN_str_variable.o: ./libs/ANN_str_variable.c
	gcc -c -fPIC ./libs/ANN_str_variable.c

ANN_str_variables.o: ./libs/ANN_str_variables.c
	gcc -c -fPIC ./libs/ANN_str_variables.c

ANN_str_states.o: ./libs/ANN_str_states.c
	gcc -c -fPIC ./libs/ANN_str_states.c

ANN_unistd.o: ./libs/ANN_unistd.c
	gcc -c -fPIC ./libs/ANN_unistd.c

functions.o: ./libs/functions.c
	gcc -c -fPIC ./libs/functions.c
	
clean:
	rm -f *.o libsettings.so libgraph.so libmachine.so mksettings mkgraph mkmachine