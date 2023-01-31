#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

void usage(char *program_name) {
    printf("Usage: %s <filename>\n", program_name);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage(argv[0]);
    }

    return 0;
}
