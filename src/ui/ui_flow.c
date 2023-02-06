#include "ui_flow.h"
#include "ui.h"
#include "utils/fetch_readme.h"
#include "data/data.h"
#include "utils/files.h"
#include "data/file_parsing.h"
#include "data/data_compare.h"
#include "data/search.h"
#include "utils/string_utils.h"

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
	// TODO: Implement add form github flow (starts, issues)
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

void start_search_flow(struct Model *model) {
	wclear(model->search_window);
	draw_border(model->search_window, "Search");
	mvwaddch(model->search_window, 4, 0, ACS_SSSB); // add ascii line char with ends on top, right, bottom
	mvwaddch(model->search_window, 4, getmaxx(model->search_window)-1, ACS_SBSS);
	mvwhline(model->search_text_window, 3, 0, ACS_HLINE, getmaxx(model->search_text_window));
	draw_box(model->search_text_window, 0, 1, 3, getmaxx(model->search_text_window)-2);

	wrefresh(model->search_window);
	wmove(model->search_text_window, 1, 3);
	curs_set(2);

	const int search_bar_width = getmaxx(model->search_text_window) - 6;

	int ch;
	int term_length = 0;
	char * search_term = malloc(1);
	search_term[0]='\0';
	search_res res;
	bool is_typing = true;
	node * selected = NULL;
	int selected_index = 0;
	while ((ch = wgetch(model->search_text_window)) != '\n') {
		if (is_typing) {
			if (ch == '\t') {
				is_typing = false;
				curs_set(0);
			} else if (ch == KEY_BACKSPACE || ch == 127) {
				if (term_length > 0) {
					search_term = realloc(search_term, term_length--);
					search_term[term_length] = '\0';
					if (term_length > search_bar_width) {
						mvwprintw(model->search_text_window, 1, 3 + search_bar_width, " ");
					} else if (term_length == search_bar_width) {
						mvwprintw(model->search_text_window, 1, 2, " %s", search_term);
					} else
					{
						mvwprintw(model->search_text_window, 1, 3 + term_length, " ");
					}
				}
			} else if (ch >= 32 && ch <= 126) {
				search_term = realloc(search_term, ++term_length + 1);
				search_term[term_length] = '\0';
				search_term[term_length - 1] = (char) ch;
				if (term_length > search_bar_width) {
					mvwprintw(model->search_text_window, 1, 2, "%s", &search_term[term_length-search_bar_width]);
				} else {
					mvwprintw(model->search_text_window, 1, 3, "%s", search_term);
				}
			} else {
				continue; // don't search again and also no redraw
			}
			res = search(model->list, search_term, search_score_fn);
			sort_search_res(&res);
			selected_index = 0;
			if (res.size > 0) {
				selected = res.searchResults[selected_index].node;
			}
		} else {
			if (ch == '\t') {
				is_typing = true;
				curs_set(2);
			} else if (ch == 'j' || ch == KEY_DOWN) {
				if (selected_index < (int)res.size-1) selected_index++;
			} else if (ch == 'k' || ch == KEY_UP) {
				if (selected_index > 0 ) selected_index--;
			} else if (ch == 'q') {
				break; // cancel search
			} else {
				continue; // no redraw needed
			}
			if (res.size > 0) {
				selected = res.searchResults[selected_index].node;
			}
		}

		int max_lines = getmaxy(model->search_text_window) - 4; // (the search bar on top needs 4 lines
		clear_rect(model->search_text_window, 4, 0,
				   max_lines, getmaxx(model->search_text_window));

		mvwprintw(model->search_text_window, 4, 2, "No results found!"); // this will be overwritten if res are found

		int offset = selected_index - max_lines + 1;
		if (offset < 0) offset = 0;
		int match_in_name_at;
		for (int i = offset; i < (int)res.size && i < max_lines+offset; i++) {
			opensource_project * osp = (opensource_project *) res.searchResults[i].node->value;

			match_in_name_at = find_first_string_like(osp->name, search_term);
			if(!is_typing && i == selected_index) {
				wattron(model->search_text_window, A_REVERSE);
			}
			mvwhline(model->search_text_window, 4+i-offset, 0, ' ', getmaxx(model->search_text_window));
			if (match_in_name_at >= 0) {
				// highlight the search_term in name if found
				mvwprintw(model->search_text_window, 4+i-offset, 2, "%.*s",match_in_name_at, osp->name);
				wattron(model->search_text_window, A_BOLD);
				mvwprintw(model->search_text_window, 4+i-offset, 2+match_in_name_at,
						  "%.*s", term_length, osp->name + match_in_name_at);
				wattroff(model->search_text_window, A_BOLD);
				mvwprintw(model->search_text_window, 4+i-offset, 2+match_in_name_at+term_length,
				          "%s",osp->name + match_in_name_at + term_length);
			} else{
				mvwprintw(model->search_text_window, 4+i-offset, 2, "%s", osp->name);
			}
			if(!is_typing && i == selected_index) {
				wattroff(model->search_text_window, A_REVERSE);
			}
		}
		// reset the cursor to the search bar if typing
		if (is_typing) {
			wmove(model->search_text_window, 1, 3 + (term_length > search_bar_width ? search_bar_width : term_length));
		}
	}
	model->current = selected;
	curs_set(0);
	noecho();
}
