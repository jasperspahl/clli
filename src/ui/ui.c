//
// Created by jasper on 31.01.23.
//

#include "ui.h"
#include "../data/data.h"

#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <string.h>

#define OVER_VIEW_TO_DETAIL_RATIO 0.7
#define STATUSBAR_HEIGHT 2
#define DOUBLE_BORDER_WIDTH 2

void init_ncurses(void) {
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
	                                   getmaxy(model->detail_window) - DOUBLE_BORDER_WIDTH, // heigth
	                                   getmaxx(model->detail_window) - DOUBLE_BORDER_WIDTH, // width
	                                   1, 1);
}

void draw_screen(struct Model *model) {
	// Draw the statusbar
	wclear(model->statusbar);
	wattron(model->statusbar, A_BOLD);
	mvwprintw(model->statusbar, 0, 1, "Mode: %s | Size of list: %d", view_names[model->view], model->list->size);
	wattroff(model->statusbar, A_BOLD);
	wnoutrefresh(model->statusbar);

	// Draw the overview window
	draw_overview(model);

	// Draw the detail window
	draw_detail(model);
	doupdate();

	switch (model->view) {
		case HELP:
			// TODO: Implement help view
			// draw_help(model);
			break;
		case ADD:
			// TODO: Implement add view
			// draw_add(model);
			break;
		case DELETE:
			// TODO: Implement delete view
			// draw delete bestätigung
			//draw_delete(model);
			break;
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
		goto draw_bdr;
	}
	// cast the current node to an open source project
	opensource_project *osp = model->current->value;

	// clear the detail window
	wclear(model->detail_text_window);

	int max_x = getmaxx(model->detail_text_window);

	mvwprintw(model->detail_text_window, 0, 0, "%s", osp->name);
	mvwprintw(model->detail_text_window, 1, 0, "%s\n", osp->url);
	mvwprintw(model->detail_text_window, 0, max_x - 12, "Stars: %5d", osp->stars);
	mvwprintw(model->detail_text_window, 1, max_x - 12, "Issues: %4d", osp->issues);
	mvwhline(model->detail_text_window, 2, 0, ACS_HLINE, max_x);
	mvwprintw(model->detail_text_window, 3, 0, "%s", osp->description);

	draw_bdr:
	if (model->view == DETAIL) {
		wattron(model->detail_window, A_REVERSE);
		draw_border(model->detail_window, "Detail");
		wattroff(model->detail_window, A_REVERSE);
	} else {
		draw_border(model->detail_window, "Detail");
	}
	wnoutrefresh(model->detail_window);
}

void end_ncurses(void) {
	endwin();
}

bool handle_list_input(struct Model *model, int ch) {
	switch (ch) {
		case 'k':
		case KEY_UP:
			if (model->current->previous != NULL) {
				model->current = model->current->previous;
			}
			return true;
		case 'j':
		case KEY_DOWN:
			if (model->current->next != NULL) {
				model->current = model->current->next;
			}
			return true;
		case 'l':
		case '\t':
		case KEY_LEFT:
		case KEY_RIGHT:
			model->view = DETAIL;
			return true;
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
	}
	return false;
}

bool handle_help_input(struct Model *model, int ch) {
	switch (ch) {
		case 27:
		case 'q':
			model->view = model->previous_view;
			return true;
	}
	return false;
}
