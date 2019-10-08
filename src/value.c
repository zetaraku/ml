#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "allocator.h"
#include "context_stack.h"
#include "value.h"
#include "utils.h"
#define TRUE_TEXT "#t"
#define FALSE_TEXT "#f"

Value *NIL = NULL;

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

static Value *func_call(Value *func, Value *argument_list);
Value *Value__eval(Value *value) {
	if (false) {
		;
	} else if (value->type == TYPE_CONS) {	// function call / macro call
		if (value == NIL)
			return NIL;

		Value *opvalue = List__getValueAt(value, 0);
		if (opvalue->type == TYPE_SYMBOL) {
			Symbol op_name = Value_getSymbol(opvalue);
			if (false) {
				;
			} else if (strcmp(op_name, "list") == 0) {
				Value *ret_dummy_head = Value__fromCons(Cons__of(NIL, NIL));
				Value *ret_current_node = ret_dummy_head;

				// eval
				Value *li = ListIterator_from(value->_.right), *v;
				while ((v = ListIterator_next(&li)) != NULL) {
					ret_current_node = ret_current_node->_.right =
						Value__fromCons(Cons__of(
							Value__eval(v), NIL
						));
				}

				Value *ret = ret_dummy_head->_.right;
				Value_unlink(ret_dummy_head);

				return ret;
			} else if (strcmp(op_name, "+") == 0) {
				Num num = 0;
				for (Value *v=value->_.right; v!=NIL; v=v->_.right)
					num += Value__extractNum(Value__eval(v->_.left));
				return Value__fromNum(num);
			} else if (strcmp(op_name, "-") == 0) {
				Num num =
					  Value__extractNum(Value__eval(List__getValueAt(value, 1)))
					- Value__extractNum(Value__eval(List__getValueAt(value, 2)))
				;
				return Value__fromNum(num);
			} else if (strcmp(op_name, "*") == 0) {
				Num num = 1;
				for (Value *v=value->_.right; v!=NIL; v=v->_.right)
					num *= Value__extractNum(Value__eval(v->_.left));
				return Value__fromNum(num);
			} else if (strcmp(op_name, "/") == 0) {
				Num num =
					  Value__extractNum(Value__eval(List__getValueAt(value, 1)))
					/ Value__extractNum(Value__eval(List__getValueAt(value, 2)))
				;
				return Value__fromNum(num);
			} else if (strcmp(op_name, "mod") == 0) {
				Num num =
					  Value__extractNum(Value__eval(List__getValueAt(value, 1)))
					% Value__extractNum(Value__eval(List__getValueAt(value, 2)))
				;
				return Value__fromNum(num);
			} else if (strcmp(op_name, ">") == 0) {
				Bool boolean =
					  Value__extractNum(Value__eval(List__getValueAt(value, 1)))
					> Value__extractNum(Value__eval(List__getValueAt(value, 2)))
				;
				return Value__fromBool(boolean);
			} else if (strcmp(op_name, "<") == 0) {
				Bool boolean =
					  Value__extractNum(Value__eval(List__getValueAt(value, 1)))
					< Value__extractNum(Value__eval(List__getValueAt(value, 2)))
				;
				return Value__fromBool(boolean);
			} else if (strcmp(op_name, "=") == 0) {
				Bool boolean =
					   Value__extractNum(Value__eval(List__getValueAt(value, 1)))
					== Value__extractNum(Value__eval(List__getValueAt(value, 2)))
				;
				return Value__fromBool(boolean);
			} else if (strcmp(op_name, "and") == 0) {		// macro
				Bool boolean = true;
				for (Value *v=value->_.right; v!=NIL; v=v->_.right) {
					boolean = boolean && Value__extractBool(Value__eval(v->_.left));
					if (!boolean)
						break;
				}
				return Value__fromBool(boolean);
			} else if (strcmp(op_name, "or") == 0) {			// macro
				Bool boolean = false;
				for (Value *v=value->_.right; v!=NIL; v=v->_.right) {
					boolean = boolean || Value__extractBool(Value__eval(v->_.left));
					if (boolean)
						break;
				}
				return Value__fromBool(boolean);
			} else if (strcmp(op_name, "not") == 0) {
				Bool boolean = !Value__extractBool(Value__eval(List__getValueAt(value, 1)));
				return Value__fromBool(boolean);
			// } else if (strcmp(op_name, "print-num") == 0) {
			// 	int num = Value__extractNum(Value__eval(List__getValueAt(value, 1)));
			// 	printf("%d\n", num);
			// 	return NIL;
			// } else if (strcmp(op_name, "print-bool") == 0) {
			// 	Bool boolean = Value__extractBool(Value__eval(List__getValueAt(value, 1)));
			// 	printf("%s\n", boolean ? "#t" : "#f");
			// 	return NIL;
			} else if (strcmp(op_name, "print") == 0) {
				for (Value *li=value->_.right; li!=NIL; li=li->_.right) {
					Value *val = Value__eval(li->_.left);
					Value_print(val);
					putchar('\n');
					Value_unlink(val);
				}
				return NIL;
			} else if (strcmp(op_name, "if") == 0) {			// macro
				Bool boolean = Value__extractBool(Value__eval(List__getValueAt(value, 1)));
				if (boolean)
					return Value__eval(List__getValueAt(value, 2));
				else
					return Value__eval(List__getValueAt(value, 3));
			} else if (strcmp(op_name, "quote") == 0) {												// macro
				return Value_copy(List__getValueAt(value, 1));//?
			} else if (strcmp(op_name, "lambda") == 0) {
				Value *func = Value_copy(value);
				assert(List__isList(func));
				func->_._closure = CURRENT_EXEC;
				return func;
			} else if (strcmp(op_name, "let") == 0) {		// macro
				Value *def_list = List__getValueAt(value, 1);

				Value *retv = NIL;

				FuncExecContext *ctx = new_FuncExecContext(CURRENT_EXEC);
				PUSH_EXEC_CTX(ctx); {
					Value *dli = ListIterator_from(def_list), *dv;
					while ((dv = ListIterator_next(&dli)) != NULL) {
						Value *v;
						SymTable_insert(ctx->symTable,
							List__getValueAt(dv, 0),
							v=Value__eval(List__getValueAt(dv, 1))
						);
						Value_unlink(v);
					}

					Value *li=ListIterator_from(value->_.right->_.right), *v;
					while ((v = ListIterator_next(&li)) != NULL) {
						Value_unlink(retv);
						retv = Value__eval(v);
					}
				} POP_EXEC_CTX();

				return retv;
			} else if (strcmp(op_name, "define") == 0) {	// not recommend
				Value *v;
				SymTable_insert(CURRENT_EXEC->symTable,
					List__getValueAt(value, 1),
					v=Value__eval(List__getValueAt(value, 2))
				);
				Value_unlink(v);
				return NIL;
			} else {								// symbol function call
				Value *funcv = FuncExecContext_deref(CURRENT_EXEC, opvalue);
				Value *retv = func_call(funcv, value->_.right);
				Value_unlink(funcv);
				return retv;
			}
		} else if (opvalue->type == TYPE_CONS) {	// lambda funcion call
			Value *funcv = Value__eval(opvalue);
			Value *retv = func_call(funcv, value->_.right);
			Value_unlink(funcv);
			return retv;
		} else {
			assert(0 && "TypeError: Head is not Function.");
		}
	} else if (value->type == TYPE_SYMBOL) {
		return FuncExecContext_deref(CURRENT_EXEC, value);
	} else if (value->type == TYPE_BOOL) {
		return Value_copy(value);
	} else if (value->type == TYPE_NUM) {
		return Value_copy(value);
	}
}

static Value *func_call(Value *func, Value *argument_list) {
	#ifdef DEBUG
	{
		printf("calling function:\n");
			Value_print(func);
		putchar('\n');
		printf("with arguments:\n");
			Value_print(argument_list);
		putchar('\n');
	}
	#endif

	Value *param_list = List__getValueAt(func, 1);

	assert(List__isList(param_list));
	assert(List__isList(argument_list));


	FuncExecContext *ctx = new_FuncExecContext(func->_._closure);

	// evaluate arguments and fill into parameters
	Value *pli = ListIterator_from(param_list), *pv;
	Value *ali = ListIterator_from(argument_list), *av;
	while ((pv = ListIterator_next(&pli)) != NULL) {
		av = ListIterator_next(&ali);
		SymTable_insert(ctx->symTable,
			pv, Value__eval(av != NULL ? av : NIL)
		);
	}

	Value *retv = NIL;
	PUSH_EXEC_CTX(ctx); {
		// evaluate each list item of function body
		Value *body_list = func->_.right->_.right;
		Value *li = ListIterator_from(body_list), *v;
		while ((v = ListIterator_next(&li)) != NULL) {
			Value_unlink(retv);
			retv = Value__eval(v);
		}
	} POP_EXEC_CTX();

	return retv;
}
