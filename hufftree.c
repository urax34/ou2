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
newTree->root->leftChild=treeLeft->root;
treeLeft->root->parent=newTree->root;
newTree->root->rightChild=treeRight->root;
treeRight->root->parent=newTree->root;
data WeightLeftData, WeightRightData;
huffTree_pos left = huffTree_root(treeLeft);
huffTree_pos right = huffTree_root(treeRight);
WeightLeftData = huffTree_inspectWeight(treeLeft, left);
WeightRightData = huffTree_inspectWeight(treeRight, right);
huffTree_pos r=huffTree_root(newTree);
int sum = (int)(intptr_t)WeightLeftData+(int)(intptr_t)WeightRightData;
data sumData = (data)(intptr_t)sum;
huffTree_setValues(newTree,NULL,sumData,r);
treeLeft->root=NULL;
treeRight->root=NULL;
free(treeLeft);
free(treeRight);
return newTree;
}

huff_tree *buildHuffTree (int **harr, const int size){
huff_tree *huffArr[size];
for (int i = 0; i<size; i++){
huffArr[i] = huffTree_create();
huffArr[i]->root->character=(data)(intptr_t)harr[i][0];
huffArr[i]->root->weight=(data)(intptr_t)harr[i][1];
}
int n = 1;
while (n<size){
huffArr[n]=huffTree_merge(huffArr[n], huffArr[n-1]);
int t = n;
int j = n+1;
if (j<size){
while (j<size && (int)(intptr_t)huffArr[t]->root->weight >= (int)(intptr_t)huffArr[j]->root->weight){
huff_tree *tempHuff;
tempHuff=huffArr[t];
huffArr[t]=huffArr[j];
huffArr[j]=tempHuff;
t++;
j++;
}
}
n++;
}
return huffArr[n-1];
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
