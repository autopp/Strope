#ifndef STROPE_H_
#define STROPE_H_

#include <stddef.h>

typedef struct StropeHeader {
  int ref_count;
} Strope;

typedef struct StropeLeaf {
  StropeHeader header;
  int length;
  char *body;
} StropeLeaf;

typedef struct StropeNode {
  StropeHeader header;
  StropeTree *left;
  StropeTree *right;
} StropeNode;

typedef struct StropeTree {
  union {
    StropeHeader any;
    StropeLeaf leaf;
    StropeNode node;
  } as;
} StropeTree;

typedef struct Strope {
  StropeTree *tree;
} Strope;

Strope *Strope_new(void);
Strope *Strope_new_with(const char *, size_t size);
void Strope_free(Strope *self);

size_t Strope_length(Strope *self);
char Strope_at(Strope *self, size_t index);
Strope *Strope_concat(Strope *self, Strope *other);
Strope *Strope_substring(Strope *self, size_t i, size_t j);
const char *Strope_cstring(Strope *self);

#endif
