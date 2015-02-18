/*
Datatyp:     huff_tree
Författare:  Stefan Bränberg
Användare:   dv14sbg
Datum:       16 feb 2015

Denna datatyp är ett huffmanträd som består av ett modifierat binärt träd.
Det binära trädet är ursprungligen skrivet att av Johan Eliasson <johane@cs.umu.se>
och är fritt för användning under kursen DV2: Algoritmer och problemlösning VT15.

Ändringarna består i stora drag av att två funktioner har lagts till - en för att
sätta ihop två träd till ett under en rot, och en för att från en ordnad tabell
bygga ett Huffmanträd. Det kan vara värt att nämna att även fast vi har kallat
datatypen för huff_tree så är det inte ett färdigt Huffmanträd förrän man har
kört funktionen builfHuffTree.

Vi har tagit bort insert-funktionerna eftersom det inte finns någon poäng
med att själv sätta in noder i trädet.

*/


#include <stdbool.h>

#ifndef __DATA
#define __DATA
typedef void *data;
#endif

#ifndef __MEMFREEDATAFUNC
#define __MEMFREEDATAFUNC
typedef void memFreeFunc(data);
#endif

typedef struct Huff_Node {
    struct Huff_Node *parent;
    struct Huff_Node *leftChild;
    struct Huff_Node *rightChild;
    data character;
    data weight;
} node;

typedef node * huffTree_pos;

typedef struct {
    huffTree_pos root;
    memFreeFunc *freeFunc;
} huff_tree;


/*
Syfte: Skapa ett nytt binärt träd med en rotnod
Returvärde: pekare till det nyskapade trädet
Kommentarer:
*/
huff_tree *huffTree_create();

/*
Syfte: Installera en minneshanterare för trädet så att den kan ta över
       ansvaret för att avallokera minnet för värdena då de ej finns kvar
       i trädet mer.
Parametrar: t - trädet
            f - en funktionspekare till en funktion som tar in ett värde
                av typen data (void pekare) och avallokerar minnet för värdet
                den pekar på (ett av den typ av värden som lagras i trädet).
Kommentarer: Bör anropas direkt efter att trädet skapats om funktionaliteten
             ska användas. Trädet funkar även utan att denna funktion anropas,
             men det är då upp till användaren av datatypen att avallokera allt
             minne för datat som lagras i trädet.
*/
void huffTree_setMemHandler(huff_tree *t, memFreeFunc *f);

/*
Syfte: Hämta positionen för rotnoden
Parametrar: trädet
Returvärde: positionen för rotnden
Kommentarer:
*/
huffTree_pos huffTree_root(huff_tree *tree);

/*
Syfte: Kontrollera om en nod i trädet har ett vänsterbarn
Parametrar: tree - trädet
            n - positionen som man vill undersöka om den har ett vänsterbarn
Returvärde: true om et vänsterbarn fanns, false annars
Kommentarer:
*/
bool huffTree_hasLeftChild(huff_tree *tree,huffTree_pos n);

/*
Syfte: Kontrollera om ett högerbarn finns till en given nod
Parametrar: tree - trädet
            n - positionen som man vill undersöka om den har ett högerbarn
Returvärde: true om et högerbarn fanns, false annars
Kommentarer:
*/
bool huffTree_hasRightChild(huff_tree *tree,huffTree_pos n);

/*
Syfte: Hämta högra barnets position för en given nod
Parametrar: tree - trädet
            n - positionen vars högerbarn man vill hämta
Returvärde:  positionen för högerbarnet
Kommentarer: Ej definierad om ett högerbarn inte finns
*/
huffTree_pos huffTree_rightChild(huff_tree *tree,huffTree_pos n);

/*
Syfte: Hämta vänstra barnets position för en given nod
Parametrar: tree - trädet
            n - positionen vars vänsterbarn man vill hämta
Returvärde: positionen för vänsterbarnet
Kommentarer: Ej definierad om ett vänsterbarn inte finn
*/
huffTree_pos huffTree_leftChild(huff_tree *tree,huffTree_pos n);

/*
Syfte: Hämta positionen för föräldern
Parametrar: tree - trädet
            n - positionen vars förälder man vill hämta
Returvärde: positionen för föräldern
Kommentarer: Ej definierad för rotnoden
*/
huffTree_pos huffTree_parent(huff_tree *tree,huffTree_pos n);

/*
Syfte: Hämta en nods tecken (character).
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: Värdet på nodens tecken (character).
Kommentarer:
*/
data huffTree_inspectCharacter(huff_tree *tree,huffTree_pos n);
/*
Syfte: Hämta en nods vikt (weight).
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: Värdet på nodens vikt (weight).
Kommentarer:
*/
data huffTree_inspectWeight(huff_tree *tree,huffTree_pos n);
/*
Syfte: Kontrollera om det finns ett tecken i noden
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: true om noden har ett tecken annars false
Varning: Om tecknet är tecknet är null char så returneras false.
*/
bool huffTree_hasCharacter(huff_tree *tree,huffTree_pos n);
/*
Syfte: Kontrollera om det finns en vikt i noden
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: true om noden har en vikt annars false

*/

bool huffTree_hasWeight(huff_tree *tree,huffTree_pos n);
/*
Syfte: Sätta en ett tecken och en vikt för en nod
Parametrar: tree - trädet
            character - tecknet för noden
            weight - vikten för noden
            n - positionen för noden
Kommentarer:
*/
void huffTree_setValues(huff_tree *tree,data character, data weight,huffTree_pos n);

/*
Syfte: Sätta ihop två träd under en gemensam rot.
Parametrar: Två träd
Returvärde: Det sammansatta trädet
*/

huff_tree *huffTree_merge (huff_tree *treeLeft, huff_tree *treeRight);

/*
Syfte:  Att bygga ett huffmanträd efter frekvenserna i en sorterad tabell implementerad mha en 2d array.
        Vi skapar en array av storleken size med ett träd på varje arrayplats och använder oss av huffTree_merge för att
        sätta ihop de två träd som har minst vikt tills det bara finns ett träd kvar som returneras.

Parametrar: harr - en sorterad tabell på formen harr[][2], där harr[][0] består av tecken och harr[][1] är vikten.
            size - storleken på första dimensionen i arrayen
Returvärde: Det resulterande huffmanträdet
Kommentarer: Tabellen måste vara sorterad med minsta vikt först
*/

huff_tree *buildHuffTree (int **harr, const int size);

/*
Syfte: Ta bort en nod ur trädet
Parametrar: tree - trädet
            n - positionen för noden
Returvärde: position för föräldranoden till noden som togs bort
Kommentarer: Om noden som ska tas bort har barn kommer även dessa tas bort
*/
huffTree_pos huffTree_deleteNode(huff_tree *tree,huffTree_pos n);

/*
Syfte: Avallokera minnet för trädet
Parametrar: tree - trädet
Kommentarer: efter anropet så är tree ej längre definierat
*/
void huffTree_free(huff_tree *tree);


