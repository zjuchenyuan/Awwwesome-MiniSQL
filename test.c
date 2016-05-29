#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MiniSQL.h"
#include "BPlusTree.h"
int main()
{
    FILE *fp;
    meta_t *meta;
    leaf_t *leaf;
    internal_t *root;
    my_key_t newKey;
    struct tree_t tree;
    char fileName[1024];
    int i, j;
    off_t offset;
    scanf("%s", fileName);
    fp = fopen(fileName, "w");
    fclose(fp);
    // test InitTree
    InitTree(&tree, fileName, intType);
    meta = (meta_t *)ReadBlock(fileName, META_OFFSET, sizeof(meta_t));
    printf("meta data:\n");
    printf("meta.order: %ld\nmeta.valueSize: %ld\nmeta.keySize: %ld\nmeta.internalNum: %ld\nmeta.leafNum: %ld\nmeta.height: %ld\nmeta.slot: %ld\nmeta.rootOffset: %ld\nmeta.leafOffset: %ld\n", meta->order, meta->valueSize, meta->keySize, meta->internalNum, meta->leafNum, meta->height, meta->slot, meta->rootOffset, meta->leafOffset);
    // test insert
    for (i = 9; i >= 0; i--)
    {
    #ifndef DEBUG
        printf("i: %d\n", i);
    #endif
        newKey.key = i;
        Insert(&tree, newKey, i);
    }

    //newKey.key = 6666;
    //printf("Insert faile? %d\n", Insert(&tree, newKey, 0x12345678));
    //newKey.key = 4112;
    //printf("Insert faile? %d\n", Insert(&tree, newKey, 0x4112));
    meta = (meta_t *)ReadBlock(fileName, META_OFFSET, sizeof(meta_t));
    leaf = (leaf_t *)ReadBlock(fileName, meta->leafOffset, sizeof(leaf_t));
    printf("leaf data:\n");
    printf("leaf->parent: %ld\nleaf->next: %ld\nleaf->prev: %ld\nleaf->n: %ld\nleaf->children[0].value: %ld\nleaf->children[0].key.key: %d\n", leaf->parent, leaf->next, leaf->prev, leaf->n, leaf->children[0].value, leaf->children[0].key.key);
    for (j = 0; j < (int)meta->leafNum; j++)
    {
        for (i = 0; i < (int)leaf->n; i++)
        {
            printf("%ld ", leaf->children[i].value);
        }
        printf("\n\n");
        offset = leaf->next;
        free(leaf);
        leaf = (leaf_t *)ReadBlock(fileName, offset, sizeof(leaf_t));
    }
    printf("\n\n");
    // test ReadBlock
    printf("meta data:\n");
    printf("meta.order: %ld\nmeta.valueSize: %ld\nmeta.keySize: %ld\nmeta.internalNum: %ld\nmeta.leafNum: %ld\nmeta.height: %ld\nmeta.slot: %ld\nmeta.rootOffset: %ld\nmeta.leafOffset: %ld\n", meta->order, meta->valueSize, meta->keySize, meta->internalNum, meta->leafNum, meta->height, meta->slot, meta->rootOffset, meta->leafOffset);
    // test root
    root = (internal_t *)ReadBlock(fileName, meta->rootOffset, sizeof(internal_t));
    printf("root data: \n");
    printf("root->n: %ld\n", root->n);
    free(leaf);
    free(root);
    return 0;
}
