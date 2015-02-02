#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hufftree.h"

#define ARR_SIZE 256


//typedef struct HuffNode {
//    char c;
//    int v;
//    struct HuffNode *leftChild,*rightChild, *parent;
//} HuffNode;

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

}
/*Sorts a HuffIntArrayTable by the number of times each char has appeared in a text,
    lowest first. */
void treesortHuffArr(HuffIntArrayTable *h, int n){
    int i;
    int x=1;
    int temp;
    for (i=(n-1)/2;i>0;i--){
        //printf("\n%d:a siftup\n", x);
        x++;
        siftup(i,n,h);
        /*
        for(int i= 0; i<h->arrNumber; i++){
            printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
        }
        printf("\n\n");
        */
    }
    x=1;
   // printf("Byt till andra\n");
    for (i=n;i>0;i--){
       //printf("\n%d:a siftup\n", x);
        x++;
        siftup(0,i,h);


        temp = h->array[0][1];
        h->array[0][1]=h->array[i-1][1];
        h->array[i-1][1]=temp;

        temp = h->array[0][0];
        h->array[0][0]=h->array[i-1][0];
        h->array[i-1][0]=temp;
        /*
        for(int i= 0; i<h->arrNumber; i++){
            printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
        }
        */
    printf("\n\n");
    }

}
huff_tree *huffEncode (HuffIntArrayTable *harr){

    //huff_tree *huff = huffTree_create();

    const int size = harr->arrNumber;
    huff_tree *huffArr[size];
//    int temp[size][2];
/*Gör en array av träd istället för en array med ints. */
    for (int i = 0; i<size; i++){
        huffArr[i] = huffTree_create();
        huffArr[i]->root->character=(data)(intptr_t)harr->array[i][0];
        huffArr[i]->root->weight=(data)(intptr_t)harr->array[i][1];
    }
    for(int x= 0; x<size; x++){
        printf("%c%d ", (char)(intptr_t)huffArr[x]->root->character, (int)(intptr_t)huffArr[x]->root->weight);
    }

    int i = 0;
    int n = 1;
    huffTree_pos pos_i;
    huffTree_pos pos_mid;
    huffTree_pos pos_n;
    int weight_i;
    int weight_mid;
    int weight_n;
//    huff_tree temptree;

/*Här slås träden ihop ett och ett i ordning. Det finns antagligen minnesläckor...*/
    while (i<n){
        printf("\n i = %d n = %d\n", i, n);
        pos_i = huffTree_root(huffArr[i]);
        pos_mid = huffTree_root(huffArr[i+1]);
        pos_n = huffTree_root(huffArr[n]);

        weight_i = (intptr_t)huffTree_inspectWeight(huffArr[i],pos_i);
        weight_mid = (intptr_t)huffTree_inspectWeight(huffArr[i+1],pos_mid);
        weight_n = (intptr_t)huffTree_inspectWeight(huffArr[n],pos_n);

        if (i == 0 && n == 1){

            huffArr[n]= huffTree_merge(huffArr[n],huffArr[i]);
            i++;
            if (n<size-2){
                n=n+2;
            }
            else if (n<size-1){
                n++;
            }

        }
        else if (i == size-2 && n == size-1){

            huffArr[n]= huffTree_merge(huffArr[n],huffArr[i]);
            i++;

        }

        else if ((int)weight_i+(int)weight_mid<=(int)weight_mid+(int)weight_n){
                 //&& (int)weight_i+(int)weight_mid<=(int)weight_i+(int)weight_n){
            huffArr[i+1]= huffTree_merge(huffArr[i+1],huffArr[i]);
            i++;
            if (n<size-1){
            n++;
            }

        }
        else if ((int)weight_i+(int)weight_n<(int)weight_mid+(int)weight_n
                 && (int)weight_i+(int)weight_n<(int)weight_mid+(int)weight_i){
            huffArr[n]= huffTree_merge(huffArr[n],huffArr[i]);
            i++;
            if (n<size-1){
                n++;
            }



        }
        else if ((int)weight_i+(int)weight_mid>=(int)weight_mid+(int)weight_n){
            huffArr[n]= huffTree_merge(huffArr[n],huffArr[i+1]);

            huffArr[i+1]=huffArr[i];

            i++;
            if (n<size-2){
                n++;
            }


        }


       for(int x= 0; x<size; x++){
       printf("%c%d ", (char)(intptr_t)huffArr[x]->root->character, (int)(intptr_t)huffArr[x]->root->weight);
    }
        //i++;



    printf("\n");
    }

    /*Här skriver jag ut root-värdet, värdet på dom två första barnen och om det högra
    barnet har barn så skrivs dom också ut*/
            printf("\n\n Root %c %d ", (char)(intptr_t)huffArr[n]->root->character, (int)(intptr_t)huffArr[n]->root->weight);

            huffTree_pos p = huffTree_root(huffArr[n]);
            p = huffTree_leftChild(huffArr[n],p);
            data testC = huffTree_inspectCharacter(huffArr[n],p);
            data testw = huffTree_inspectWeight(huffArr[n],p);
            printf("Left %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);

            p = huffTree_root(huffArr[n]);
            p = huffTree_rightChild(huffArr[n],p);
            testC = huffTree_inspectCharacter(huffArr[n],p);
            testw = huffTree_inspectWeight(huffArr[n],p);
            printf("Right %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            printf("\n\n Right Child \n");

            if (huffTree_hasLeftChild(huffArr[n],p)){
                p = huffTree_leftChild(huffArr[n],p);
                data testC = huffTree_inspectCharacter(huffArr[n],p);
                data testw = huffTree_inspectWeight(huffArr[n],p);
                printf("Left %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            }

            p = huffTree_root(huffArr[n]);
            p = huffTree_rightChild(huffArr[n],p);

            if (huffTree_hasRightChild(huffArr[n],p)){
                p = huffTree_rightChild(huffArr[n],p);
                data testC = huffTree_inspectCharacter(huffArr[n],p);
                data testw = huffTree_inspectWeight(huffArr[n],p);
                printf("Right %c %d", (int)(intptr_t)testC, (int)(intptr_t)testw);
            }
    printf("\n\n");
    for(int x= 0; x<size; x++){
        printf("%c %d ", (char)(intptr_t)huffArr[x]->root->character, (int)(intptr_t)huffArr[x]->root->weight);
    }
return huffArr[n];

}

void test_treeAgorithm (HuffIntArrayTable *harr){

    const int size = harr->arrNumber;

    int temp[size][2];

    for (int i = 0; i<size; i++){
        temp[i][0]=harr->array[i][0];
        temp[i][1]=harr->array[i][1];
    }



    int i = 0;
    int n = 1;
    while (i<n){

        if (i == n-1){
            temp[n][1]=temp[i][1]+temp[n][1];
            temp[i][1]=0;
            temp[i][0]=0;
            temp[n][0]=0;
          //  i++;
            if (n<size-2){
            n=n+2;
            }
            else if (n==size-2){
                n++;
            }
        }


        else if (temp[i][1]+temp[i+1][1]<temp[i+1][1]+temp[i+2][1]){
            temp[i+1][1]=temp[i][1]+temp[i+1][1];
            temp[i][1]=0;
            temp[i][0]=0;
            temp[i+1][0]=0;
          //  i++;
            if (n<size-2){
            n=n+2;
            }
            else if (n==size-2){
                n++;
            }
        }
        else if (temp[i][1]+temp[i+1][1]<=temp[i+1][1]+temp[i+2][1]){
            temp[i+2][1]=temp[i+1][1]+temp[i+2][1];
            temp[i+2][0]=0;

            temp[i+1][1]=temp[i][1];
            temp[i+1][0]=0;

            temp[i][1]=0;
            temp[i][0]=0;

           // i++;
            if (n<size-2){
            n=n+2;
            }
            else if (n==size-2){
                n++;
            }
        }
        i++;

        for(int x= 0; x<size; x++){
        printf("%c %d ", (char)temp[x][0], temp[x][1]);

        }


    printf("\n");
    }



}


int compareInt(void *ip,void *ip2){
    return (*(int*)ip) - (*(int*)ip2);
}



int main (void){

    char temp;
    //FILE *fp = fopen("Löremipsum.txt","r");
    FILE *fp = fopen("test.txt","r");


    HuffIntArrayTable *h = calloc(1,sizeof(HuffIntArrayTable));


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

        printf("%c", temp);

    }
    printf ("\nOSORTERAT\n");
    for (int i= 0; i<h->arrNumber; i++){
        printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
    }

    treesortHuffArr(h,h->arrNumber);

    printf ("\nSORTERAT\n");
    for(int i= 0; i<h->arrNumber; i++){
        printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
    }

    //test_treeAgorithm(h);
    huff_tree *huff = huffEncode(h);

    fclose(fp);
    free(h);


    return 0;




}
