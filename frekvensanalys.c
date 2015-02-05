#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hufftree.h"

#define ARR_SIZE 256



typedef struct HuffIntArrayTable {
    int array[ARR_SIZE][2];
    int arrNumber;

} HuffIntArrayTable;



void exchange (int *x, int *y){
    int temp;
    temp = *x;
    *x=*y;
    *y=temp;
}
/*Help functiom for the treesort*/
void siftup (int i, int n, HuffIntArrayTable *h){
    int j = (2 * i)+1;
    int copyV = h->array[i][1];
    int copyC = h->array[i][0];
    while (j <= n-1){
        if (j<n-1 && h->array[j+1][1] > h->array[j][1]){
            j++;
        }
        if (h->array[j][1]<copyV){
            h->array[i][1]=copyV;
            h->array[i][0]=copyC;
            break;
        }
        h->array[i][1]=h->array[j][1];
        h->array[i][0]=h->array[j][0];
        i = j;
            h->array[i][1]=copyV;
            h->array[i][0]=copyC;
        j=(2*i)+1;
    }
//    for(int i= 0; i<h->arrNumber; i++){
//        printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
//    }
}
/*Sorts a HuffIntArrayTable by the number of times each char has appeared in a text,
    lowest first. */
void treesortHuffArr(HuffIntArrayTable *h, int n){
    int i;
    int x=1;
    int temp;
    for (i=(n-1)/2;i>0;i--){

        x++;
        siftup(i,n,h);

    }
    x=1;

    for (i=n;i>0;i--){

        x++;
        siftup(0,i,h);


        temp = h->array[0][1];
        h->array[0][1]=h->array[i-1][1];
        h->array[i-1][1]=temp;

        temp = h->array[0][0];
        h->array[0][0]=h->array[i-1][0];
        h->array[i-1][0]=temp;

    //printf("\n\n");
    }

}
huff_tree *huffEncode (HuffIntArrayTable *harr){



    const int size = harr->arrNumber;
    huff_tree *huffArr[size];

/*Gör en array av träd istället för en array med ints. */
    for (int i = 0; i<size; i++){
        huffArr[i] = huffTree_create();
        huffTree_setMemHandler(huffArr[i],free);
        huffArr[i]->root->character=(data)(intptr_t)harr->array[i][0];
        huffArr[i]->root->weight=(data)(intptr_t)harr->array[i][1];
    }
    for(int x= 0; x<size; x++){
        printf("%c%d ", (char)(intptr_t)huffArr[x]->root->character, (int)(intptr_t)huffArr[x]->root->weight);
    }


    int n = 1;

    huffTree_pos pos_n;
    data weight_t;
    data weight_j;

/*Här slås träden ihop ett och ett i ordning. Det finns antagligen minnesläckor...*/
    while (n<size){
        //printf("\n i = %d n = %d\n", i, n);
        /*pos_i = huffTree_root(huffArr[i]);
        pos_n = huffTree_root(huffArr[i+1]);
        weight_i = huffTree_inspectWeight(huffArr[i],pos_i);
        weight_n = huffTree_inspectWeight(huffArr[i+1],pos_n);
        */

        huffArr[n]=huffTree_merge(huffArr[n], huffArr[n-1]);
        int t = n;
        int j = n+1;

        if (j<size){

            //weight_t = huffTree_inspectWeight(huffArr[t],huffTree_root(huffArr[t]));
            //weight_j = huffTree_inspectWeight(huffArr[j],huffTree_root(huffArr[j]));
            while (j<size && (int)(intptr_t)huffArr[t]->root->weight >(int)(intptr_t)huffArr[j]->root->weight){

                weight_t = huffTree_inspectWeight(huffArr[t],huffTree_root(huffArr[t]));
                weight_j = huffTree_inspectWeight(huffArr[j],huffTree_root(huffArr[j]));

                printf("\n t = %d j = %d\n", (int)(intptr_t)weight_t, (int)(intptr_t)weight_j);

                huff_tree *tempHuff = huffTree_create();


        for(int x= 0; x<size; x++){
            printf("%c%d ", (char)(intptr_t)huffArr[x]->root->character, (int)(intptr_t)huffArr[x]->root->weight);
        }

        printf("\n\n");
                tempHuff=huffArr[t];
                huffArr[t]=huffArr[j];
                huffArr[j]=tempHuff;
                t++;
                j++;
            }
        }
        n++;


    }
    /*Här skriver jag ut root-värdet, värdet på dom två första barnen och om det högra
    barnet har barn så skrivs dom också ut*/

            printf("\n\n Root %c %d ", (char)(intptr_t)huffArr[n-1]->root->character, (int)(intptr_t)huffArr[n-1]->root->weight);

            huffTree_pos p = huffTree_root(huffArr[n-1]);
            p = huffTree_leftChild(huffArr[n-1],p);
            data testC = huffTree_inspectCharacter(huffArr[n-1],p);
            data testw = huffTree_inspectWeight(huffArr[n-1],p);
            printf("Left %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);

            p = huffTree_root(huffArr[n-1]);
            p = huffTree_rightChild(huffArr[n-1],p);
            testC = huffTree_inspectCharacter(huffArr[n-1],p);
            testw = huffTree_inspectWeight(huffArr[n-1],p);
            printf("Right %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            printf("\n\n Right Child \n");

            if (huffTree_hasLeftChild(huffArr[n-1],p)){
                p = huffTree_leftChild(huffArr[n-1],p);
                data testC = huffTree_inspectCharacter(huffArr[n-1],p);
                data testw = huffTree_inspectWeight(huffArr[n-1],p);
                printf("Left %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            }

            p = huffTree_root(huffArr[n-1]);
            p = huffTree_rightChild(huffArr[n-1],p);

            if (huffTree_hasRightChild(huffArr[n-1],p)){
                p = huffTree_rightChild(huffArr[n-1],p);
                data testC = huffTree_inspectCharacter(huffArr[n-1],p);
                data testw = huffTree_inspectWeight(huffArr[n-1],p);
                printf("Right %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            }
            printf("\n\n Left Child \n");
            p = huffTree_root(huffArr[n-1]);
            p = huffTree_leftChild(huffArr[n-1],p);
            if (huffTree_hasLeftChild(huffArr[n-1],p)){
                p = huffTree_leftChild(huffArr[n-1],p);
                data testC = huffTree_inspectCharacter(huffArr[n-1],p);
                data testw = huffTree_inspectWeight(huffArr[n-1],p);
                printf("Left %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            }

            p = huffTree_root(huffArr[n-1]);
            p = huffTree_leftChild(huffArr[n-1],p);

            if (huffTree_hasRightChild(huffArr[n-1],p)){
                p = huffTree_rightChild(huffArr[n-1],p);
                data testC = huffTree_inspectCharacter(huffArr[n-1],p);
                data testw = huffTree_inspectWeight(huffArr[n-1],p);
                printf("Right %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            }
    printf("\n\n");

    //for(int x= 0; x<2; x++){


    //    if (huffArr[x]->root!=NULL)
            //free(huffArr[x]->root);

        //huffTree_deleteNode(huffArr[x]->root);
        //    huffTree_free(huffArr[x]);

    //}

    return huffArr[n-1];
}


int compareInt(void *ip,void *ip2){
    return (*(int*)ip) - (*(int*)ip2);
}

void readTextToArray (HuffIntArrayTable *h, FILE *fp){
    char temp;

    while((temp = fgetc(fp)) != EOF){

        if (ferror(fp)) {
            break;
        }
        for (int i=0;i<ARR_SIZE;i++){
            if (h->arrNumber==0){
                h->array[0][0]=(int)temp;
                h->array[0][1]=1;
                h->arrNumber++;

                break;
            }
            else if (h->array[i][0]==temp){
                h->array[i][0]=(int)temp;
                h->array[i][1]=h->array[i][1] + 1;


                break;
            }
            else if (i==h->arrNumber){
                h->array[i][0]=(int)temp;
                h->array[i][1]= 1;
                h->arrNumber++;

                break;
            }
        }
    }
}


int main (void){


    //FILE *fp = fopen("Löremipsum.txt","r");
    FILE *fp = fopen("test.txt","r");
    HuffIntArrayTable *h = calloc(1,sizeof(HuffIntArrayTable));
    readTextToArray(h,fp);
//        printf ("\nOSORTERAT\n");
//    for (int i= 0; i<h->arrNumber; i++){
//        printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
//    }
    treesortHuffArr(h,h->arrNumber);
//        printf ("\nSORTERAT\n");
//    for(int i= 0; i<h->arrNumber; i++){
//        printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
//    }
    huff_tree *huff = huffEncode(h);
    fclose(fp);
    free(h);
    //huffTree_free(huff);
    //free(huff);
//    if(huff->freeFunc && huffTree_hasCharacter(huff,huff->root))
//        huff->freeFunc(huff->root->character);
//    if(huff->freeFunc && huffTree_hasWeight(huff,huff->root))
//        huff->freeFunc(huff->root->weight);
//    free(huff->root);
//    free(huff);
    return 0;




}
