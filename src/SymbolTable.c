#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "include/Value.h"
#include "include/SymbolTable.h"

Node *new_Node(void *datap);
void delete_Node(Node *thiz);
KVPair *new_KVPair(void *key, void *value);
void delete_KVPair(KVPair *thiz);

Node *new_Node(void *datap) {
	Node *thiz = (Node *)malloc(sizeof(Node));
	thiz->datap = datap;
	thiz->next = NULL;
	return thiz;
}
void delete_Node(Node *thiz) {
	free(thiz);
}

KVPair *new_KVPair(void *key, void *value) {
	KVPair *thiz = (KVPair *)malloc(sizeof(KVPair));
	thiz->key = key;
	thiz->value = value;
	return thiz;
}
void delete_KVPair(KVPair *thiz) {
	free(thiz);
}

SymbolTable *new_SymbolTable() {
	SymbolTable *thiz = (SymbolTable *)malloc(sizeof(SymbolTable));
	thiz->dummy_head = new_Node(NULL);
	return thiz;
}
void delete_SymbolTable(SymbolTable *thiz) {
	for (
		Node *node = thiz->dummy_head->next, *next;
		node != NULL;
		node = next
	) {
		next = node->next;
		KVPair *kv = (KVPair *)node->datap;
		Value_unlink((Value *)kv->key);
		Value_unlink((Value *)kv->value);
		delete_KVPair(kv);
		delete_Node(node);
	}
	free(thiz->dummy_head);
	free(thiz);
}
void SymbolTable_printout(SymbolTable *thiz) {
	printf("[%p]", thiz->dummy_head);
	for (
		Node *curr = thiz->dummy_head->next;
		true;
		curr = curr->next
	) {
		printf("->%p", curr);
		if (curr == NULL)
			break;
	}
	putchar('\n');
}
void SymbolTable_insert(SymbolTable *thiz, Value *key, Value *val) {
	assert(key->type == TYPE_SYMBOL);
	#ifdef DEBUG
		printf("SymbolTable_insert @%p:\n", thiz);
		printf("key: ");
		Value_print(key);
		putchar('\n');
		printf("val: ");
		Value_print(val);
		putchar('\n');
	#endif

	Node *node_to_be_insert = new_Node(
		new_KVPair(Value_copy(key), Value_copy(val))
	);
	// printf("insert node: %p\n", node_to_be_insert);

	Node *prev = thiz->dummy_head;
	for (
		Node *curr = thiz->dummy_head->next;
		true;
		prev = curr, curr = curr->next
	) {
		bool ok = false;
		if (curr == NULL) {
			ok = true;
		} else {
			KVPair *kv = (KVPair *)curr->datap;
			Value *k = (Value *)kv->key;
			if (strcmp(
					Value_getSymbol(key),
					Value_getSymbol(k)
				) <= 0
			)
				ok = true;
		}


		if (ok) {
			prev->next = node_to_be_insert;
			node_to_be_insert->next = curr;
			return;
		}
	}
}
Value *SymbolTable_lookup(SymbolTable *thiz, Value *key) {
	assert(key->type == TYPE_SYMBOL);
	#ifdef DEBUG
		printf("SymbolTable_lookup @%p: %s\n", thiz, Value_getSymbol(key));
	#endif

	for (
		Node *curr = thiz->dummy_head->next;
		curr != NULL;
		curr = curr->next
	) {
		KVPair *kv = (KVPair *)curr->datap;
		Value *k = (Value *)kv->key;
		Value *v = (Value *)kv->value;
		if (strcmp(Value_getSymbol(k), Value_getSymbol(key)) == 0) {
			#ifdef DEBUG
				printf("SymbolTable_lookup found: ");
					Value_print(v);
				putchar('\n');
			#endif
			return Value_copy(v);
		}
	}

	return NULL;
}
bool SymbolTable_delete(SymbolTable *thiz, Value *key) {
	assert(key->type == TYPE_SYMBOL);
	#ifdef DEBUG
		printf("SymbolTable_delete @%p: %s\n", thiz, Value_getSymbol(key));
	#endif

	Node *prev = thiz->dummy_head;
	for (
		Node *curr = thiz->dummy_head->next;
		curr != NULL;
		prev = curr, curr = curr->next
	) {
		KVPair *kv = (KVPair *)curr->datap;
		Value *k = (Value *)kv->key;
		Value *v = (Value *)kv->value;
		if (
			strcmp(
				Value_getSymbol(k),
				Value_getSymbol(key)
			) == 0
		) {
			prev->next = curr->next;
			Value_unlink(k);
			Value_unlink(v);
			delete_KVPair(kv);
			delete_Node(curr);
			return true;
		}
	}

	return false;
}
