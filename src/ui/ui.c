#include "ui.h"
#include "data/data.h"
#include "utils/editor_utils.h"

#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <locale.h>

#define OVER_VIEW_TO_DETAIL_RATIO 0.7
#define STATUSBAR_HEIGHT 2
#define DOUBLE_BORDER_WIDTH 2
#define MAX_UI_TEXT_LINES 10000

const char *view_names[9] = {
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


void init_ncurses(void) {
	setlocale(LC_ALL, "en_US.UTF-8");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
}

void init_colors(void) {

}

void init_windows(struct Model *model) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	int overview_width = (int) round((double) max_x * (1.0 - OVER_VIEW_TO_DETAIL_RATIO));
	model->statusbar = newwin(STATUSBAR_HEIGHT, max_x, max_y - STATUSBAR_HEIGHT, 0);
	model->overview_window = newwin(max_y - STATUSBAR_HEIGHT, overview_width, 0, 0);
	model->detail_window = newwin(max_y - STATUSBAR_HEIGHT, max_x - overview_width, 0, overview_width);
	model->detail_text_window = derwin(model->detail_window,
	                                   getmaxy(model->detail_window) - DOUBLE_BORDER_WIDTH, // height
	                                   getmaxx(model->detail_window) - DOUBLE_BORDER_WIDTH, // width
	                                   1, 1);
	model->detail_text_pad = newpad(MAX_UI_TEXT_LINES, getmaxx(model->detail_window) - DOUBLE_BORDER_WIDTH);
	model->help_window = newwin(max_y / 2, max_x / 2, max_y / 4, max_x / 4);
	model->help_text_window = derwin(model->help_window,
	                                 getmaxy(model->help_window) - DOUBLE_BORDER_WIDTH - // height
	                                 1, // extra line in model->help_text_window to display current help page
	                                 getmaxx(model->help_window) - DOUBLE_BORDER_WIDTH, // width
	                                 2, 1);
	model->add_window = newwin(8, max_x, max_y - STATUSBAR_HEIGHT - 8, 0);
	model->add_text_window = derwin(model->add_window,
	                                getmaxy(model->add_window) - DOUBLE_BORDER_WIDTH, // height
	                                getmaxx(model->add_window) - DOUBLE_BORDER_WIDTH, // width
	                                1, 1);
	model->popup_window = newwin(8, max_x * 2 / 3, (max_y * 2 / 3) - 4, max_x / 6);
}

void draw_screen(struct Model *model) {
	// Draw the statusbar
	wclear(model->statusbar);
	wattron(model->statusbar, A_BOLD);
	wattron(model->statusbar, A_REVERSE);
	mvwhline(model->statusbar, 0, 0, ' ', getmaxx(model->statusbar));
	mvwprintw(model->statusbar, 0, 1, "Mode: %s | Size of list: %d", view_names[model->view], model->list->size);
	wattroff(model->statusbar, A_REVERSE);
	wattroff(model->statusbar, A_BOLD);
	mvwprintw(model->statusbar, 1, 1, "q:Quit  n:New  o:Open  d:Delete  ?:Help");
	wnoutrefresh(model->statusbar);

	// Draw the overview window
	draw_overview(model);

	// Draw the detail window
	draw_detail(model);

	if (model->view == HELP) {
		draw_help(model);
	}
	doupdate();

	// things that need to be drawn after do update

	switch (model->view) {
		case SEARCH:
			// TODO: Implement search view
			// IDEA: floating window with fzf like search
			// draw_search(model);
			break;
		case SORT:
			// TODO: Implement sort view
			// draw_sort(model);
			break;
		default:
			break;
	}
}

void draw_border(WINDOW *window, char *title) {
	box(window, 0, 0);
	wattron(window, A_BOLD);
	if (title != NULL)
		mvwprintw(window, 0, 2, "%s", title);
	wattroff(window, A_BOLD);
}

