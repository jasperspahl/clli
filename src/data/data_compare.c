#include "data_compare.h"
#include "data.h"
#include "utils/string_utils.h"

int (*sort_fn_arr[4][2])(const void *, const void *) = {
		[NAME] = {
				[ASC] = compare_name_asc,
				[DESC] = compare_name_desc,
		},
		[URL] = {
				[ASC] = compare_url_asc,
				[DESC] = compare_url_desc,
		},
		[STARS] = {
				[ASC] = compare_stars_asc,
				[DESC] = compare_stars_desc,
		},
		[ISSUES] = {
				[ASC] = compare_issues_asc,
				[DESC] = compare_issues_desc,
		},
};

int compare_name_asc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return stringcompare(proj_a->name, proj_b->name);
}

int compare_name_desc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return stringcompare(proj_b->name, proj_a->name);
}

int compare_url_asc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return stringcompare(proj_a->url, proj_b->url);
}

int compare_url_desc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return stringcompare(proj_b->url, proj_a->url);
}

int compare_stars_asc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return (int) proj_a->stars - (int) proj_b->stars;
}

int compare_stars_desc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return (int) proj_b->stars - (int) proj_a->stars;
}

int compare_issues_asc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return (int) proj_a->issues - (int) proj_b->issues;
}

int compare_issues_desc(const void *a, const void *b) {
	opensource_project *proj_a = (opensource_project *) a;
	opensource_project *proj_b = (opensource_project *) b;
	return (int) proj_b->issues - (int) proj_a->issues;
}

int search_score_fn(void *data, char *search_term) {
	return 0;
}
