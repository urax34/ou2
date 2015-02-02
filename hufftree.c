#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "hufftree.h"


/*Creates empty tree*/
huff_tree *huffTree_create() {
    huff_tree *tree=calloc(1,sizeof(huff_tree));
    tree->root=calloc(1,sizeof(node));
    return tree;
}

void huffTree_setMemHandler(huff_tree *t, memFreeFunc *f){
    t->freeFunc=f;
}

/*Returns position of root node*/

huffTree_pos huffTree_root(huff_tree *tree) {
    return tree->root;
}

bool huffTree_hasLeftChild(huff_tree *tree,huffTree_pos n) {
    return n->leftChild!=NULL;
}

bool huffTree_hasRightChild(huff_tree *tree,huffTree_pos n) {
    return n->rightChild!=NULL;
}

huffTree_pos huffTree_rightChild(huff_tree *tree,huffTree_pos n) {
    return n->rightChild;
}

huffTree_pos huffTree_leftChild(huff_tree *tree,huffTree_pos n) {
    return n->leftChild;
}

huffTree_pos huffTree_parent(huff_tree *tree,huffTree_pos n) {
    return n->parent;
}

data huffTree_inspectCharacter(huff_tree *tree,huffTree_pos n) {
    return n->character;
}
data huffTree_inspectWeight(huff_tree *tree,huffTree_pos n) {
    return n->weight;
}


bool huffTree_hasCharacter(huff_tree *tree,huffTree_pos n) {
    return n->character!=NULL;
    //return n->character!=0;
}
bool huffTree_hasWeight(huff_tree *tree,huffTree_pos n) {
    return n->weight!=NULL;
    //return n->character!=0;
}

void huffTree_setValues(huff_tree *tree,data character, data weight,huffTree_pos n) {
    n->character=character;
    n->weight=weight;
}

huffTree_pos huffTree_insertRight(huff_tree *tree,huffTree_pos n) {
    huffTree_pos p=calloc(1,sizeof(node));
    p->parent =n;
    if (n->rightChild!=NULL)

        huffTree_deleteNode(tree,n->rightChild);
    n->rightChild=p;
    return p;
}

huffTree_pos huffTree_insertLeft(huff_tree *tree,huffTree_pos n) {
    huffTree_pos p=calloc(1,sizeof(node));
    p->parent=n;
    if (n->leftChild!=NULL)
        huffTree_deleteNode(tree,n->leftChild);
    n->leftChild=p;
    return p;
}

