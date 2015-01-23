#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void){

    char temp;
    FILE *fp = fopen("Löremipsum.txt","r");
    //FILE *fp = fopen("test.txt","r");


    int i = 0;
    while (!feof(fp)) {
        fscanf(fp, "%c", &temp);
        printf("%c", temp);
        i++;
    }
    //printf("%d", i);
    fclose(fp);

    return 0;




}
