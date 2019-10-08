#ifndef CONTEXT_STACK_H
#define CONTEXT_STACK_H

#include "value.h"
#include "sym_table.h"

#define CURRENT_EXEC (exec_context_stack[exec_context_stack_size-1])
#define PUSH_EXEC_CTX(ctx) exec_context_stack[exec_context_stack_size++] = ctx
#define POP_EXEC_CTX() exec_context_stack[--exec_context_stack_size]
struct FuncExecContext;
typedef struct FuncExecContext FuncExecContext;
struct FuncExecContext {
	FuncExecContext *upper;
	SymTable *symTable;
};
extern FuncExecContext *exec_context_stack[1024]; extern size_t exec_context_stack_size;

FuncExecContext *new_FuncExecContext(FuncExecContext *upper);
void delete_FuncExecContext(FuncExecContext *thiz);
Value *FuncExecContext_deref(FuncExecContext *thiz, Value *symbol);

#endif
