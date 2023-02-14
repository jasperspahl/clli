//
// Created by jasper on 14.02.23.
//

#ifndef PROJEKT_COMMANDS_H
#define PROJEKT_COMMANDS_H

#include "ui.h"
void handle_command(struct Model *model, char * command_input);

/**
 * Write the list from the model to a file.
 * the filename should be specified in argv[1]
 * @note no spaces allowed in filenames
 * @param model the draw model
 * @param argc length of the arguments
 * @param argv Array of arguments
 */
void write_command(struct Model *model, size_t argc, char **argv);
void open_command(struct Model *model, size_t argc, char **argv);
void quit_command(struct Model *model, size_t argc, char **argv);

#endif //PROJEKT_COMMANDS_H
