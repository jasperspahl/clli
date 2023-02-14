//
// Created by jasper on 14.02.23.
//

#include "commands.h"
#include "utils/string_utils.h"
#include "data/file_parsing.h"

void handle_command(struct Model *model, char *command_input) {
	size_t argc;
	char **argv = split_string(command_input, ' ', &argc);
	if (string_equals(argv[0], "o") || string_equals(argv[0], "open")) {
		open_command(model, argc, argv);
	} else if (string_equals(argv[0], "w") || string_equals(argv[0], "write")) {
		write_command(model, argc, argv);
	} else if (string_equals(argv[0], "wq")) {
		write_command(model, argc, argv);
		quit_command(model, argc, argv);
	} else if (string_equals(argv[0], "q") || string_equals(argv[0], "quit")) {
		quit_command(model, argc, argv);
	}
}


void write_command(struct Model *model, size_t argc, char **argv) {
	char * filename = argc == 2 && string_length(argv[1]) > 0 ? argv[1] : model->input_file;
	if (filename == NULL || string_length(filename) < 1) {
		// TODO: add error message to statusbar
		return;
	}
	write_file(filename, model->list);
	if (model->input_file == NULL) model->input_file = filename;
}

void open_command(struct Model *model, size_t argc, char **argv) {
	if (argc < 2) {
		return; // needs to be 2 to get the filename
	}
	if (string_length(argv[1]) < 1) {
		// TODO: Error handling
		return;
	}
	model->input_file = argv[1];
	model->list = read_file(model->input_file);
	model->current = model->list->head;
}

void quit_command(struct Model *model, size_t argc, char ** argv) {
	(void) argc;
	(void) argv;
	model->view = QUIT;
}
