#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "value.h"
#include "context_stack.h"

FuncExecContext *exec_context_stack[1024]; size_t exec_context_stack_size = 0;

FuncExecContext *new_FuncExecContext(FuncExecContext *upper) {
	FuncExecContext *thiz = (FuncExecContext *)malloc(sizeof(FuncExecContext));
	thiz->upper = upper;
	thiz->symTable = new_SymTable();
	return thiz;
}
void delete_FuncExecContext(FuncExecContext *thiz) {
	free(thiz);
}
Value *FuncExecContext_deref(FuncExecContext *thiz, Value *symbol) {
	assert(symbol->type == TYPE_SYMBOL);
	#ifdef DEBUG
		printf("FuncExecContext_deref: %s\n", Value_getSymbol(symbol));
	#endif
	for (FuncExecContext *ctx = thiz; ctx != NULL; ctx = ctx->upper) {
		Value *value = SymTable_lookup(ctx->symTable, symbol);
		if (value != NULL)
			return value;
	}
	assert(false && "TypeError: the value of symbol is undefined.");
	// return NIL;
}
