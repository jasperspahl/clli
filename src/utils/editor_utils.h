//
// Created by jasper on 02.02.23.
//

#ifndef PROJEKT_EDITOR_UTILS_H
#define PROJEKT_EDITOR_UTILS_H

/**
 * Opens the editor and returns the edited text.
 * @param text The text to be edited.
 * @param editor The editor to be used.
 * @return The edited text.
 */
char *editor_edit(char *text, const char *editor);

/**
 * Saves the text to a file.
 * @param text The text to be saved.
 * @param filename The filename to be used.
 */
void editor_save(char *text, const char *filename);

/**
 * Loads the text from a file.
 * @param filename The filename to be used.
 * @return The text.
 */
char *editor_load(const char *filename);

#endif //PROJEKT_EDITOR_UTILS_H
