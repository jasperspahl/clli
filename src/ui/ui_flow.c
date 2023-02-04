#include "ui_flow.h"
#include "ui.h"
#include "utils/fetch_readme.h"
#include "data/data.h"
#include "utils/files.h"
#include "data/file_parsing.h"
#include "data/data_compare.h"

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
	bool loop_again = true;
	int ch = wgetch(model->add_window);
	while (loop_again && ch != 'q') {
		switch (ch) {
			case 'a':
				start_add_manual_flow(model);
				loop_again = false;
				break;
			case 'g':
				start_add_github_flow(model);
				loop_again = false;
				break;
			default:
				ch = wgetch(model->add_window);
				break;
		}
	}
	model->view = model->previous_view;
}

void start_add_manual_flow(struct Model *model) {
	int ch;
	char *name, *url, *description;
	name = url = NULL;
	uint32_t stars, issues;
	ui_okcancel status = UI_RETRY;
	wclear(model->add_text_window);

	while (status == UI_RETRY) {
		mvwprintw(model->add_text_window, 1, 2, "Name: ");
		draw_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		wmove(model->add_text_window, 1, 8);

		name = ui_text_input(model->add_text_window, 1, 8, name);

		status = ui_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		if (status == UI_CANCEL) {
			free(name);
			return;
		}
		if (name == NULL || strlen(name) == 0) {
			status = UI_RETRY;
			continue;
		}
	}
	description = fetch_readme(name);

	url = malloc(sizeof("https://github.com/") + strlen(name));
	sprintf(url, "https://github.com/%s", name);
	wclear(model->add_text_window);
	status = UI_RETRY;
	while (status == UI_RETRY) {
		mvwprintw(model->add_text_window, 1, 2, "Url: ");
		draw_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		wmove(model->add_text_window, 1, 7);

		url = ui_text_input(model->add_text_window, 1, 7, url);

		status = ui_ok_cancel(model->add_text_window, 4, 2, "OK", "Cancel");
		if (status == UI_CANCEL) {
			if (description != NULL) {
				free(description);
			}
			free(name);
			free(url);
			return;
		}
		if (url == NULL || strlen(url) == 0) {
			status = UI_RETRY;
			continue;
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
				description = ui_open_vim(description);
				move_on = true;
				break;
			case 'n':
				description = ui_open_nano(description);
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
}

void start_add_github_flow(struct Model *model) {
	// TODO: Implement
	model->view = model->previous_view;
}

void start_delete_flow(struct Model *model) {
	wclear(model->popup_window);
	if (model->current == NULL) return;
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
		node *next = model->current->next;      /*******************************************************/
		if (next == NULL) {                     /* This keeps the current on the same index except if  */
			next = model->current->previous;    /* the node to delete is the last of the list          */
		}                                       /*******************************************************/
		remove_node_free(model->list, model->current);
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
	if (!selected) {
		return;// user cancelled
	}
	wclear(model->popup_window);

	draw_border(model->popup_window, mode_name);
	while (ui_res == UI_RETRY) {
		draw_ok_cancel(model->popup_window, 5, 2, "Ok", "Cancel");
		mvwprintw(model->popup_window, 2, 2, "File: ");

		filename = ui_text_input(model->popup_window, 2, 8, filename);
		ui_res = ui_ok_cancel(model->popup_window, 5, 2, "Ok", "Cancel");
		if (ui_res == UI_CANCEL) {
			if (filename != NULL) free(filename);
			return;
		}
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
	if (filename != NULL) free(filename);
}

void start_sort_flow(struct Model *model) {
	wclear(model->popup_window);

	draw_border(model->popup_window, "Sort by");
	mvwprintw(model->popup_window, 1, 4, "n: Name");
	mvwprintw(model->popup_window, 2, 4, "u: Url");
	mvwprintw(model->popup_window, 3, 4, "s: Stars");
	mvwprintw(model->popup_window, 4, 4, "i: Issues");
	mvwprintw(model->popup_window, 6, 4, "q: Cancel");


	int ch;
	enum SortBy sb;
	bool selected = false;
	while (!selected && (ch = wgetch(model->popup_window)) != 'q') {
		switch (ch) {
			case 'n':
				sb = NAME;
				selected = true;
				break;
			case 'u':
				sb = URL;
				selected = true;
				break;
			case 's':
				sb = STARS;
				selected = true;
				break;
			case 'i':
				sb = ISSUES;
				selected = true;
				break;
			default:
				break;
		}
	}
	if (!selected) {
		return;// user cancelled
	}

	wclear(model->popup_window);

	draw_border(model->popup_window, "Order by");
	mvwprintw(model->popup_window, 2, 4, "a: Ascending");
	mvwprintw(model->popup_window, 3, 4, "d: Descending");
	mvwprintw(model->popup_window, 6, 4, "q: cancel");

	enum SortOrder so;
	selected = false;
	while (!selected && (ch = wgetch(model->popup_window)) != 'q') {
		switch (ch) {
			case 'a':
				so = ASC;
				selected = true;
				break;
			case 'd':
				so = DESC;
				selected = true;
				break;
			default:
				break;
		}
	}
	if (!selected) {
		return;// user cancelled
	}

	sort_list(model->list, get_sort_fn(sb, so));
}
