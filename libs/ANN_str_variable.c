#include "./ANN_lib_graph.h"

void 
declare_variable (Variable_t *variable) {
	declare_obj(&variable->alias);
	declare_obj(&variable->path_to_inital_state);
	variable->learning = FALSE;
	variable->result= FALSE;
	variable->index_in_flower = (ssize_t) -1;
}

void 
initialize_variable (Variable_t *variable, Message_t alias, Message_t path_to_inital_state, ssize_t index_in_flower, Boolean_t learning, Boolean_t result) {
	if (!alias.text) {
		declare_obj(&alias);
		initialize_message(&alias, "", (size_t) 0);
	}
	if (!path_to_inital_state.text) {
		declare_obj(&path_to_inital_state);
		initialize_message(&path_to_inital_state, "", (size_t) 0);
	}
	if (variable->alias.text || variable->path_to_inital_state.text) {
		free_variable(variable);
		declare_obj(variable);
	}
	variable->alias = alias;
	variable->path_to_inital_state = path_to_inital_state;
	variable->index_in_flower = index_in_flower;
	variable->learning = learning;
	variable->result= result;
}

void 
free_variable (Variable_t *variable) {
	free_obj(&variable->alias);
	free_obj(&variable->path_to_inital_state);
	variable->index_in_flower = (ssize_t) -1;
	variable->learning = FALSE;
	variable->result= FALSE;
}