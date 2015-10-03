#include "strope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define malloc_sizeof(type) (malloc(sizeof(type)))

#define StropeTree_LEAF (0)
#define StropeTree_NODE (1)

#define StropeTree_as_leaf(tree) (&(tree)->as.leaf)
#define StropeTree_as_node(tree) (&(tree)->as.node)

static void StropeTree_invalid_type_error(StropeTree *tree) __attribute__((__noreturn__));

static void StropeTree_invalid_type_error(StropeTree *tree) {
  fprintf(stderr, "invalid tree type: %d\n", tree->as.any.header.type);
  exit(1);
}

static StropeChunk *StropeChunk_new(const char *body, size_t size) {
  StropeChunk *self = malloc_sizeof(StropeChunk);

  if (self == NULL) {
    return NULL;
  }

  self->ref_count = 0;
  self->size = size;
  self->body = malloc(sizeof(char) * size);

  if (self->body == NULL) {
    free(self);
    return NULL;
  }

  memcpy((void *)(self->body), body, size);

  return self;
}

static void StropeChunk_free(StropeChunk *self) {
  if (self->ref_count != 0) {
    fprintf(stderr, "free chunk referenced other\n");
    exit(1);
  }

  free((char *)self->body);
  free(self);
}

static StropeTree *StropeTree_new(int type, int weight) {
  StropeTree *self = malloc_sizeof(StropeTree);

  if (self == NULL) {
    return NULL;
  }

  self->as.any.header.type = type;
  self->as.any.header.ref_count = 0;
  self->as.any.header.weight = weight;

  return self;
}

static StropeTree *StropeLeaf_new(StropeChunk *chunk, size_t offset, size_t length) {
  StropeTree *self = StropeTree_new(StropeTree_LEAF, length);

  self->as.leaf.chunk = chunk;
  self->as.leaf.offset = offset;
  self->as.leaf.length = length;

  return self;
}

static int StropeTree_total_weight(StropeTree *tree) {
  switch (tree->as.any.header.type) {
  case StropeTree_LEAF:
    return tree->as.leaf.header.weight;
    break;

  case StropeTree_NODE: {
    StropeNode *node = StropeTree_as_node(tree);
    return node->left->as.any.header.weight + StropeTree_total_weight(node->right);
    break;
  }

  default:
    StropeTree_invalid_type_error(tree);
    break;
  }
}

static StropeTree *StropeNode_new(StropeTree *left, StropeTree *right) {
  int weight = StropeTree_total_weight(left);
  StropeTree *self = StropeTree_new(StropeTree_NODE, weight);

  StropeNode *node = StropeTree_as_node(self);

  node->left = left;
  node->right = right;

  return self;
}

static Strope *Strope_new_with(StropeTree *tree) {
  Strope *self = malloc_sizeof(Strope);

  if (self == NULL) {
    return NULL;
  }

  self->tree = tree;

  return self;
}

Strope *Strope_new(const char *str) {
  StropeChunk *chunk = StropeChunk_new(str, strlen(str));

  if (chunk == NULL) {
    return NULL;
  }

  StropeTree *tree = StropeLeaf_new(chunk, 0, chunk->size);

  if (tree == NULL) {
    StropeChunk_free(chunk);
    return NULL;
  }

  return Strope_new_with(tree);
}


void Strope_free(Strope *self) {
  // Not implemented
}

Strope *Strope_concat(Strope *self, Strope *other) {
  StropeTree *tree = StropeNode_new(self->tree, other->tree);

  if (tree == NULL) {
    return NULL;
  }

  return Strope_new_with(tree);
}

size_t Strope_length(Strope *self) {
  // Not implemented
  return -1;
}

char Strope_at(Strope *self, size_t index) {
  return '\0';
}


Strope *Strope_substring(Strope *self, size_t i, size_t j) {
  // Not implemented
  return NULL;
}

const char *Strope_cstring(Strope *self) {
  // Not implemented
  return NULL;
}