void draw_overview(struct Model *model) {
	wclear(model->overview_window);
	if (model->list->size == 0) {
		mvwprintw(model->overview_window, 1, 1, "No projects found");
	} else {
		node *current = model->list->head;
		int max_y = getmaxy(model->overview_window);
		if (model->list->size > max_y) {
			int index = get_index(model->list, model->current);
			int offset = index - max_y / 2;
			if (offset < 0) {
				offset = 0;
			}
			for (int i = 0; i < offset; i++) {
				current = current->next;
			}
		}

		int i = 1;
		while (current != NULL) {
			opensource_project *osp = current->value;
			if (current == model->current) {
				wattron(model->overview_window, A_REVERSE);
			}
			mvwhline(model->overview_window, i, 1, ' ', getmaxx(model->overview_window) - 2);
			mvwprintw(model->overview_window, i, 1, "%s", osp->name);
			if (current == model->current) {
				wattroff(model->overview_window, A_REVERSE);
			}
			current = current->next;
			i++;
		}
	}
	if (model->view == LIST) {
		wattron(model->overview_window, A_REVERSE);
		draw_border(model->overview_window, "Overview");
		wattroff(model->overview_window, A_REVERSE);
	} else {
		draw_border(model->overview_window, "Overview");
	}
	wnoutrefresh(model->overview_window);
}

void draw_detail(struct Model *model) {
	if (model->current == NULL) {
		mvwprintw(model->detail_text_window, 0, 0, "No project selected");
	} else {
		// cast the current node to an open source project
		opensource_project *osp = model->current->value;

		// clear the detail window
		wclear(model->detail_text_window);

		int max_x = getmaxx(model->detail_text_window);
		mvwprintw(model->detail_text_window, 0, 0, "%s", osp->name);
		mvwprintw(model->detail_text_window, 1, 0, "%s\n", osp->url);
		mvwprintw(model->detail_text_window, 0, max_x - 13, "Stars: %6d", osp->stars);
		mvwprintw(model->detail_text_window, 1, max_x - 13, "Issues: %5d", osp->issues);
		mvwhline(model->detail_text_window, 2, 0, ACS_HLINE, max_x);
		wclear(model->detail_text_pad);
		if (model->show_hex) {
			draw_hex(model);
		} else {
			mvwaddstr(model->detail_text_pad, 0, 0, osp->description);
		}
	}
	if (model->view == DETAIL) {
		wattron(model->detail_window, A_REVERSE);
		draw_border(model->detail_window, "Detail");
		wattroff(model->detail_window, A_REVERSE);
	} else if (model->show_hex) {
		wattron(model->detail_window, A_REVERSE);
		draw_border(model->detail_window, "Hex");
		wattroff(model->detail_window, A_REVERSE);
	} else {
		draw_border(model->detail_window, "Detail");
	}
	wnoutrefresh(model->detail_window);
	pnoutrefresh(model->detail_text_pad, // source pad
	             model->detail_pos, // source pad y
	             0, // source pad x
	             getbegy(model->detail_text_window) + 3, // dest window y
	             getbegx(model->detail_text_window), // dest window x
	             getbegy(model->detail_text_window) + getmaxy(model->detail_text_window) - 1, // dest window height
	             getbegx(model->detail_text_window) + getmaxx(model->detail_text_window) // dest window width
	);
}

const char *help_elements[] = {
		"q: Quit",
		"j: Move down",
		"k: Move up",
		"l: Move right",
		"h: Move left",
		"n: New entry",
		"e: Edit entry",
		"d: Delete entry",
		"o: Open/Append from File",
		"w: Write file",
		"x: Toggle hex view",
		"/: Search",
		"?: Help",
		"ENTER: Confirm",
		"F10: Quit",
		"J: next in Detail View",
		"K: previous in Detail View",
};

void draw_help(struct Model *model) {
	wclear(model->help_text_window);
	int max_y = getmaxy(model->help_text_window);
	size_t elements = sizeof(help_elements) / sizeof(help_elements[0]);
	size_t pages = (elements - 1) / max_y;
	if (model->help_page > pages) {
		model->help_page = pages;
	}
	size_t start = model->help_page * max_y;
	size_t end = start + max_y;
	if (end > sizeof(help_elements) / sizeof(help_elements[0])) {
		end = sizeof(help_elements) / sizeof(help_elements[0]);
	}
	for (size_t i = start; i < end; ++i) {
		mvwprintw(model->help_text_window, (int) i - (int) start, 0, " %s", help_elements[i]);
	}
	draw_border(model->help_window, "Help");
	mvwprintw(model->help_window, 1, 2, "Page %zu of %zu", model->help_page + 1, pages + 1);
	wsyncdown(model->help_window);
	wnoutrefresh(model->help_window);
}


