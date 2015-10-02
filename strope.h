#ifndef STROPE_H_
#define STROPE_H_

#include <stddef.h>

typedef struct StropeHeader {
  int ref_count;
} StropeHeader;

struct StropeTree;
typedef struct StropeTree StropeTree;

typedef struct StropeLeaf {
  struct StropeHeader header;
  int length;
  char *body;
} StropeLeaf;

typedef struct StropeNode {
  struct StropeHeader header;
  StropeTree *left;
  StropeTree *right;
} StropeNode;

struct StropeTree {
  union {
    struct StropeHeader any;
    StropeLeaf leaf;
    StropeNode node;
  } as;
};

typedef struct Strope {
  StropeTree *tree;
} Strope;

Strope *Strope_new(const char *str);
void Strope_free(Strope *self);

size_t Strope_length(Strope *self);
char Strope_at(Strope *self, size_t index);
Strope *Strope_concat(Strope *self, Strope *other);
Strope *Strope_substring(Strope *self, size_t i, size_t j);
const char *Strope_cstring(Strope *self);

#endif
