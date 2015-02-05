#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hufftree.h"
#include "dlist.h"
#include "bitset.h"

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
    }

}
huff_tree *huffEncode (HuffIntArrayTable *harr){

    const int size = harr->arrNumber;
    huff_tree *huffArr[size];

/*Gör en array av träd istället för en array med ints. */
    for (int i = 0; i<size; i++){
        huffArr[i] = huffTree_create();
        huffArr[i]->root->character=(data)(intptr_t)harr->array[i][0];
        huffArr[i]->root->weight=(data)(intptr_t)harr->array[i][1];
    }

    int n = 1;

    while (n<size){


        huffArr[n]=huffTree_merge(huffArr[n], huffArr[n-1]);

        int t = n;
        int j = n+1;

        if (j<size){

            while (j<size && (int)(intptr_t)huffArr[t]->root->weight >(int)(intptr_t)huffArr[j]->root->weight){

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

    free(huff->root);
    free(huff);
    free(h);

    return 0;

}
