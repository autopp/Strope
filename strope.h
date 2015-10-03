#ifndef STROPE_H_
#define STROPE_H_

#include <stddef.h>

typedef struct StropeHeader {
  int type;
  int ref_count;
  int weight;
} StropeHeader;

struct StropeTree;
typedef struct StropeTree StropeTree;

typedef struct StropeChunk {
  int ref_count;
  const char *body;
  size_t size;
} StropeChunk;

typedef struct StropeLeaf {
  struct StropeHeader header;
  StropeChunk *chunk;
  size_t offset;
  size_t length;
} StropeLeaf;

typedef struct StropeNode {
  struct StropeHeader header;
  StropeTree *left;
  StropeTree *right;
} StropeNode;

struct StropeTree {
  union {
    struct {
      struct StropeHeader header;
    } any;
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
Strope *Strope_substring(Strope *self, size_t i, size_t n);
const char *Strope_cstring(Strope *self);

#endif
