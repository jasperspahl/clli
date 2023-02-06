//
// Created by jasper on 02.02.23.
//

#ifndef PROJEKT_UI_FLOW_H
#define PROJEKT_UI_FLOW_H

#include "ui.h"

/**
 * Draws the add view and start the add flow.
 * @param model The model to update.
 */
void start_add_flow(struct Model *model);

void start_add_manual_flow(struct Model *model);

void start_add_github_flow(struct Model *model);

void start_delete_flow(struct Model *model);

void start_open_flow(struct Model *model);

void start_sort_flow(struct Model *model);

void start_search_flow(struct Model *model);

#endif //PROJEKT_UI_FLOW_H
