#include "zooManager.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>


#define MAX_CAPACITY 100

typedef enum {SANO, OSSERVAZIONE, IN_CURA, QUARANTENA} statoSalute;
typedef enum {VISITA_VETERINARIA, ALIMENTAZIONE, TRASFERIMENTO_ANIMALE, MANUTENZIONE_RECINTO, ASSISTENZA_VISITATORI} tipoRichiesta;
typedef enum {ALTA, MEDIA, BASSA} priorita;




struct _zooManager {
    //tree classificazione;          // Albero che organizza gerarchicamente gli animali, le specie, famiglie...
    hashmap aree;
    hashmap animali;

   // stack richiesteEffettuate;     // Stack in cui inserire le richieste effettuate

   // queue alta;                    // Richieste con priorità alta
   // queue media;                   // Priorità media
   // queue bassa;                   // Priorità bassa
};

struct _animale {
    char id[20];
    char nome[50];
    char specie[50];
    int eta;

    area area;            // area in cui è ospitato (struct)
    statoSalute stato;    // stato di salute dell'animale (enum)
};

struct _richiesta {
    char codice[20];                   // Codice della richiesta
    tipoRichiesta tipologia;           // tipologia di richiesta (enum)

    animale animaleCoinvolto;
    area areaInteressata;
    priorita priorita;

    char descrizione[500];
};

struct _area {
    char codice[20];            // Codice dell'area
    char nome[20];
    char tipologia[50];         // Tipo di area
    int maxCapacity;            // Capacità massima dell'area
    int currentAnimalNumber;    // Num corrente di animali ospitati
};

// per ripristinare lo stato precedente di una richiesta
struct _operazione {
    tipoRichiesta tipologia;
    animale animaleCoinvolto;

    area nuovaArea;
    area vecchiaArea;

    statoSalute statoAttuale;
    statoSalute statoPrecedente;
};


/* Manage:
 * Insert a new species -> create new left and right child nodes of a parent node (a family)
 * Insert a new animal -> function that allows to fill in all the info about the animal (animal struct), and then inserts it into its family
 * New request -> Create a new request, specifying all its details, and "send it"
 * Selecting next request -> check the priority of the request and execute the ones with the higher priority first
 * Request Completion -> Execute what the request states, and then push it into the stack
 */

/* Come verrebbero gestite le richieste
 * Quando viene presa in carico -> Si controlla la priorità, e a seconda della priorità viene presa in carico
 * Come viene aggiornata la situazione dello zoo -> Vengono aggiornate le informazioni relative alle aree o animali
 * annullare l'ultima operazione -> per annullare l'ultima operazione bisogna memorizzare tutte le informazioni della richiesta,
 * per poi ripristinarle (tipo richiesta, animale coinvolto, area...)
 */

/* Verificare se la giraffa appartiene ai mammiferi...
 * Funzione per Tree traversal che attraversa tutto l'albero (BFS or DFS), e compara il nome dell'animale da cercare, con quello dei nodi visitati
 */



/*
 * inizializza il sistema di gestione dello zoo, predisponendo le strutture necessarie
 * per memorizzare la classificazione degli animali, le aree, gli animali, le richieste operative
 * e le operazioni effettuate.
 */
zooManager zooManager_create() {
    zooManager manager = malloc(sizeof(struct _zooManager));
    if (manager == NULL) return NULL;

    //manager->classificazione = tree_create();
    manager->aree = hashmap_create(100);
    return manager;
}