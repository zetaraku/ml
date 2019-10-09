// #include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "include/FuncExecContext.h"
#include "include/Value.h"
#include "include/utils.h"

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
			} else if (strcmp(op_name, "quote") == 0) {			// macro
				return Value_copy(List__getValueAt(value, 1));	//?
			} else if (strcmp(op_name, "lambda") == 0) {
				Value *func = Value_copy(value);
				assert(List__isList(func));
				func->_._closure = CURRENT_EXEC_CTX;
				return func;
			} else if (strcmp(op_name, "let") == 0) {			// macro
				Value *def_list = List__getValueAt(value, 1);

				Value *retv = NIL;

				FuncExecContext *ctx = new_FuncExecContext(CURRENT_EXEC_CTX);
				PUSH_EXEC_CTX(ctx); {
					Value *dli = ListIterator_from(def_list), *dv;
					while ((dv = ListIterator_next(&dli)) != NULL) {
						Value *v;
						SymbolTable_insert(ctx->symTable,
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
				} ctx = POP_EXEC_CTX();

				return retv;
			} else if (strcmp(op_name, "define") == 0) {	// not recommend
				Value *v;
				SymbolTable_insert(CURRENT_EXEC_CTX->symTable,
					List__getValueAt(value, 1),
					v=Value__eval(List__getValueAt(value, 2))
				);
				Value_unlink(v);
				return NIL;
			} else {								// symbol function call
				Value *funcv = FuncExecContext_deref(CURRENT_EXEC_CTX, opvalue);
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
			fprintf(stderr, "TypeError: list head is uncallable.");
			return NIL;
		}
	} else if (value->type == TYPE_SYMBOL) {
		return FuncExecContext_deref(CURRENT_EXEC_CTX, value);
	} else if (value->type == TYPE_BOOL) {
		return Value_copy(value);
	} else if (value->type == TYPE_NUM) {
		return Value_copy(value);
	}

	assert(false && "Unknown type.");
	return NIL;
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
		SymbolTable_insert(ctx->symTable,
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
	} ctx = POP_EXEC_CTX();

	return retv;
}
