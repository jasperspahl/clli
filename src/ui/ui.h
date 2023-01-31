#ifndef PROJEKT_UI_H
#define PROJEKT_UI_H

#include <ncurses.h>
#include <stdbool.h>
#include "../data/linked_list.h"

enum View {
	LIST,
	DETAIL,
	HELP,
	ADD,
	EDIT,
	DELETE,
	SEARCH,
	SORT,
	QUIT
};

static const char *view_names[] = {
		"  List",
		"Detail",
		"  Help",
		"   Add",
		"  Edit",
		"Delete",
		"Search",
		"  Sort",
		"  Quit"
};

struct Model {
	linked_list *list;
	node *current;

	enum View view;
	enum View previous_view;
	WINDOW *overview_window;
	WINDOW *detail_window;
	WINDOW *statusbar;
	char *search_term;
};

void usage(char *program_name);

void init_ncurses(void);

void init_colors(void);

void init_windows(struct Model *model);

void draw_screen(struct Model *model);

void draw_border(WINDOW *window, char *title);

void draw_overview(struct Model *model);

void draw_detail(struct Model *model);

/**
 * Handles the input for the list view.
 * @param model The model to update.
 * @param ch The input character.
 * @return True if the input was handled, false otherwise.
 */
bool handle_list_input(struct Model *model, int ch);

/**
 * Handles the input for the detail view.
 * @param model The model to update.
 * @param ch The input character.
 * @return True if the input was handled, false otherwise.
 */
bool handle_detail_input(struct Model *model, int ch);

/**
 * Handles the input for the help view.
 * @param model The model to update.
 * @param ch The input character.
 * @return True if the input was handled, false otherwise.
 */
bool handle_help_input(struct Model *model, int ch);

void end_ncurses(void);


#endif //PROJEKT_UI_H
