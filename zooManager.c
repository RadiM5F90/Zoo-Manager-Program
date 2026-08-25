#include "zooManager.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>


#define MAX_CAPACITY 100

typedef enum {SANO, OSSERVAZIONE, IN_CURA, QUARANTENA} statoSalute;
typedef enum {VISITA_VETERINARIA, ALIMENTAZIONE, TRASFERIMENTO_ANIMALE, MANUTENZIONE_RECINTO, ASSISTENZA_VISITATORI} tipoRichiesta;
typedef enum {ALTA, MEDIA, BASSA} priorita;

// Helper private function to find an element in the graph
node_id find_node_by_value(direct_graph _specieAnimali, char* _value);



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

// #### CHANGE from direct_graph to zooManager ######
int aggiungiElemento(direct_graph _specieAnimali, char* _padre, char* _elemento) {
    if (_specieAnimali == NULL || _padre == NULL || _elemento == NULL) return ZOO_ERROR_NULL;

    // check if _padre exists
    node_id padre = find_node_by_value(_specieAnimali, _padre);
    if (padre < 0) return ZOO_ERROR_NULL;

    // Check if _elemento exists
    node_id checkElemento = find_node_by_value(_specieAnimali, _elemento);
    if (checkElemento >= 0) return ZOO_ERROR_NULL;

    // Create the new node
    node_id nuovoElemento = add_node(_specieAnimali, _elemento);
    if (nuovoElemento < 0) return ZOO_ERROR_NULL;

    // Adding an edge between the 2 nodes
    if (add_edge(_specieAnimali, padre, nuovoElemento) != DIRECT_GRAPH_SUCCESS) return ZOO_ERROR_NOT_FOUND;

    return ZOO_SUCCESS;
}

int verificaSpecie(direct_graph _specieAnimali, char* _specie, char* _famiglia) {
    if (_specieAnimali == NULL || _specie == NULL || _famiglia == NULL) return ZOO_ERROR_NULL;

    // Check if _specie exists
    node_id specie = find_node_by_value(_specieAnimali, _specie);
    if (specie < 0) return ZOO_ERROR_NULL;

    // Check if _famiglia exists
    node_id famiglia = find_node_by_value(_specieAnimali, _famiglia);
    if (famiglia < 0) return ZOO_ERROR_NULL;

    // Check if the path between those 2 nodes exists
    return direct_graph_path_exists(_specieAnimali, famiglia, specie);
}

int aggiungiArea(zooManager _manager, char* _codice, char* _nome, char* _tipologia, int _maxCapacity) {
    if (_manager == NULL) return ZOO_ERROR_NULL;
    if (_codice == NULL || _nome == NULL || _tipologia == NULL) return ZOO_ERROR_NULL;
    if (_maxCapacity < 0) return ZOO_ERROR_CAPACITY;

    // Controlla se l'area esiste già
    if (hashmap_has_key(_manager->aree, _codice)) return ZOO_ERROR_AREA;

    area _area = malloc(sizeof(struct _area));
    if (_area == NULL) return ZOO_ERROR_ALLOC;

    strcpy(_area->codice, _codice);
    strcpy(_area->nome, _nome);
    strcpy(_area->tipologia, _tipologia);

    _area->maxCapacity = _maxCapacity;
    _area->currentAnimalNumber = 0;

    // Crea la hashmap contenente tutti gli animali dell'area
    _area->animaliPresenti = hashmap_create(20);
    if (_area->animaliPresenti == NULL) {
        free(_area);
        return ZOO_ERROR_ALLOC;
    }

    return ZOO_SUCCESS;
}


int aggiungiAnimale(zooManager _manager, char* _codice, char* _nome, char* _specie, int _eta, char* _areaIniziale, char* _statoSalute) {
    if (_manager == NULL) return ZOO_ERROR_NULL;
    if (_codice == NULL || _nome == NULL || _specie == NULL || _areaIniziale == NULL || _statoSalute == NULL) return ZOO_ERROR_NULL;

    // Controlla se l'animale è già presente
    // Cerca l'animale nella hashmap "animali" dello zooManager
    if (hashmap_has_key(_manager->animali, _codice)) return ZOO_ERROR_ANIMAL;


    // area in cui inserire il nuovo animale
    area _area;

    // Cerca l'area. Se l'area non c'è, restituisce l'errore
    if (hashmap_get(_manager->aree, _areaIniziale, (void**)&_area) != 0) return ZOO_ERROR_AREA;

    // Controlla la capienza dell'area
    if (_area->currentAnimalNumber >= _area->maxCapacity) return ZOO_ERROR_CAPACITY;

    animale nuovoAnimale = malloc(sizeof(struct _animale));
    if (nuovoAnimale == NULL) return ZOO_ERROR_ALLOC;

    strcpy(nuovoAnimale->codice, _codice);
    strcpy(nuovoAnimale->nome, _nome);
    strcpy(nuovoAnimale->specie, _specie);

    nuovoAnimale->eta = _eta;
    nuovoAnimale->area = _area;

    // Converte lo stato di salute
    // Compara quello che scrive l'utente. Se l'utente scrive "sano", lo stato di salute sarà SANO
    if (strcmp(_statoSalute, "sano") == 0) nuovoAnimale->stato = SANO;
    else if (strcmp(_statoSalute, "sotto osservazione") == 0) nuovoAnimale->stato = OSSERVAZIONE;
    else if (strcmp(_statoSalute, "in cura") == 0) nuovoAnimale->stato = IN_CURA;
    else if (strcmp(_statoSalute, "quarantena") == 0) nuovoAnimale->stato = QUARANTENA;
    else {
        free(nuovoAnimale);
        return ZOO_ERROR_NOT_FOUND;
    }


    // Aggiunge il nuovo animale alla hashmap animali dello zooManager
    // (key = codice, value = nome del nuovo animale)
    hashmap_set(_manager->animali, _codice, nuovoAnimale);

    // Inserisce il nuovo animale nella hashmap dell'area
    hashmap_set(_area->animaliPresenti, _codice, nuovoAnimale);

    // Aumenta il numero di animali presenti nell'area
    _area->currentAnimalNumber++;

    return ZOO_SUCCESS;
}


node_id find_node_by_value(direct_graph _specieAnimali, char* _value) {
    if (_specieAnimali == NULL || _value == NULL) return DIRECT_GRAPH_ERROR_NULL;

    for (node_id i = 0; i < direct_graph_size(_specieAnimali); i++) {
        void* value;
        if (get_value(_specieAnimali, i, &value) == DIRECT_GRAPH_SUCCESS) {
            if (strcmp((char*)value, _value) == 0) return i;
        }
    }
    return DIRECT_GRAPH_ERROR_INVALID_ID;
}