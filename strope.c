#include "strope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define malloc_sizeof(type) (malloc(sizeof(type)))

#define StropeTree_LEAF (0)
#define StropeTree_NODE (1)

#define StropeTree_type(tree) ((tree)->as.any.header.type)

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

static int StropeTree_length(StropeTree *tree) {
  switch (tree->as.any.header.type) {
  case StropeTree_LEAF:
    return tree->as.any.header.weight;
    break;

  case StropeTree_NODE: {
    StropeNode *node = StropeTree_as_node(tree);
    return node->header.weight + StropeTree_length(node->right);
    break;
  }

  default:
    StropeTree_invalid_type_error(tree);
    break;
  }
}

static StropeTree *StropeNode_new(StropeTree *left, StropeTree *right) {
  int weight = StropeTree_length(left);
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
  return StropeTree_length(self->tree);
}

char StropeTree_at(StropeTree *self, size_t index) {
  if (StropeTree_type(self) == StropeTree_LEAF) {
    StropeLeaf *leaf = StropeTree_as_leaf(self);

    return leaf->chunk->body[leaf->offset + index];
  } else {
    StropeNode *node = StropeTree_as_node(self);

    if (index < node->header.weight) {
      return StropeTree_at(node->left, index);
    } else {
      return StropeTree_at(node->right, index - node->header.weight);
    }
  }
}

char Strope_at(Strope *self, size_t index) {
  return StropeTree_at(self->tree, index);
}


Strope *Strope_substring(Strope *self, size_t i, size_t j) {
  // Not implemented
  return NULL;
}

void StropeTree_dump_cstring(StropeTree *tree, char *buf) {
  switch (tree->as.any.header.type) {
  case StropeTree_LEAF: {
    StropeLeaf *leaf = StropeTree_as_leaf(tree);
    memcpy(buf, leaf->chunk->body + leaf->offset, leaf->length);
    break;
  }
  case StropeTree_NODE: {
    StropeNode *node = StropeTree_as_node(tree);
    StropeTree_dump_cstring(node->left, buf);
    StropeTree_dump_cstring(node->right, buf + node->header.weight);
    break;
  }
  default:
    StropeTree_invalid_type_error(tree);
    break;
  }
}

const char *Strope_cstring(Strope *self) {
  int length = Strope_length(self);
  char *buf = malloc(sizeof(char) * (length + 1));

  if (buf == NULL) {
    return NULL;
  }

  StropeTree_dump_cstring(self->tree, buf);
  buf[length] = '\0';

  return buf;
}
