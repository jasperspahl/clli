//
// Created by jasper on 02.02.23.
//

#include "utils/fetch_readme.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s <repo>\n", argv[0]);
		return 1;
	}
	char *repo = argv[1];
	char *readme = fetch_readme(repo);
	if (readme == NULL) {
		printf("Could not fetch README.md from %s\n", repo);
		return 1;
	}
	printf("%s", readme);
	free(readme);
	return 0;
}