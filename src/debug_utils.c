#include "debug_utils.h"
#include <stdio.h>
void print_int(void *value) {
    printf("%d ", *(int *)value);
}