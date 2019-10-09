#ifndef SymbolTable_H
#define SymbolTable_H

#include "Value.h"

struct KVPair;
typedef struct KVPair KVPair;
struct KVPair {
	void *key;
	void *value;
};

struct Node;
typedef struct Node Node;
struct Node {
	void *datap;
	Node *next;
};

struct SymbolTable;
typedef struct SymbolTable SymbolTable;
struct SymbolTable {
	Node *dummy_head;
};

SymbolTable *new_SymbolTable();
void delete_SymbolTable(SymbolTable *thiz);
void SymbolTable_printout(SymbolTable *thiz);
void SymbolTable_insert(SymbolTable *thiz, Value *key, Value *val);
Value *SymbolTable_lookup(SymbolTable *thiz, Value *key);
bool SymbolTable_delete(SymbolTable *thiz, Value *key);

#endif
