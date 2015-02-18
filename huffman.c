#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hufftree.h"
#include "bitset.h"
#include <string.h>
#include <unistd.h>
#define ARR_SIZE 256

typedef struct {
    char character;
    bitset *bit;
} listElement;

void exchange (int *x, int *y){
    int temp;
    temp = *x;
    *x=*y;
    *y=temp;
}
/*Hjälpfunktion för treesort*/
void siftup (int i, int n, int **h){
    int j = (2 * i)+1;
    int copyV = h[i][1];
    int copyC = h[i][0];
    while (j <= n-1){
        if (j<n-1 && h[j+1][1] > h[j][1]){
            j++;
        }
        if (h[j][1]<copyV){
            h[i][1]=copyV;
            h[i][0]=copyC;
            break;
        }
        h[i][1]=h[j][1];
        h[i][0]=h[j][0];
        i = j;
            h[i][1]=copyV;
            h[i][0]=copyC;
        j=(2*i)+1;
    }

}
/*  Använder en treesort-algoritm för att sortera en tabell implementerad med
    hjälp av en 2d-array, där h[][0] = nyckeln (ett ascii-tecken) och h[][1]
    är frekvensen som vi sorterar efter. Minsta värdet placeras först.
    Input   - 2d array gjord av pekare till pekare där andra dimensionen är 2.
            - Antal platser i första dimensionen
*/
void treesortHuffArr(int **h, int n){
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
        temp = h[0][1];
        h[0][1]=h[i-1][1];
        h[i-1][1]=temp;
        temp = h[0][0];
        h[0][0]=h[i-1][0];
        h[i-1][0]=temp;
    }

}

/*  Tar en tabell implementerad med en 2d array och gör en array av rötter till
    träd, där värdena i arr[][0] sätts in i rötternas character och
    värdena i arr[][1] sätts i i rötternas weight.
    Input   -   arr, 2d array gjord med pekare till pekare. Andra dimensionen
                måste vara 2.
            -   size, storleken på första dimensionen i arrayen.
    Output  -   Array med pekare till träd bestående av en rot.
*/

huff_tree  **buildForest(int **arr,const int size){
    huff_tree **huffArr=malloc(size*sizeof(huff_tree*));

    for (int i = 0; i<size; i++){
        huffArr[i] = huffTree_create();
        huffTree_pos p = huffTree_root(huffArr[i]);
        huffTree_setValues(huffArr[i],(data)(intptr_t)arr[i][0],
        (data)(intptr_t)arr[i][1],p);
    }
    return huffArr;
}
/*  Läser in text från en fil och kollar för varje bokstav i en frekvenstabell,
    gjord av en 2d array, så att den finns där och ökar dess frekevens med 1.
    Input   -   2d array av pekare till pekare. Andra dimensionen ska vara 2.
            -   Namnet på filen som ska läsas in.
            -   storleken på första dimensionen i arrayen
                (antalet nycklar i tabellen)
*/
void readTextToArray (int **h, char *frekvensFil, const int size){
    FILE *fp=fopen(frekvensFil,"r");
    char temp;
    for (int i = 0; i<size;i++){
        h[i][0]=i;
        h[i][1]=0;
    }
    unsigned int c;
    temp = fgetc(fp);
    while( temp != EOF){
        if (ferror(fp)) {
            break;
        }
        c=(unsigned char)temp;
        int t = h[c][1];
        h[c][1]=t+1;
        temp = fgetc(fp);
    }
    /*Sist av allt så ser vi till så att EOT finns med en gång eftersom vi
    kommer skriva in det sist i utfilen*/
    int slut = (int)'\4';
    h[slut][0]=slut;
    h[slut][1]=1;

    fclose(fp);
}

/*
    Tar in en pekare till en tom bitset. För varje iteration av treetotable
    blir bitset 1 bit längre, dessa bitar beskriver vägen igenom trädet för
    att återfinna karaktären.
    Då hufftree hittar en bokstav i positionen vi stegat till används
    bokstaven som en unsigned char, den anger positionen i arrayen l som
    ska peka på den skapade bitseten.
    input= hufftree pekare, hufftreeposition, bitsetpekare och voidpekar-
    array.
    output= inget.

*/
void treeToArray(huff_tree *h, huffTree_pos p,bitset *b,void *l[]){
    if(!huffTree_hasLeftChild(h,p) && !huffTree_hasRightChild(h,p)){
        char qq = (char)(intptr_t)huffTree_inspectCharacter(h,p);
        l[(unsigned char)qq]=b;
        p = huffTree_deleteNode(h,p);
        while(!huffTree_hasRightChild(h,p) && !huffTree_hasLeftChild(h,p)
              && p!=h->root) {
            p = huffTree_deleteNode(h,p);
        }
    }
    else {
        if(huffTree_hasLeftChild(h,p)) {
            p = huffTree_leftChild(h,p);
            bitset_setBitValue(b,bitset_size(b),false);
            treeToArray(h,p,b,l);
        }
        else {
            p = huffTree_rightChild(h,p);
            bitset_setBitValue(b,bitset_size(b),true);
            treeToArray(h,p,b,l);
        }
    }
}
/*
    Denna funktion går igenom infilen karaktär för karaktär. Lägger till
    varje karaktärs bitvärde som skapades i treeToArray till 'bitset *ut'.
    Skapar en char * av 'ut', och skriver den till fil.
    input= void *[] som innehåller bitsetpekare, filnamnspekare till infil
    & utfil.
    output= inget.
*/
void encodeText(void *l[], char *utfil, char *infil) {
     FILE *fp0 = fopen(infil,"r");
    fseek(fp0, 0L, SEEK_END);
    int fp0size = ftell(fp0);
    fclose(fp0);
    printf("%d bytes read from %s.\n", fp0size,infil);
    unsigned char temp;
    bitset *ut= bitset_empty();
    unsigned long x=0;
    FILE *utfilen = fopen(utfil, "w");
    FILE *infilen = fopen(infil, "r");
    int cntr=0;
    while( cntr < fp0size ){
        temp = fgetc(infilen);
        cntr++;
        int lengd = bitset_size(l[temp]);
        for(int i=0;i<lengd;i++){
            bitset_setBitValue(ut,x,bitset_memberOf(l[temp],i));
            x++;
        }
        if(temp==(int)'\4') {
            printf("EOT-character within the textfile.\n");
            exit(0);
        }
    }
    int lengd = bitset_size(l[(int)'\4']);
    for(int i=0;i<lengd;i++){
                bitset_setBitValue(ut,x,bitset_memberOf(l[(int)'\4'],i));
                x++;
            }
            int rest=8 - bitset_size(ut)%8;
            int tot = bitset_size(ut) + rest;
            for (int n= bitset_size(ut); n<tot; n++){
                bitset_setBitValue(ut,n,true);
            }
    char *bytearr = toByteArray(ut);
    fwrite(bytearr,sizeof(char),bitset_size(ut)/8,utfilen);
    printf("%d bytes used in encoded form.\n",bitset_size(ut)/8);
    fclose(utfilen);
    fclose(infilen);
    free(bytearr);
    bitset_free(ut);
    for(int i=0;i<ARR_SIZE;i++){
        bitset_free(l[i]);
    }
}

