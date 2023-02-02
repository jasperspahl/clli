#include "ui_flow.h"
#include "ui.h"
#include "../utils/fetch_readme.h"
#include "../data/data.h"
#include "../utils/files.h"
#include "../data/file_parsing.h"

#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

void start_add_flow(struct Model *model) {
	wclear(model->add_window);
	draw_border(model->add_window, "Add");
	mvwprintw(model->add_text_window, 1, 2, "a: Add project manually");
	mvwprintw(model->add_text_window, 2, 2, "g: Add project from GitHub");
	mvwprintw(model->add_text_window, 4, 2, "q: Quit");
	wmove(model->add_window, 1, 8);
	wrefresh(model->add_window);
	int ch;
	while ((ch = wgetch(model->add_window)) != 'q') {
		switch (ch) {
			case 'a':
				start_add_manual_flow(model);
				draw_screen(model);
				return;
			case 'g':
				start_add_github_flow(model);
				draw_screen(model);
				return;
			default:
				break;
		}
	}
	model->view = model->previous_view;
}

void start_add_manual_flow(struct Model *model) {
	int ch;
	char *name, *url, *description;
	uint32_t stars, issues;
	ui_okcancel status = UI_RETRY;
	name = url = description = NULL;
	wclear(model->add_text_window);

	while (status == UI_RETRY) {
		mvwprintw(model->add_text_window, 1, 2, "Name: ");
		draw_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		wmove(model->add_text_window, 1, 8);

		name = ui_text_input(model->add_text_window, 1, 8, name);
		if (name == NULL || strlen(name) == 0) {
			continue;
		}

		status = ui_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		if (status == UI_CANCEL) {
			free(name);
			return;
		}
	}

	wclear(model->add_text_window);
	status = UI_RETRY;
	while (status == UI_RETRY) {
		mvwprintw(model->add_text_window, 1, 2, "Url: ");
		draw_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		wmove(model->add_text_window, 1, 7);

		url = ui_text_input(model->add_text_window, 1, 7, url);
		if (url == NULL || strlen(url) == 0) {
			continue;
		}

		status = ui_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		if (status == UI_CANCEL) {
			free(name);
			free(url);
			return;
		}

	}

	wclear(model->add_text_window);
	mvwprintw(model->add_text_window, 1, 2, "Add description: ");
	mvwprintw(model->add_text_window, 2, 2, "v: open vim");
	mvwprintw(model->add_text_window, 3, 2, "n: open nano");
	mvwprintw(model->add_text_window, 4, 2, "s: skip");
	bool move_on = false;
	while (!move_on && (ch = wgetch(model->add_text_window)) != 's') {
		switch (ch) {
			case 'v':
				description = ui_open_vim(fetch_readme(name));
				move_on = true;
				break;
			case 'n':
				description = ui_open_nano(fetch_readme(name));
				move_on = true;
				break;
			default:
				break;
		}
	}
	// Rerender Everything after exiting editor
	refresh();
	clear();
	draw_overview(model);
	draw_detail(model);
	doupdate();

	wclear(model->add_text_window);
	draw_border(model->add_window, "Add");
	wrefresh(model->add_window);

	status = UI_RETRY;
	while (status == UI_RETRY) {
		mvwprintw(model->add_text_window, 1, 2, "Stars: ");
		draw_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		wmove(model->add_text_window, 1, 9);

		stars = ui_uint32_t_input(model->add_text_window, 1, 9);

		status = ui_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		if (status == UI_CANCEL) {
			free(name);
			free(description);
			free(url);
			return;
		}
	}

	wclear(model->add_text_window);
	status = UI_RETRY;
	while (status == UI_RETRY) {
		mvwprintw(model->add_text_window, 1, 2, "Issues: ");
		draw_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		wmove(model->add_text_window, 1, 10);

		issues = ui_uint32_t_input(model->add_text_window, 1, 10);

		status = ui_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		if (status == UI_CANCEL) {
			free(name);
			free(description);
			free(url);
			return;
		}
	}

	opensource_project *osp = create_opensource_project(name, description, url, stars, issues);
	if (osp == NULL) {
		free(name);
		free(description);
		free(url);
		return;
	}
	model->current = add_node(model->list, osp);
	model->view = model->previous_view;
}


void start_add_github_flow(struct Model *model) {
	// TODO: Implement
	model->view = model->previous_view;
}

void start_delete_flow(struct Model *model) {
	wclear(model->popup_window);
	opensource_project *osp = model->current->value;
	draw_border(model->popup_window, "Delete");
	mvwprintw(model->popup_window, 2, 2, "Are you sure you want to delete ");
	wattron(model->popup_window, A_BOLD);
	wprintw(model->popup_window, "%s", osp->name);
	wattroff(model->popup_window, A_BOLD);
	wprintw(model->popup_window, "?");

	draw_ok_cancel(model->popup_window, 5, 2, "OK", "Cancel");
	wrefresh(model->popup_window);
	ui_okcancel result;
	while ((result = ui_ok_cancel(model->popup_window, 5, 2, "OK", "Cancel")) == UI_RETRY) {
		// do nothing
	}
	if (result == UI_OK) {
		node *next = model->current->next;
		remove_node(model->list, model->current);
		model->current = next;
	}
	model->view = model->previous_view;
}

void start_open_flow(struct Model *model) {
	int ch;
	char *filename = NULL;
	char *mode_name;
	ui_okcancel ui_res = UI_RETRY;

	wclear(model->popup_window);
	draw_border(model->popup_window, "Open File");
	mvwprintw(model->popup_window, 2, 2, "a: append to list");
	mvwprintw(model->popup_window, 3, 2, "o: open new file");
	mvwprintw(model->popup_window, 4, 6, "(this will replace the current list without saving)");
	mvwprintw(model->popup_window, 5, 2, "q: cancel");

	bool selected = false;
	while (!selected && (ch = wgetch(model->popup_window)) != 'q') {
		if (ch == 'a') {
			mode_name = "Append from File";
			selected = true;
		} else if (ch == 'o') {
			mode_name = "Open File";
			selected = true;
		}
	}
	wclear(model->popup_window);

	draw_border(model->popup_window, mode_name);
	draw_ok_cancel(model->popup_window, 5, 2, "Ok", "Cancel");
	mvwprintw(model->popup_window, 2, 2, "File: ");

	while (ui_res == UI_RETRY) {
		filename = ui_text_input(model->popup_window, 2, 8, filename);
		ui_res = ui_ok_cancel(model->popup_window, 5, 2, "Ok", "Cancel");
		if (filename == NULL || strlen(filename) == 0 || !file_exists(filename)) {
			ui_res = UI_RETRY;
		}
	}
	linked_list *l = read_file(filename);
	// l does not need to be freed, as it is either merged into the current list or freed in the else branch
	if (ch == 'o' && l->head != NULL) {
		model->current = l->head;
		model->list = l;
		model->input_file = filename;
		return; // do not free filename
	} else if (l->head != NULL) {
		model->list = merge_lists(model->list, l);
	} else {
		wclear(model->popup_window);

		draw_border(model->popup_window, "Error");
		mvwprintw(model->popup_window, 2, 2, "The file %s didn't contain any objects", filename);

		mvwprintw(model->popup_window, 5, 2, "press any key to continue.");
		wgetch(model->popup_window);
		free_list(l);
	}
	free(filename);
}
