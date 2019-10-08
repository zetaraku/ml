%{
	#include <stdio.h>
	#include "context_stack.h"
	#include "value.h"
	#include "utils.h"

	extern int yylex();
	void yyerror(Value **prog, const char *s) {
		fprintf(stderr, "yyerror: %s\n", s);
	}
%}

%error-verbose
%parse-param { Value **prog_ptr }

%union {
	char strval[256];
	bool boolval;
	int numval;
	struct Value *value;
}

%start Program_CLI
%token UNKNOWN
%token '(' ')'
%token <strval> ID
%token <boolval> BOOLLIT
%token <numval> NUMLIT
%type <value> Program Exp_list Exp Program_CLI Outer_Exp_list

%%

Program:
	Exp_list			{*prog_ptr = $1; YYACCEPT;}
;

Program_CLI:
	Outer_Exp_list
;
Outer_Exp_list:
	Exp {
		Value *retv = Value__eval($1);
		Value_print(retv); putchar('\n');
	} Outer_Exp_list
	| /* lambda */		{$$ = NIL;}
;

Exp_list:
	Exp Exp_list		{$$ = Value__fromCons(Cons__of($1, $2));}
	| /* lambda */		{$$ = NIL;}
;
Exp:
	'(' Exp_list ')'	{$$ = $2;}
	| ID				{$$ = Value__fromSymbol($1);}
	| BOOLLIT			{$$ = Value__fromBool($1);}
	| NUMLIT			{$$ = Value__fromNum($1);}
;

%%
