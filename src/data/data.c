#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "linked_list.h"

void free_opensource_project(void *value) {
	opensource_project *project = value;
	free(project->name);
	free(project->description);
	free(project->url);
	free(project);
}

opensource_project *
create_opensource_project(char *name, char *description, char *url, uint32_t stars, uint32_t issues) {
	opensource_project *project = malloc(sizeof(opensource_project));
	*project = (opensource_project) {
			.name = name,
			.description = description,
			.url = url,
			.stars = stars,
			.issues = issues,
	};
	return project;
}

void serialize_opensource_project(void *value, FILE *file) {
	opensource_project *project = value;
	if (project == NULL) {
		return;
	}
	size_t name_length, description_length, url_length;
	name_length = description_length = url_length = 0;
	if (project->name != NULL) {
		name_length = strlen(project->name);
	}
	if (project->description != NULL) {
		description_length = strlen(project->description);
	}
	if (project->url != NULL) {
		url_length = strlen(project->url);
	}
	fwrite(&name_length, sizeof(size_t), 1, file);
	if (name_length > 0) {
		fwrite(project->name, sizeof(char), strlen(project->name), file);
	}
	fwrite(&description_length, sizeof(size_t), 1, file);
	if (description_length > 0) {
		fwrite(project->description, sizeof(char), strlen(project->description), file);
	}
	fwrite(&url_length, sizeof(size_t), 1, file);
	if (url_length > 0) {
		fwrite(project->url, sizeof(char), strlen(project->url), file);
	}
	fwrite(&project->stars, sizeof(uint32_t), 1, file);
	fwrite(&project->issues, sizeof(uint32_t), 1, file);
}

opensource_project *deserialize_opensource_project(FILE *file) {
	// Allocate memory for the project
	opensource_project *project = malloc(sizeof(opensource_project));
	size_t name_length, description_length, url_length;
	// Read length of name, allocate memory for name and read name
	if (fread(&name_length, sizeof(size_t), 1, file) != 1) {
		free(project);
		return NULL;
	}
	if (name_length > 0) {
		project->name = malloc(sizeof(char) * name_length);
		if (fread(project->name, sizeof(char), name_length, file) != name_length) {
			free(project->name);
			free(project);
			return NULL;
		}
	}
	// Read length of description, allocate memory for description and read description
	if (fread(&description_length, sizeof(size_t), 1, file) != 1) {
		free(project->name);
		free(project);
		return NULL;
	}
	if (description_length > 0) {
		project->description = malloc(sizeof(char) * description_length);
		if (fread(project->description, sizeof(char), description_length, file) != description_length) {
			free(project->name);
			free(project->description);
			free(project);
			return NULL;
		}
	}
	// Read length of url, allocate memory for url and read url
	if (fread(&url_length, sizeof(size_t), 1, file) != 1) {
		free(project->name);
		free(project->description);
		free(project);
		return NULL;
	}
	if (url_length > 0) {
		project->url = malloc(sizeof(char) * url_length);
		if (fread(project->url, sizeof(char), url_length, file) != url_length) {
			free(project->name);
			free(project->description);
			free(project->url);
			free(project);
			return NULL;
		}
	}
	// Read stars and issues
	fread(&project->stars, sizeof(uint32_t), 1, file);
	fread(&project->issues, sizeof(uint32_t), 1, file);
	return project;
}

void print_opensource_project(void *value) {
	opensource_project *project = value;
	printf("Name: %s\n", project->name);
	printf("Description: %s\n", project->description);
	printf("URL: %s\n", project->url);
	printf("Stars: %d\n", project->stars);
	printf("Issues: %d\n", project->issues);
}
