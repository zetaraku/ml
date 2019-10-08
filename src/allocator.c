#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "value.h"
#define MAX_ALLOCATED_VALUE (1024)

Value *allocated_value[MAX_ALLOCATED_VALUE]; size_t allocated_value_size = 0;
int val_allocated = 0, val_freed = 0;

Value *reg_alloc() {
	assert(allocated_value_size < MAX_ALLOCATED_VALUE && "MAX_ALLOCATED_VALUE exceed.");

	Value *value = (Value *)malloc(sizeof(Value));

	allocated_value[allocated_value_size++] = value;
	val_allocated++;

	return value;
}

void reg_unalloc(Value *value) {
	for (size_t i = 0; i < allocated_value_size; i++) {
		if (allocated_value[i] == value) {
			allocated_value[i] = allocated_value[allocated_value_size-1];
			allocated_value_size--;
			val_freed++;

			free(value);

			return;
		}
	}
	assert(false);
}

void reg_view_alloc() {
	printf("unfreed:\n");
	for (size_t i = 0; i < allocated_value_size; i++)
		_Value_print_raw(allocated_value[i]);
}

void reg_show_usage() {
	printf("allocated: %d, freed: %d, unfreed: %d\n",
		val_allocated, val_freed, val_allocated - val_freed
	);
}
