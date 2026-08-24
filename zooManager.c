#include "zooManager.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>


#define MAX_CAPACITY 100

typedef enum {SANO, OSSERVAZIONE, IN_CURA, QUARANTENA} statoSalute;
typedef enum {VISITA_VETERINARIA, ALIMENTAZIONE, TRASFERIMENTO_ANIMALE, MANUTENZIONE_RECINTO, ASSISTENZA_VISITATORI} tipoRichiesta;
typedef enum {ALTA, MEDIA, BASSA} priorita;

// Helper private function to find an element in the graph
int findElement(direct_graph _graph, node_id _element);


struct _zooManager {
    direct_graph classificazione;          // Grafo diretto che organizza gerarchicamente gli animali, le specie, famiglie...
    hashmap aree;
    hashmap animali;

    stack richiesteEffettuate;     // Stack in cui inserire le richieste effettuate

    queue alta;                    // Richieste con priorità alta
    queue media;                   // Priorità media
    queue bassa;                   // Priorità bassa
};

struct _animale {
    char codice[20];       // Numero identificativo
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

    // Nuova aggiunta: hashmap contenete gli animali presenti nell'area
    hashmap animaliPresenti;
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


/*
 * inizializza il sistema di gestione dello zoo, predisponendo le strutture necessarie
 * per memorizzare la classificazione degli animali, le aree, gli animali, le richieste operative
 * e le operazioni effettuate.
 */
zooManager zooManager_create() {
    zooManager manager = malloc(sizeof(struct _zooManager));
    if (manager == NULL) return NULL;

    manager->classificazione = direct_graph_create(100);
    manager->aree = hashmap_create(100);

    manager->richiesteEffettuate = create_stack(100);

    manager->alta = create_queue(100);
    manager->media = create_queue(100);
    manager->bassa = create_queue(100);

    return manager;
}

int aggiungiElemento(direct_graph specieAnimali, node_id _padre, node_id _animale) {
    // To add a new element, take a graph and add a new node
    // take the _padre, create a new node, and add an edge between _padre and _animale
    // Use the BFS/DFS to find _padre, if not found return error, else add a new node and an edge


    return ZOO_SUCCESS;
}

int verificaSpecie(direct_graph specieAnimali, node_id _specie, node_id _famiglia) {
    return ZOO_SUCCESS;
}

int aggiungiArea(zooManager _manager, char* codice, char* nome, char* tipologia, int maxCapacity) {
    if (_manager == NULL) return ZOO_ERROR_NULL;
    if (codice == NULL || nome == NULL || tipologia == NULL) return ZOO_ERROR_NULL;
    if (maxCapacity < 0) return ZOO_ERROR_CAPACITY;

    // Controlla se l'area esiste già
    if (hashmap_has_key(_manager->aree, codice)) return ZOO_ERROR_AREA;

    area _area = malloc(sizeof(struct _area));
    if (_area == NULL) return ZOO_ERROR_ALLOC;

    strcpy(_area->codice, codice);
    strcpy(_area->nome, nome);
    strcpy(_area->tipologia, tipologia);

    _area->maxCapacity = maxCapacity;
    _area->currentAnimalNumber = 0;

    // Crea la hashmap contenente tutti gli animali dell'area
    _area->animaliPresenti = hashmap_create(20);
    if (_area->animaliPresenti == NULL) {
        free(_area);
        return ZOO_ERROR_ALLOC;
    }

    return ZOO_SUCCESS;
}


int aggiungiAnimale(zooManager _manager, char* codice, char* nome, char* specie, int eta, char* areaIniziale, char* statoSalute) {
    if (_manager == NULL) return ZOO_ERROR_NULL;
    if (codice == NULL || nome == NULL || specie == NULL || areaIniziale == NULL || statoSalute == NULL) return ZOO_ERROR_NULL;

    // Controlla se l'animale è già presente
    // Cerca l'animale nella hashmap "animali" dello zooManager
    if (hashmap_has_key(_manager->animali, codice)) return ZOO_ERROR_ANIMAL;


    // area in cui inserire il nuovo animale
    area _area;

    // Cerca l'area. Se l'area non c'è, restituisce l'errore
    if (hashmap_get(_manager->aree, areaIniziale, (void**)&_area) != 0) return ZOO_ERROR_AREA;

    // Controlla la capienza dell'area
    if (_area->currentAnimalNumber >= _area->maxCapacity) return ZOO_ERROR_CAPACITY;

    animale nuovoAnimale = malloc(sizeof(struct _animale));
    if (nuovoAnimale == NULL) return ZOO_ERROR_ALLOC;

    strcpy(nuovoAnimale->codice, codice);
    strcpy(nuovoAnimale->nome, nome);
    strcpy(nuovoAnimale->specie, specie);

    nuovoAnimale->eta = eta;
    nuovoAnimale->area = _area;

    // Converte lo stato di salute
    // Compara quello che scrive l'utente. Se l'utente scrive "sano", lo stato di salute sarà SANO
    if (strcmp(statoSalute, "sano") == 0) nuovoAnimale->stato = SANO;
    else if (strcmp(statoSalute, "sotto osservazione") == 0) nuovoAnimale->stato = OSSERVAZIONE;
    else if (strcmp(statoSalute, "in cura") == 0) nuovoAnimale->stato = IN_CURA;
    else if (strcmp(statoSalute, "quarantena") == 0) nuovoAnimale->stato = QUARANTENA;
    else {
        free(nuovoAnimale);
        return ZOO_ERROR_NOT_FOUND;
    }


    // Aggiunge il nuovo animale alla hashmap animali dello zooManager
    // (key = codice, value = nome del nuovo animale)
    hashmap_set(_manager->animali, codice, nuovoAnimale);

    // Inserisce il nuovo animale nella hashmap dell'area
    hashmap_set(_area->animaliPresenti, codice, nuovoAnimale);

    // Aumenta il numero di animali presenti nell'area
    _area->currentAnimalNumber++;

    return ZOO_SUCCESS;
}

// Helper function to find an element in the graph
int findElement(direct_graph _graph, node_id _element) {
    if (_graph == NULL) return ZOO_ERROR_NULL;


}