/*
    DecodeText stegar igenom bitset som innehåller en hel komprimerad fil.
    För varje bit ur bitset tas motsvarande steg i huffmanträdet. När ett
    löv är funnet så läses värdet och skrivs till fil. Sedan sätts pekaren
    tillbaka på Huffmanträdets rot och börjar på nästa karaktär.
    input = bitset-pekare, hufftree-pekare, filnamns-pekare.
    Returnerar inget.
*/
void decodeText(bitset *b, huff_tree *h, char *utfil) {
    FILE *fp = fopen(utfil, "w");
    int x=0;
    int c;
    huffTree_pos p=huffTree_root(h);
    int correctLength=0;
    while(x<b->length) {
        if(!huffTree_hasLeftChild(h,p) && !huffTree_hasRightChild(h,p)) {
            c=(int)(intptr_t)huffTree_inspectCharacter(h,p);
            if(c==(int)'\4') {
                if(x+8< b->length) {
                    printf("incorrect frequency analysis ");
                    printf("file or incorrect encrypted file.\n");
                    exit(0);
                }
                else {
                    printf("File decoded succesfully.\n");
                    correctLength=1;
                    x=b->length;
                }
            }
            else {
                fputc(c,fp);
                p=huffTree_root(h);
            }
        }
        else {
            bool boul = bitset_memberOf(b,x);
            x++;
            if(boul) {
                p=huffTree_rightChild(h,p);
            }
            else {
                p=huffTree_leftChild(h,p);
            }
        }
    }
    if(correctLength!=1) {
        printf("incorrect frequency analysis file or ");
        printf("incorrect encrypted file.\n");
        exit(0);
    }
    fclose(fp);
}

/*
    Hjälpfunktion för att hålla main ren och liten.
*/
void printUsage() {
    printf("USAGE:\nhuffman [OPTION] [FILE0] [FILE1] [FILE2]\n Options:\n"
    "-encode encodes FILE1 according to frequence analysis done on FILE0."
    " Stores the result in FILE2\n-decode decodes FILE1 according to"
    " frequence analysis done on FILE0. Stores the result in FILE2\n");
}

int main (int argc, char *argv[]) {
    bool encodeOrDecode=true;
    if(argc!= 5) {
        printUsage();
        exit(0);
    }
    if(strncmp(argv[1],"-encode",7)==0) {
        encodeOrDecode=true;
    }
    else if(strncmp(argv[1],"-decode",7)==0) {
        encodeOrDecode=false;
    }
    else {
        printUsage();
        exit(0);
    }
    if(access(argv[2], F_OK) ==-1) {
        printf("no access to FILE0");
        exit(0);
    }
    if(access(argv[3], F_OK) ==-1) {
        printf("no access to FILE1");
        exit(0);
    }

    int **h=(int**)malloc(sizeof(int*)*ARR_SIZE);
    for(int i=0;i<ARR_SIZE;i++){
        h[i] = (int*)malloc(sizeof(int)*2);
    }
    readTextToArray(h,argv[2],ARR_SIZE);
    treesortHuffArr(h,ARR_SIZE);
    huff_tree **huffArr=buildForest(h,ARR_SIZE);
    huff_tree *huff = buildHuffTree(huffArr,ARR_SIZE);
    free(huffArr);
    for(int j=0;j<ARR_SIZE;j++) {
        free(h[j]);
    }
    free(h);
    if(access(argv[3], F_OK) ==-1){
        printf("no access to FILE1");
        exit(0);
    }
    if(encodeOrDecode) {
        huffTree_pos pos= huffTree_root(huff);
        void  *l= malloc(sizeof(void*)*ARR_SIZE);
        while(huffTree_hasRightChild(huff,pos)){
            bitset *b = bitset_empty();
            treeToArray(huff,pos,b,l);
        }
        encodeText(l,argv[4], argv[3]);
        huffTree_free(huff);
        free(l);
    }
    else {
        bitset *b=bitsetFromFile(argv[3]);
        decodeText(b,huff,argv[4]);
        bitset_free(b);
        huffTree_free(huff);
    }
    return 0;
}