char *ui_open_vim(char *buffer) {
	return editor_edit(buffer, "nvim +\"set ft=markdown\"");
}

char *ui_open_nano(char *buffer) {
	return editor_edit(buffer, "nano");
}

char *ui_text_input(WINDOW *win, int y, int x, char *buffer) {
	int ch;
	int selected = 0;
	if (buffer != NULL) {
		selected = (int) strlen(buffer);
		mvwprintw(win, y, x, "%s", buffer);
		wmove(win, y, x + selected);
		wredrawln(win, y, 1);
	}
	curs_set(1);
	// get the name continue on tab
	while ((ch = wgetch(win)) != '\t') {
		if (ch == KEY_BACKSPACE || ch == 127) {
			if (selected > 0) {
				selected--;
				buffer = realloc(buffer, selected + 1); // +1 for null terminator
				buffer[selected] = '\0';
				mvwprintw(win, y, x + selected, " ");
				wmove(win, y, x + selected);
			}
		} else if (ch == 9 || ch == KEY_ENTER || ch == 13) { // tab or enter
			break;
		} else if (ch >= 32 && ch <= 126) { // only allow printable characters
			selected++;
			buffer = realloc(buffer, selected + 1); // +1 for null terminator
			buffer[selected] = '\0';
			buffer[selected - 1] = (char) ch;
			mvwprintw(win, y, x, "%s", buffer);
			wmove(win, y, x + selected);
		}
		wredrawln(win, y, 1);
	}
	curs_set(0);
	return buffer;
}

uint32_t ui_uint32_t_input(WINDOW *win, int y, int x) {
	int ch;
	int selected = 0;
	char *buffer = NULL;
	if (buffer != NULL) {
		selected = (int) strlen(buffer);
		mvwprintw(win, y, x, "%s", buffer);
		wredrawln(win, y, 1);
	}
	curs_set(1);
	// get the name continue on tab
	while ((ch = wgetch(win)) != '\t') {
		if (ch == KEY_BACKSPACE || ch == 127) {
			if (selected > 0) {
				selected--;
				buffer = realloc(buffer, selected + 1); // +1 for null terminator
				buffer[selected] = '\0';
				mvwprintw(win, y, x + selected, " ");
				wmove(win, y, x + selected);
			}
		} else if (ch == 9 || ch == KEY_ENTER || ch == 13) { // tab or enter
			break;
		} else if (ch >= '0' && ch <= '9') { // only allow numbers
			selected++;
			buffer = realloc(buffer, selected + 1); // +1 for null terminator
			buffer[selected] = '\0';
			buffer[selected - 1] = (char) ch;
			mvwprintw(win, y, x, "%s", buffer);
		}
		wredrawln(win, y, 1);
	}
	uint32_t num = 0;
	uint32_t multiplier = 1;
	for (int i = selected - 1; i >= 0; i--) {
		num += (buffer[i] - '0') * multiplier;
		multiplier *= 10;
	}
	free(buffer);
	curs_set(0);
	return num;
}

void draw_ok_cancel(WINDOW *win, int y, int x, const char *ok, const char *cancel) {
	int x_cancel = x + 4 + (int) strlen(ok);
	wattron(win, A_REVERSE);
	mvwprintw(win, y, x, " %s ", ok);
	mvwprintw(win, y, x_cancel, " %s ", cancel);
	wattroff(win, A_REVERSE);
	wredrawln(win, y, 1);
}

