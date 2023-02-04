//
// Created by jasper on 04.02.23.
//

#ifndef PROJEKT_DATA_COMPARE_H
#define PROJEKT_DATA_COMPARE_H

enum SortBy {
	NAME,
	URL,
	STARS,
	ISSUES,
};
enum SortOrder {
	ASC,
	DESC,
};

extern int (*sort_fn_arr[4][2])(const void *, const void *);

#define get_sort_fn(field, order) sort_fn_arr[field][order]

int compare_name_asc(const void *a, const void *b);

int compare_name_desc(const void *a, const void *b);

int compare_url_asc(const void *a, const void *b);

int compare_url_desc(const void *a, const void *b);

int compare_stars_asc(const void *a, const void *b);

int compare_stars_desc(const void *a, const void *b);

int compare_issues_asc(const void *a, const void *b);

int compare_issues_desc(const void *a, const void *b);

#endif //PROJEKT_DATA_COMPARE_H
