#ifndef FuncExecContext_H
#define FuncExecContext_H

#include "Value.h"
#include "SymbolTable.h"

#define CURRENT_EXEC_CTX (FuncExecContext__stack[FuncExecContext__stack_size-1])
#define PUSH_EXEC_CTX(ctx) FuncExecContext__stack[FuncExecContext__stack_size++] = ctx
#define POP_EXEC_CTX() FuncExecContext__stack[--FuncExecContext__stack_size]

// class
struct FuncExecContext;
typedef struct FuncExecContext FuncExecContext;
struct FuncExecContext {
	FuncExecContext *upper;
	SymbolTable *symTable;
};

// methods
FuncExecContext *new_FuncExecContext(FuncExecContext *upper);
void delete_FuncExecContext(FuncExecContext *thiz);
Value *FuncExecContext_deref(FuncExecContext *thiz, Value *symbol);

// static members
extern FuncExecContext *FuncExecContext__stack[1024]; extern size_t FuncExecContext__stack_size;

#endif
