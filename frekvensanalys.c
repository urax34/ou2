#include <stdio.h>
#include <stdlib.h>


#define ARR_SIZE 256


typedef struct HuffNode {
    char c;
    int v;
    struct HuffNode *leftChild,*rightChild, *parent;
} HuffNode;

typedef struct HuffArray {
    int array[ARR_SIZE][2];
    int arrNumber;

} HuffArray;



void exchange (int *x, int *y){
    int temp;
    temp = *x;
    *x=*y;
    *y=temp;
}

void siftup (int i, int n, HuffArray *h){
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

void treesortHuffArr(HuffArray *h, int n){
    int i;
    int temp;
    for (i=(n/2)-1;i>0;i--){
        siftup(i,n,h);

    }


    for (i=n-1;i>0;i--){
    siftup(0,i,h);


    temp = h->array[0][1];
    h->array[0][1]=h->array[i][1];
    h->array[i][1]=temp;

    temp = h->array[0][0];
    h->array[0][0]=h->array[i][0];
    h->array[i][0]=temp;

    }

}

int compareInt(void *ip,void *ip2){
    return (*(int*)ip) - (*(int*)ip2);
}

void sortArray(int *arr){

}

int main (void){

    char temp;
    FILE *fp = fopen("Löremipsum.txt","r");


    HuffArray *h = calloc(1,sizeof(HuffArray));


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

    for (int i= 0; i<h->arrNumber; i++){
        printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
    }

    treesortHuffArr(h,h->arrNumber);

    printf ("\nSORTERAT\n");
    for(int i= 0; i<h->arrNumber; i++){
        printf("%c %d\n", (char)h->array[i][0], h->array[i][1]);
    }


    fclose(fp);
    free(h);


    return 0;




}
