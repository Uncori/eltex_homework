#ifndef SRC_FUNCTIONS_H
#define SRC_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct subscriber {
    char *second_name;
    char *first_name;
    char *last_name;
    char *phone_number;
} subscriber;

typedef struct directory {
    unsigned int count_subscriber;
    subscriber *subscriber;
} directory;

void memory_allocation_directory(directory *data);
void remove_directory(directory *data);
void print_subscriber(directory *data);
void print_subscriber_i(directory *data, unsigned int *i);
void second_menu(directory *data);
void second_find(directory *data);
void first_find(directory *data);
void last_find(directory *data);
void phone_find(directory *data);
void input_directory(directory *data);

#endif  // SRC_FUNCTIONS_H