huff_tree *huffTree_merge (huff_tree *treeLeft, huff_tree *treeRight){
    huff_tree *newTree=huffTree_create();

    data CharLeftData, CharRightData, WeightLeftData, WeightRightData;
//    int tempCharLeft, tempCharRight, tempWeightLeft, tempWeightRight;
    huffTree_pos r = huffTree_root(newTree);

    huffTree_pos left = huffTree_root(treeLeft);
    r = huffTree_insertLeft(newTree,r);
    //tempCharLeft = huffTree_inspectCharacter(treeLeft, left);
    CharLeftData = huffTree_inspectCharacter(treeLeft, left);
    //tempWeightLeft = huffTree_inspectWeight(treeLeft, left);
    WeightLeftData = huffTree_inspectWeight(treeLeft, left);
    //huffTree_setValues(newTree,tempCharLeft,tempWeightLeft,r);
    huffTree_setValues(newTree,CharLeftData,WeightLeftData,r);

    if (huffTree_hasLeftChild(treeLeft,left)){
        r->leftChild=left->leftChild;
    }
    if (huffTree_hasRightChild(treeLeft,left)){
        r->rightChild=left->rightChild;
    }

    r=huffTree_root(newTree);
    huffTree_pos right = huffTree_root(treeRight);
    r = huffTree_insertRight(newTree,r);
    //tempCharRight = huffTree_inspectCharacter(treeRight, right);
    CharRightData = huffTree_inspectCharacter(treeRight, right);
    //tempWeightRight = huffTree_inspectWeight(treeRight, right);
    WeightRightData = huffTree_inspectWeight(treeRight, right);
    //huffTree_setValues(newTree,tempCharRight,tempWeightRight,r);
    huffTree_setValues(newTree,CharRightData,WeightRightData,r);

    if (huffTree_hasLeftChild(treeRight,right)){
        r->leftChild=right->leftChild;
    }
    if (huffTree_hasRightChild(treeRight,right)){
        r->rightChild=right->rightChild;
    }
    r=huffTree_root(newTree);
    int sum = (int)(intptr_t)WeightLeftData+(int)(intptr_t)WeightRightData;
    data sumData = (data)(intptr_t)sum;
    printf("\n Data  = %d\n", (int)sum);
    //huffTree_setValues(newTree,0,(int)tempWeightLeft + (int)tempWeightRight,r);
    huffTree_setValues(newTree,NULL,sumData,r);
    //huffTree_free(treeLeft);
    //huffTree_free(treeRight);
    //huffTree_deleteNode(treeLeft,huffTree_root(treeLeft));
    //free(treeLeft->root->character);
    //free(treeLeft->root->weight);

    //treeLeft->root->leftChild=NULL;
    //treeRight->root->leftChild=NULL;
//    if (treeLeft->root->character=!NULL)
//        free(treeLeft->root->character);

//    if (treeLeft->root->weight=!NULL)
//        free(treeLeft->root->weight);
   // huffTree_free(treeLeft);


    if (treeLeft->root!=NULL)
        free(treeLeft->root);
    if (treeLeft->root!=NULL)
        free(treeRight->root);
    //free(treeRight);
    free(treeLeft);
    free(treeRight);
    return newTree;
   /* data tempChar, tempWeightLeft, tempWeightRight;
    huffTree_pos p = huffTree_root(treeLeft);
    tempChar = huffTree_inspectCharacter(treeLeft, p);
    tempWeightLeft = huffTree_inspectWeight(treeLeft, p);
    p = huffTree_insertLeft(treeLeft,p);
    huffTree_setValues(treeLeft,tempChar,tempWeightLeft,p);

    p = huffTree_root(treeLeft);

    p = huffTree_insertRight(treeLeft,p);

    huffTree_pos n=huffTree_root(treeRight);
    tempChar = huffTree_inspectCharacter(treeRight, n);
    tempWeightRight = huffTree_inspectWeight(treeRight, n);

    huffTree_setValues(treeLeft, tempChar, tempWeightRight,p);

    p = huffTree_root(treeLeft);
    huffTree_setValues(treeLeft,0,(int)tempWeightLeft+(int)tempWeightRight,p);*/

//    return newTree;
}

huffTree_pos huffTree_deleteNode(huff_tree *tree,huffTree_pos n) {
    huffTree_pos parent=n->parent;
    if(n->leftChild!=NULL) {
        huffTree_deleteNode(tree,n->leftChild);
    }
    if(n->rightChild!=NULL) {
        huffTree_deleteNode(tree,n->rightChild);
    }
    if(parent->rightChild==n) {
        parent->rightChild=NULL;
    }
    else {
        parent->leftChild=NULL;
    }
    if(tree->freeFunc && huffTree_hasCharacter(tree,n))
        tree->freeFunc(n->character);
    if(tree->freeFunc && huffTree_hasWeight(tree,n))
        tree->freeFunc(n->weight);
    free(n);
    return parent;
}






void huffTree_free(huff_tree *tree) {
    if( huffTree_hasLeftChild(tree,tree->root))
        huffTree_deleteNode(tree,tree->root->leftChild);
    if( huffTree_hasRightChild(tree,tree->root))
        huffTree_deleteNode(tree,tree->root->rightChild);
    if(tree->freeFunc && huffTree_hasCharacter(tree,tree->root))
        tree->freeFunc(tree->root->character);
    if(tree->freeFunc && huffTree_hasWeight(tree,tree->root))
        tree->freeFunc(tree->root->weight);
    free(tree->root);
    free(tree);
}
