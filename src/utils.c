#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "include/Value.h"
#include "include/utils.h"

Cons Cons__of(Value *left, Value *right) {
	Cons cons; {
		cons.left = left;
		cons.right = right;
		cons._closure = NULL;
	}
	return cons;
}

Value *List__of(size_t list_size, ...) {
	Value *dummy_head = Value__fromCons(Cons__of(NIL, NIL));
	Value *current = dummy_head;

	va_list arg_ptr; va_start(arg_ptr, list_size); {
		for (size_t i = 0; i < list_size; i++) {
			Value *value = va_arg(arg_ptr, Value *);
			current = current->_.right = Value__fromCons(Cons__of(value, NIL));
		}
	} va_end(arg_ptr);

	Value *ret = dummy_head->_.right;
	Value_unlink(dummy_head);

	return ret;
}
Value *List__isList(Value *value) {
	// for (Value *v = value; v != NIL; v = v->_.right) {
	// 	if (v->type != TYPE_CONS)
	// 		return Value__fromBool(false);
	// }
	// return Value__fromBool(true);
	return Value__fromBool(value->type == TYPE_CONS);
}
Value *List__getValueAt(Value *cons_value, size_t index) {
	assert(cons_value->type == TYPE_CONS && "Type error: not a list." );
	size_t i = 0;
	for (Value *cv = cons_value; cv != NIL; cv = cv->_.right, i++) {
		if (i == index)
			return cv->_.left;
		assert(cv->_.right->type == TYPE_CONS && "Type error: not a list." );
	}
	assert(0 && "child out of range.");
}

Value *ListIterator_from(Value *begin) {
	assert(begin->type == TYPE_CONS && "Type error: not a list." );
	return begin;
}
Value *ListIterator_next(Value **li) {
	if ((*li) == NIL) {
		return NULL;
	}
	Value *ret = (*li)->_.left;
	(*li) = (*li)->_.right;
	return ret;
}
