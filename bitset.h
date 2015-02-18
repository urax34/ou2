/*
Datatyp:     huff_tree
Författare:  Jon Leijon
Användare:   dv14jln
Datum:       16 feb 2015

Denna datatyp skapar bitset. Ett bitset är en mängd av bitar, som lagras
i arrayen, och två ints som håller reda på hur många bitar som skrivits(length)
och hur många bitar som har allokerats minne för(capacity).
Bitset är ursprungligen skrivet av Johan Eliasson
<johane@cs.umu.se> och är fritt för användning under kursen
DV2: Algoritmer och problemlösning VT15.

Den enda ändringen är funktionen bitsetFromFile, som kan användas för att
återskapa en bitset som skrivits till fil. Det gör den genom att skapa en
bitset, allokera minne i bitsetarrayen lika stort som filen, och använda
fread för att skriva filen till bitsetarrayen.

*/
#ifndef __Huffman__BitSet__
#define __Huffman__BitSet__

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int length;
    int capacity;
    char *array;
} bitset;

//Create a new empty bitset of length 0.
bitset *bitset_empty();

//Set bit bitno to value (true=1, false=0) in the bitset b.
//bitNo should be >= 0
//If bitNo >= bitset_size(b) the bitset will be extended up to that bit. Bits from bitset_size(b)
//to bitNo will be set to zero/false.
void bitset_setBitValue(bitset *b,int bitNo,bool value);

//Get the value of bit bitNo in the bitset b. Undefined behaviour for bitNO >= bitset_size(b).
bool bitset_memberOf(bitset *b,int bitNo);

//Convert this bitset to a byte array.
//The resulting array will be large enough to contain all bits up to bitset_size(b). if bitset_size is not
//a multiple of 8 bits the final byte is padded with bits of value 0.
//Memory is dynamicly allocated for the array. The user is responsible for deallocating the memory.
char *toByteArray(bitset *b);

//Returns the size of this bitset
int bitset_size(bitset *b);

//Deallocate all memory used by a bitset
void bitset_free(bitset *b);

//Creates a bitset from the file specified by the filename.
bitset* bitsetFromFile(char *filename);
#endif /* defined(__Huffman__BitSet__) */
