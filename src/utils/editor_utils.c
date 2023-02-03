//
// Created by jasper on 02.02.23.
//

#include "editor_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

char *editor_edit(char *text, const char *editor) {
	if (editor == NULL) {
		editor = getenv("EDITOR");
	}
	if (editor == NULL) {
		editor = "vi";
	}
	srand((unsigned int) time(NULL));
	// Create a temporary file
	char filename[] = "/tmp/clli-XXXXXX-XXXXXX";
	filename[11] = (char) (48 + (rand() % 10));
	filename[12] = (char) (48 + (rand() % 10));
	filename[13] = (char) (48 + (rand() % 10));
	filename[14] = (char) (48 + (rand() % 10));
	filename[15] = (char) (48 + (rand() % 10));
	filename[17] = (char) (65 + (rand() % 26));
	filename[18] = (char) (65 + (rand() % 26));
	filename[19] = (char) (65 + (rand() % 26));
	filename[20] = (char) (65 + (rand() % 26));
	filename[21] = (char) (65 + (rand() % 26));
	filename[22] = (char) (65 + (rand() % 26));
	editor_save(text, filename);
	char *command = malloc(strlen(editor) + strlen(filename) + 2);
	sprintf(command, "%s %s", editor, filename);
	if (0 == fork()) {
		execlp("sh", "sh", "-c", command, NULL);
		exit(0);
	}
	free(command);
	wait(NULL);
	return editor_load(filename);
}

void editor_save(char *text, const char *filename) {

	FILE *file = fopen(filename, "w");
	if (file == NULL) {
		printf("Could not open file %s: %s\n", filename, strerror(errno));
		exit(1);
	}
	if (text == NULL) {
		text = "";
	}
	fprintf(file, "%s", text);
	fclose(file);
}

char *editor_load(const char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		printf("Could not open file %s: %s\n", filename, strerror(errno));
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *new_text = malloc(size + 1);
	fread(new_text, 1, size, file);
	new_text[size] = '\0';
	return new_text;
}
