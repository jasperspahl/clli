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

extern const char *view_names[9];

struct Model {
	linked_list *list;
	node *current;

	enum View view;
	enum View previous_view;
	WINDOW *overview_window;
	WINDOW *detail_window;
	WINDOW *detail_text_window;
	WINDOW *detail_text_pad;
	WINDOW *help_window;
	WINDOW *help_text_window;
	WINDOW *add_window;
	WINDOW *add_text_window;
	WINDOW *popup_window;
	WINDOW *statusbar;

	char *search_term;
	char *input_file;
	size_t help_page;
	int detail_pos;
};

void usage(char *program_name);

void init_ncurses(void);

void init_colors(void);

void init_windows(struct Model *model);

void draw_screen(struct Model *model);

void draw_border(WINDOW *window, char *title);

void draw_overview(struct Model *model);

void draw_detail(struct Model *model);

void draw_help(struct Model *model);

void select_next(struct Model *model);

void select_previous(struct Model *model);


/**
 * Ui Text input.
 * example for usage:
 * @code
 * char *buffer;
 * ui_text_input(window, y, x, buffer);
 * @endcode
 * @param window The window to draw the input to.
 * @param y The y position to start the input.
 * @param x The x position to start the input.
 * @param buffer The buffer to write the input to.
 * @return The buffer.
 */
char *ui_text_input(WINDOW *window, int y, int x, char *buffer);

uint32_t ui_uint32_t_input(WINDOW *window, int y, int x);

typedef enum ui_okcancel_ {
	UI_OK,
	UI_CANCEL,
	UI_RETRY
} ui_okcancel;

/**
 * Ui Ok/Cancel dialog.
 * @example
 * @code
 * char *buffer;
 * ui_okcanel result == UI_RETRY;
 * while(result == UI_RETRY) {
 * 	ui_text_input(window, y, x, buffer);
 * 	result = ui_ok_cancel(window, y, x, "Ok", "Cancel");
 * 	if(buffer == NULL || strlen(buffer) == 0) {
 * 		result = UI_RETRY;
 * 	}
 * }
 * if(result == UI_CANCEL) {
 * 	free(buffer);
 * 	return;
 * }
 *
 * @endcode
 * @param window The window to draw the dialog to.
 * @param y The y position to start the dialog.
 * @param x The x position to start the dialog.
 * @param ok The text to display for the ok button.
 * @param cancel The text to display for the cancel button.
 * @return UI_OK if the user pressed ok, UI_CANCEL if the user pressed cancel and UI_RETRY if the user pressed retry.
 */
ui_okcancel ui_ok_cancel(WINDOW *window, int y, int x, const char *ok, const char *cancel);

void draw_ok_cancel(WINDOW *window, int y, int x, const char *ok, const char *cancel);

char *ui_open_vim(char *buffer);

char *ui_open_nano(char *buffer);


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
