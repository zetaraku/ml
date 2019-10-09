#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "include/FuncExecContext.h"
#include "include/Value.h"
#include "include/utils.h"
#define TRUE_TEXT "#t"
#define FALSE_TEXT "#f"
#define MAX_ALLOCATED_VALUE (4096)

Value *allocated_value[MAX_ALLOCATED_VALUE]; size_t allocated_value_size = 0;
int val_allocated = 0, val_freed = 0;

Value *NIL = NULL;

Value *reg_alloc() {
	assert(allocated_value_size < MAX_ALLOCATED_VALUE && "MAX_ALLOCATED_VALUE exceed.");

	Value *value = (Value *) malloc(sizeof(Value));

	allocated_value[allocated_value_size++] = value;
	val_allocated++;

	return value;
}

void reg_unalloc(Value *value) {
	for (size_t i = 0; i < allocated_value_size; i++) {
		if (allocated_value[i] == value) {
			allocated_value[i] = allocated_value[allocated_value_size-1];
			allocated_value_size--;

			free(value);
			val_freed++;

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

void _Value__initializeNIL() {
	assert(NIL == NULL);
	NIL = reg_alloc(); {
		NIL->type = TYPE_CONS;
		NIL->_ = Cons__of(NIL, NIL);
	}
}
void _Value__releaseNIL() {
	assert(NIL != NULL);
	reg_unalloc(NIL);
}

Value *_new_Value(int type) {
	Value *thiz = reg_alloc(); {
		thiz->type = type;
		thiz->link_count = 0;
	}
	return thiz;
}
void _delete_Value(Value *thiz) {
	assert(thiz->link_count == 0);

	if (thiz->type == TYPE_SYMBOL) {
		free((void *)thiz->symval);
	}

	thiz->type = TYPE_UNDEFINED;

	reg_unalloc(thiz);
}
Value *Value_link(Value *thiz) {
	if (thiz == NIL)
		return thiz;

	thiz->link_count++;

	return thiz;
}
void Value_unlink(Value *thiz) {
	if (thiz == NIL)
		return;

	if (--thiz->link_count <= 0) {
		if (thiz->type == TYPE_CONS) {
			Value_unlink(thiz->_.left);
			Value_unlink(thiz->_.right);
		}
		_delete_Value(thiz);
	}
}

void Value__assign(Value **lv, Value *rv) {
	Value *old_val = *lv;
	*lv = Value_link(rv);
	Value_unlink(old_val);
}
void Value_print(Value *thiz) {
	if (thiz->type == TYPE_BOOL) {
		printf("%s ", thiz->boolval ? TRUE_TEXT : FALSE_TEXT);
	} else if (thiz->type == TYPE_NUM) {
		printf("%d ", thiz->numval);
	} else if (thiz->type == TYPE_SYMBOL) {
		printf("%s ", thiz->symval);
	} else if (thiz->type == TYPE_CONS) {
		if (thiz->_._closure != NULL)
			printf("#@%p", thiz->_._closure->symTable);
		if (thiz == NIL) {
			printf("NIL");
		} else {
			printf("( "); {
				Value *li = ListIterator_from(thiz), *v;
				while ((v = ListIterator_next(&li)) != NULL) {
					Value_print(v);
				}
			} printf(") ");
		}
	} else {
		printf("? ");
	}
}

static void _Value_print_raw_with_indent(Value *thiz, int indent_level) {
	for (size_t i = 0; i < indent_level; i++)
		putchar('\t');

	if (thiz == NIL) {
		printf("NIL (cons)\n");
	} else if (thiz->type == TYPE_BOOL) {
		printf("%s (bool)\n", thiz->boolval ? TRUE_TEXT : FALSE_TEXT);
	} else if (thiz->type == TYPE_NUM) {
		printf("%d (num)\n", thiz->numval);
	} else if (thiz->type == TYPE_SYMBOL) {
		printf("%s (symbol)\n", thiz->symval);
	} else if (thiz->type == TYPE_CONS) {
		printf("%p (cons)\n", thiz); {
			_Value_print_raw_with_indent(thiz->_.left, indent_level+1);
			_Value_print_raw_with_indent(thiz->_.right, indent_level+1);
		}
	} else {
		printf("%p (undefined)\n", thiz);
	}
}

void _Value_print_raw(Value *thiz) {
	_Value_print_raw_with_indent(thiz, 0);
}

// Value__fromXXX
	Value *Value__fromBool(Bool boolval) {
		Value *value = _new_Value(TYPE_BOOL);
		value->boolval = boolval;
		return value;
	}
	Value *Value__fromNum(Num numval) {
		Value *value = _new_Value(TYPE_NUM);
		value->numval = numval;
		return value;
	}
	Value *Value__fromSymbol(const char *symval) {
		Value *value = _new_Value(TYPE_SYMBOL);
		value->symval = strdup(symval);
		return value;
	}
	Value *Value__fromCons(Cons consval) {
		Value *value = _new_Value(TYPE_CONS);
		value->_ = consval;
		return value;
	}
// Value__getXXX
	Bool Value_getBool(Value *thiz) {
		assert(thiz->type == TYPE_BOOL);
		return thiz->boolval;
	}
	Num Value_getNum(Value *thiz) {
		assert(thiz->type == TYPE_NUM);
		return thiz->numval;
	}
	Symbol Value_getSymbol(Value *thiz) {
		assert(thiz->type == TYPE_SYMBOL);
		return thiz->symval;
	}
	Cons Value_getCons(Value *thiz) {
		assert(thiz->type == TYPE_CONS);
		return thiz->_;
	}
// Value__extractXXX
	Bool Value__extractBool(Value *value) {
		Bool ret = Value_getBool(value);
		Value_unlink(value);
		return ret;
	}
	Num Value__extractNum(Value *value) {
		Num ret = Value_getNum(value);
		Value_unlink(value);
		return ret;
	}
	Symbol Value__extractSymbol(Value *value) {
		Symbol ret = Value_getSymbol(value);
		Value_unlink(value);
		return ret;
	}
	Cons Value__extractCons(Value *value) {
		assert(false);
		// Cons ret = Value_getCons(value);
		// Value_unlink(value);
		// return ret;
	}

Value *Value_copy(Value *thiz) {
	if (false) {
		;
	} else if (thiz->type == TYPE_SYMBOL) {
		return Value__fromSymbol(thiz->symval);
	} else if (thiz->type == TYPE_BOOL) {
		return Value__fromBool(thiz->boolval);
	} else if (thiz->type == TYPE_NUM) {
		return Value__fromNum(thiz->numval);
	} else if (thiz->type == TYPE_CONS) {
		return Value__fromCons(thiz->_);
	}
	assert(false && "TypeError: this type is unavailible for cloning.");
}
