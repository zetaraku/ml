%{
	#include <stdio.h>
	#include "include/FuncExecContext.h"
	#include "include/Value.h"
	#include "include/utils.h"

	extern int yylex();
	void yyerror(Value **prog, const char *s) {
		fprintf(stderr, "yyerror: %s\n", s);
	}

	void repl(Value *val);
%}

%error-verbose
%parse-param { Value **prog_ptr }

%union {
	char strval[256];
	bool boolval;
	int numval;
	struct Value *value;
}

%start Program
%token UNKNOWN
%token '(' ')'
%token <strval> ID
%token <boolval> BOOLLIT
%token <numval> NUMLIT
%type <value> Program Outer_Exp_list Exp_list Exp

%%

Program:
	Outer_Exp_list		{*prog_ptr = $1; YYACCEPT;}
;

Outer_Exp_list:
	Exp					{repl($1);}
	Outer_Exp_list		{$$ = Value__fromCons(Cons__of($1, $3));}
	| /* lambda */		{$$ = NIL;}
;

Exp_list:
	Exp
	Exp_list			{$$ = Value__fromCons(Cons__of($1, $2));}
	| /* lambda */		{$$ = NIL;}
;

Exp:
	'(' Exp_list ')'	{$$ = $2;}
	| ID				{$$ = Value__fromSymbol($1);}
	| BOOLLIT			{$$ = Value__fromBool($1);}
	| NUMLIT			{$$ = Value__fromNum($1);}
;

%%
