#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>

Cons Cons__of(Value *left, Value *right);
Value *List__of(size_t list_size, ...);
Value *List__isList(Value *value);
Value *List__getValueAt(Value *cons_value, size_t index);
Value *ListIterator_from(Value *begin);
Value *ListIterator_next(Value **li);

#endif
