#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "include/FuncExecContext.h"
#include "include/Value.h"
#include "include/utils.h"
#define loop while (true)
#define once for (bool b = false; !b; b = true)

extern int yyparse();

bool interactive = false;

Value *eval_prog(Value *prog) {
	Value *retv = NIL;

	FuncExecContext *ctx = new_FuncExecContext(NULL);
	PUSH_EXEC_CTX(ctx); {
		Value *li = ListIterator_from(prog), *v;
		while ((v = ListIterator_next(&li)) != NULL) {
			Value_unlink(retv);
			retv = Value__eval(v);
		}
	} ctx = POP_EXEC_CTX();
	return retv;
}

void repl(Value *val) {
	if (!interactive)
		return;
	Value *retv = Value__eval(val);
	Value_print(retv); putchar('\n');
}

int interpret() {
	_Value__initializeNIL(); {
		FuncExecContext *ctx = new_FuncExecContext(NULL);
		PUSH_EXEC_CTX(ctx); once {
			Value *prog = NULL;
			int parse_status = yyparse(&prog);

			if (parse_status != 0)
				return parse_status;

			#ifdef DEBUG
				puts("=======v     prog     v=======");
				Value_print(prog); putchar('\n');
				puts("=======^     prog     ^=======");
				reg_show_usage();

				puts("=======v    output    v=======");
			#endif

			Value *retv = eval_prog(prog);

			#ifdef DEBUG
				puts("=======^    output    ^=======");
				reg_show_usage();
			#endif

			// cleanup
			Value_unlink(retv);
			Value_unlink(prog);
		} ctx = POP_EXEC_CTX();

		#ifdef DEBUG
			reg_show_usage();
			reg_view_alloc();
		#endif
	} _Value__releaseNIL();

	return 0;
}

int main(int argc, const char **argv) {
	setbuf(stdout, NULL);

	for (size_t i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			interactive = true;
		} else {
			freopen(argv[i], "r", stdin);
		}
	}

	return interpret();
}
