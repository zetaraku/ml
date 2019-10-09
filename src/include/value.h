#ifndef Value_H
#define Value_H

#include <stdlib.h>
#include <stdbool.h>

enum TYPE {
	TYPE_UNDEFINED = 0,
	TYPE_BOOL, TYPE_NUM, TYPE_SYMBOL, TYPE_CONS,
	TYPE_OTHER
};

struct Value;
typedef struct Value Value;
	typedef const char *Symbol;
	typedef bool Bool;
	typedef int Num;
	struct Cons;
	typedef struct Cons Cons;

struct Cons {
	struct FuncExecContext *_closure;
	Value *left;
	Value *right;
};

struct Value {
	enum TYPE type;
	union {
		Symbol symval;
		Bool boolval;
		Num numval;
		Cons _;
	};
	size_t link_count;
};

extern Value *NIL;

Value *reg_alloc();
void reg_unalloc(Value *value);

void reg_view_alloc();
void reg_show_usage();

Value *_new_Value(int type);
void Value_unlink(Value *thiz);
void _delete_Value(Value *thiz);
void Value_print(Value *thiz);
void _Value_print_raw(Value *thiz);
void _Value__initializeNIL();
void _Value__releaseNIL();
Value *Value__eval(Value *value);

// Value__fromXXX
Value *Value__fromBool(Bool boolval);
Value *Value__fromNum(Num numval);
Value *Value__fromSymbol(const char *symval);
Value *Value__fromCons(Cons consval);

// Value__getXXX
Bool Value_getBool(Value *thiz);
Num Value_getNum(Value *thiz);
Symbol Value_getSymbol(Value *thiz);
Cons Value_getCons(Value *thiz);

// Value__extractXXX
Bool Value__extractBool(Value *value);
Num Value__extractNum(Value *value);
Symbol Value__extractSymbol(Value *value);
Cons Value__extractCons(Value *value);

Value *Value_link(Value *thiz);
Value *Value_copy(Value *thiz);

#endif
