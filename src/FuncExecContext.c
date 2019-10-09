#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "include/Value.h"
#include "include/FuncExecContext.h"

FuncExecContext *new_FuncExecContext(FuncExecContext *upper) {
	FuncExecContext *thiz = (FuncExecContext *) malloc(sizeof(FuncExecContext));
	thiz->upper = upper;
	thiz->symTable = new_SymbolTable();
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
		Value *value = SymbolTable_lookup(ctx->symTable, symbol);

		if (value != NULL)
			return value;
	}

	fprintf(stderr, "TypeError: The symbol '%s' is undefined.", Value_getSymbol(symbol));

	exit(-1);
}

FuncExecContext *FuncExecContext__stack[1024]; size_t FuncExecContext__stack_size = 0;
