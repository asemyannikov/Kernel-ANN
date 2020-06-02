#include "./ANN_lib_settings.h"

void 
declare_settings (Settings_t *settings) {
	settings->setting = NULL;
	settings->rows = (size_t) 0;
}

void 
initialize_settings (Settings_t *settings, Setting_t setting) {
	if (settings->rows == (size_t) 0) {
		settings->setting = Calloc((size_t) 1, sizeof(Setting_t));
	} else {
		settings->setting = Realloc(settings->setting, (settings->rows + (size_t) 1) * sizeof(Setting_t));
	}
	settings->setting[settings->rows] = setting;
	settings->rows = settings->rows + (size_t) 1;
}

void 
free_settings (Settings_t *settings) {
	size_t index;

	for (index = (size_t) 0; index < settings->rows; index = index + (size_t) 1) {
		free_setting(&settings->setting[index]);
	}
	settings->rows = (size_t) 0;
	free(settings->setting);
}