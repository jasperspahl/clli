//
// Created by jasper on 02.02.23.
//

#include "files.h"

#include <sys/stat.h>

bool file_exists(char *filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}
