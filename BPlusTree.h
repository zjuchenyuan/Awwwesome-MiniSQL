#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#define NOBUFFER
#include "MiniSQL.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//typedef _off_t off_t
// the following definition of offsets might be replaced in the real work
#define TREE_ORDER  (BLOCK_SIZE - 3 * sizeof(off_t) - sizeof(size_t)) / sizeof(record_t)
#define META_OFFSET 0  // this means one file contains exactly one tree, and the beginning of a file is the meta data
#define BLOCK_OFFSET META_OFFSET + BLOCK_SIZE
#define SIZE_NO_CHILDREN sizeof(leaf_t) + TREE_ORDER * sizeof(record_t)
#define KeyValueCmp(a, b) _Generic(a, int: IntKeyCmp, float: FloatKeyCmp, char *: StringKeyCmp)(a, b)
// ===================================================================================
// @NOTE here we need to invoke Buffer module to read / write blocks
void *ReadBlock(char *fileName, off_t offset, size_t size);  // return a pointer which points to a block in memory
int WriteBlock(char *fileName, void *block, off_t offset, size_t size);  // return 1 if succeeded or 0 if not
// ===================================================================================
// key and value definition
typedef off_t value_t;  // value type, default int
typedef struct my_key_t my_key_t;  // key type (int, float, varchar)
struct my_key_t
{
    int key;
    //float key;
    //char key[256];
    //size_t size;
};

// tree structure
typedef struct index_t index_t;
struct index_t
{
    my_key_t key;
    off_t child;
};

typedef struct record_t record_t;
struct record_t
{
    my_key_t key;
    value_t value;
};

typedef struct leaf_t leaf_t;
struct leaf_t
{
    off_t parent;
    off_t next;
    off_t prev;
    size_t n;
    record_t children[TREE_ORDER];
};

typedef struct internal_t internal_t;
struct internal_t
{
    off_t parent;
    off_t next;
    off_t prev;
    size_t n;
    index_t children[TREE_ORDER];
};

typedef struct meta_t meta_t;
struct meta_t
{
    size_t order;
    size_t valueSize;
    size_t keySize;
    size_t internalNum;
    size_t leafNum;
    size_t height;
    off_t slot;
    off_t rootOffset;
    off_t leafOffset;
    enum DataType type;
};

typedef struct tree_t *BPlusTree;
struct tree_t
{
    //FILE *fp;  // multi-level file handling
    //int fpLevel; // the level of current file, to avoid open for many times
    char path[1024];  // path to the index file
    meta_t meta;  // meta data
};

// block read / write
/* not used any more
void OpenFile(BPlusTree tree);
void CloseFile(BPlusTree tree);
int ReadIndexBlock(BPlusTree tree, void *block, off_t offset, size_t size);
int WriteIndexBlock(BPlusTree tree, void *block, off_t offset, size_t size);
*/
// initialize tree
void InitTree(BPlusTree tree, char *path, enum DataType type);
off_t AllocLeaf(BPlusTree tree, leaf_t *node);
off_t AllocInternal(BPlusTree tree, internal_t *node);
off_t AllocSize(BPlusTree tree, size_t size);
// Insert
int Insert(BPlusTree tree, my_key_t key, value_t value);
int Search(BPlusTree tree, my_key_t key);
off_t SearchIndex(BPlusTree tree, my_key_t key);
off_t SearchLeaf(BPlusTree tree, off_t parent, my_key_t key);
int KeyCmp(my_key_t A, my_key_t B);
int IntKeyCmp(int A, int B);
int FloatKeyCmp(float A, float B);
int StringKeyCmp(char *A, char *B);
void InsertIntoLeaf(leaf_t *leaf, record_t *newRecord);
void InsertIntoInternal(internal_t *internal, index_t index);
// Remove
int Remove(BPlusTree tree, my_key_t key);

#endif