ui_okcancel ui_ok_cancel(WINDOW *win, int y, int x, const char *ok, const char *cancel) {
	int ch;
	int x_ok = x;
	int x_cancel = x + 4 + (int) strlen(ok);
	wattron(win, A_REVERSE);
	wattron(win, A_BOLD);
	mvwprintw(win, y, x_ok, " %s ", ok);
	wattroff(win, A_BOLD);
	mvwprintw(win, y, x_cancel, " %s ", cancel);
	wattroff(win, A_REVERSE);
	wredrawln(win, y, 1);

	bool on_ok = true;
	while ((ch = wgetch(win)) != KEY_F(10)) {
		if (ch == '\t') {
			if (on_ok) {
				on_ok = false;
				wattron(win, A_REVERSE);
				mvwprintw(win, y, x_ok, " %s ", ok);
				wattron(win, A_BOLD);
				mvwprintw(win, y, x_cancel, " %s ", cancel);
				wattroff(win, A_BOLD);
				wattroff(win, A_REVERSE);
			} else {
				return UI_RETRY;
			}
		} else if (ch == KEY_LEFT || ch == 'h') {
			on_ok = true;
			wattron(win, A_REVERSE);
			wattron(win, A_BOLD);
			mvwprintw(win, y, x_ok, " %s ", ok);
			wattroff(win, A_BOLD);
			mvwprintw(win, y, x_cancel, " %s ", cancel);
			wattroff(win, A_REVERSE);
		} else if (ch == KEY_RIGHT || ch == 'l') {
			on_ok = false;
			wattron(win, A_REVERSE);
			mvwprintw(win, y, x_ok, " %s ", ok);
			wattron(win, A_BOLD);
			mvwprintw(win, y, x_cancel, " %s ", cancel);
			wattroff(win, A_BOLD);
			wattroff(win, A_REVERSE);
		} else if (ch == KEY_ENTER || ch == '\n') {
			if (on_ok) {
				return UI_OK;
			} else {
				return UI_CANCEL;
			}
		}
	}
	return UI_CANCEL;
}

void end_ncurses(void) {
	endwin();
}

void select_next(struct Model *model) {
	if (model->current != NULL && model->current->next != NULL) {
		model->current = model->current->next;
		model->detail_pos = 0;
	} else if ((model->current == NULL && model->list->head != NULL) ||
	           (model->current != NULL && model->current->next == NULL)) {
		model->current = model->list->head;
		model->detail_pos = 0;
	}
}

void select_previous(struct Model *model) {
	if (model->current != NULL && model->current->previous != NULL) {
		model->current = model->current->previous;
		model->detail_pos = 0;
	} else if ((model->current == NULL && model->list->tail != NULL) ||
	           (model->current != NULL && model->current->previous == NULL)) {
		model->current = model->list->tail;
		model->detail_pos = 0;
	}
}

bool handle_list_input(struct Model *model, int ch) {
	switch (ch) {
		case 'k':
		case KEY_UP:
			select_previous(model);
			return true;
		case 'j':
		case KEY_DOWN:
			select_next(model);
			return true;
		case 'l':
		case '\t':
		case KEY_LEFT:
		case KEY_RIGHT:
			model->view = DETAIL;
			return true;
		default:
			break;
	}
	return false;
}

bool handle_detail_input(struct Model *model, int ch) {
	switch (ch) {
		case 'h':
		case '\t':
		case KEY_LEFT:
		case KEY_RIGHT:
			model->view = LIST;
			return true;
		case 'j':
			if (model->detail_pos < MAX_UI_TEXT_LINES) {
				model->detail_pos = model->detail_pos + 1;
			}
			return true;
		case 'k':
			if (model->detail_pos > 0) {
				model->detail_pos--;
			}
			return true;
		case 'K':
		case KEY_UP:
			select_previous(model);
			return true;
		case 'J':
		case KEY_DOWN:
			select_next(model);
			return true;
		default:
			break;
	}
	return false;
}

bool handle_help_input(struct Model *model, int ch) {
	switch (ch) {
		case 27:
		case 'q':
			model->view = model->previous_view;
			return true;
		case 'k':
		case KEY_UP:
			model->help_page--;
			return true;
		case 'j':
		case KEY_DOWN:
			model->help_page++;
			return true;
		default:
			break;
	}
	return false;
}

void draw_hex(struct Model *model) { // UP_HEX
	node *current = model->list->head;
	int i = 0;
	while (current != NULL) {
		mvwprintw(model->detail_text_pad, i, 0,
		          "%3d: %p 👆: %14p 👇: %14p",
		          i,
		          (void *) current,
		          (void *) current->previous,
		          (void *) current->next);
		current = current->next;
		i++;
	}
}
