bison --debug --verbose --token-table -d ml.y
flex ml.l
gcc -std=c99 -DNDEBUG ml.c lex.yy.c ml.tab.c utils.c value.c allocator.c context_stack.c sym_table.c
rm lex.yy.c ml.tab.c ml.tab.h ml.output
