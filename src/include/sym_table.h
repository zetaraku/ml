#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include "value.h"

struct SymTable;
typedef struct SymTable SymTable;

SymTable *new_SymTable();
void delete_SymTable(SymTable *thiz);
void SymTable_printout(SymTable *thiz);
void SymTable_insert(SymTable *thiz, Value *key, Value *val);
Value *SymTable_lookup(SymTable *thiz, Value *key);
bool SymTable_delete(SymTable *thiz, Value *key);

#endif
