#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "allocator.h"
#include "context_stack.h"
#include "value.h"
#include "utils.h"
#define loop while (true)
#define once for (bool b = false; !b; b = true)

extern int yyparse();

Value *eval_prog(Value *prog) {
	Value *retv = NIL;

	FuncExecContext *ctx = new_FuncExecContext(NULL);
	PUSH_EXEC_CTX(ctx); {
		Value *li = ListIterator_from(prog), *v;
		while ((v = ListIterator_next(&li)) != NULL) {
			Value_unlink(retv);
			retv = Value__eval(v);
		}
	} POP_EXEC_CTX();
	return retv;
}

int main(int argc, const char **argv) {
	Value *prog = NULL;

	setbuf(stdout, NULL);

	_Value__initializeNIL(); {
		FuncExecContext *ctx = new_FuncExecContext(NULL);
		PUSH_EXEC_CTX(ctx); {
			yyparse(NULL);
		} POP_EXEC_CTX();

		// reg_show_usage();
		// reg_view_alloc();
	} _Value__releaseNIL();

	return 0;
}

int main2(int argc, const char **argv) {
	Value *prog = NULL;

	_Value__initializeNIL(); {
		FuncExecContext *ctx = new_FuncExecContext(NULL);
		PUSH_EXEC_CTX(ctx); {
			once {
				int parse_status = yyparse(&prog);

				if (parse_status != 0)
					break;

				puts("=======v     prog     v=======");
				Value_print(prog); putchar('\n');
				puts("=======^     prog     ^=======");

				reg_show_usage();

				puts("=======v    output    v=======");
				Value *retv = eval_prog(prog);
				puts("=======^    output    ^=======");

				reg_show_usage();

				puts("=======v    retval    v=======");
				Value_print(retv); putchar('\n');
				puts("=======^    retval    ^=======");

				reg_show_usage();

				// cleanup
				Value_unlink(retv);
				Value_unlink(prog);
			}
		} POP_EXEC_CTX();

		// reg_show_usage();
		// reg_view_alloc();
	} _Value__releaseNIL();

	return 0;
}
