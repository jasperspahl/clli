#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ncurses.h>

#include "data/linked_list.h"
#include "data/file_parsing.h"
#include "data/data.h"
#include "ui/ui.h"

#include "utils/get_testdata.h"

void usage(char *program_name) {
	printf("Usage: %s [<filename>]\n", program_name);
	exit(1);
}


int main(int argc, char **argv) {
	char *input_file;
	linked_list *list;
	if (argc > 2 || (argc > 1 && strcmp(argv[1], "-h") == 0)) {
		usage(argv[0]);
	}
	if (argc == 2) {
		input_file = argv[1];
		list = read_file(input_file);
	} else {
		list = create_linked_list(free_opensource_project);
	}

	if (list->size == 0) {
		opensource_project *osp =
				create_opensource_project("LibreCuisine",
				                          "A Cookbook Web Application based on the Microservices",
				                          "https://github.com/LibreCuisine/LibreCuisine", 1, 8);
		add_node(list, osp);
		opensource_project **osp_list = get_testdata_set();
		while (*osp_list != NULL) {
			add_node(list, *osp_list);
			osp_list++;
		}
	}

	struct Model model = {
			.list = list,
			.current = list->head,
			.view = LIST,
	};

	// Initialize ncurses
	init_ncurses();
	init_colors();

	init_windows(&model);
	refresh();

	// Draw the initial screen
	draw_screen(&model);

	// Main loop
	int ch;
	// F10 always quits (like htop)
	while (model.view != QUIT && (ch = getch()) != KEY_F(10)) {
		// Handle view specific input
		switch (model.view) {
			case LIST:
				if (handle_list_input(&model, ch)) {
					goto draw_now;
				}
				break;
			case DETAIL:
				if (handle_detail_input(&model, ch)) {
					goto draw_now;
				}
				break;
			case HELP:
				if (handle_help_input(&model, ch)) {
					goto draw_now;
				}
				break;
		}
		// Handle global input
		switch (ch) {
			case '?':
				model.previous_view = model.view;
				// TODO: Implement help
				model.view = HELP;
				break;
			case '/':
				model.previous_view = model.view;
				// TODO: Implement search
				model.view = SEARCH;
				break;
			case 'q':
				model.view = QUIT;
				break;
			case 'i':
				model.previous_view = model.view;
				model.view = EDIT;
				break;
			case 'a':
				model.previous_view = model.view;
				model.view = ADD;
				break;
		}
		// goto is used here to avoid handling input twice
		draw_now:
		// Draw the screen
		draw_screen(&model);
	}



	// Cleanup
	end_ncurses();
	if (argc == 2) {
		write_file(input_file, list);
	}
	write_file("output.bin", list);
	//free_list(list);
	return 0;
}
