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
    direct_graph classificazione;  // Grafo diretto che organizza gerarchicamente gli animali, le specie, famiglie...
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


zooManager zooManager_create() {
    zooManager manager = malloc(sizeof(struct _zooManager));
    if (manager == NULL) return NULL;

    manager->classificazione = direct_graph_create(100);

    // Creates the root node
    add_node(manager->classificazione, strdup("Classificazione"));

    manager->aree = hashmap_create(100);

    manager->richiesteEffettuate = create_stack(100);

    manager->alta = create_queue(100);
    manager->media = create_queue(100);
    manager->bassa = create_queue(100);

    return manager;
}

int aggiungiElemento(zooManager _manager, char* _padre, char* _elemento) {
    if (_manager == NULL) return ZOO_ERROR_NULL;
    if (_padre == NULL || _elemento == NULL) return ZOO_ERROR_NULL;

    // check if _padre exists
    node_id padre = find_node_by_value(_manager->classificazione, _padre);
    if (padre < 0) return ZOO_ERROR_NOT_FOUND;

    // Check if _elemento exists
    node_id checkElemento = find_node_by_value(_manager->classificazione, _elemento);
    if (checkElemento >= 0) return ZOO_ERROR_ALREADY_EXISTS;

    // Create the new node
    node_id nuovoElemento = add_node(_manager->classificazione, _elemento);
    if (nuovoElemento < 0) return ZOO_ERROR_NULL;

    // Adding an edge between the 2 nodes
    if (add_edge(_manager->classificazione, padre, nuovoElemento) != DIRECT_GRAPH_SUCCESS) return ZOO_ERROR_NOT_FOUND;

    return ZOO_SUCCESS;
}

int verificaSpecie(zooManager _manager, char* _specie, char* _famiglia) {
    if (_manager == NULL) return ZOO_ERROR_NULL;
    if (_specie == NULL || _famiglia == NULL) return ZOO_ERROR_NULL;

    // Check if _specie exists
    node_id specie = find_node_by_value(_manager->classificazione, _specie);
    if (specie < 0) return ZOO_ERROR_NOT_FOUND;

    // Check if _famiglia exists
    node_id famiglia = find_node_by_value(_manager->classificazione, _famiglia);
    if (famiglia < 0) return ZOO_ERROR_NOT_FOUND;

    // Check if the path between those 2 nodes exists
    return direct_graph_path_exists(_manager->classificazione, famiglia, specie);
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

    // Add the area to the manager's hashmap
    hashmap_set(_manager->aree, _codice, _area);

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


int registraNuovaRichiesta(zooManager _manager, char* _codice, char* _tipologia, char* _animaleCoinvolto, char* _area, char* _priorita, char* _descrizione) {
    if (_manager == NULL) return ZOO_ERROR_NULL;
    if (_codice == NULL || _tipologia == NULL || _area == NULL || _priorita == NULL || _descrizione == NULL) return ZOO_ERROR_NULL;


    area _areaCoinvolta;
    animale _animale = NULL;

    // Check if _area exists
    if (hashmap_get(_manager->aree, _area, (void**)&_areaCoinvolta) != 0) return ZOO_ERROR_AREA;

    // _animaleCoinvolto could also be NULL, this checks if it exists
    if (_animaleCoinvolto != NULL) {
        // Check if _animaleCoinvolto exists
        if (hashmap_get(_manager->animali, _animaleCoinvolto, (void**)&_animale) != 0) return ZOO_ERROR_ANIMAL;
    }

    // Allocate memory fo the new request
    richiesta nuovaRichiesta = malloc(sizeof(struct _richiesta));
    if (nuovaRichiesta == NULL) return ZOO_ERROR_ALLOC;

    // Fill up its values
    strcpy(nuovaRichiesta->codice, _codice);
    strcpy(nuovaRichiesta->descrizione, _descrizione);
    nuovaRichiesta->areaInteressata = _areaCoinvolta;
    nuovaRichiesta->animaleCoinvolto = _animale;


    // Convert the enum for the tipoRichiesta
    if (strcmp(_tipologia, "visita veterinaria") == 0) nuovaRichiesta->tipologia = VISITA_VETERINARIA;
    else if (strcmp(_tipologia, "alimentazione") == 0) nuovaRichiesta->tipologia = ALIMENTAZIONE;
    else if (strcmp(_tipologia, "trasferimento animale") == 0) nuovaRichiesta->tipologia = TRASFERIMENTO_ANIMALE;
    else if (strcmp(_tipologia, "manutenzione recinto") == 0) nuovaRichiesta->tipologia = MANUTENZIONE_RECINTO;
    else if (strcmp(_tipologia, "assistenza visitatori") == 0) nuovaRichiesta->tipologia = ASSISTENZA_VISITATORI;
    else {
        free(nuovaRichiesta);
        return ZOO_ERROR_NOT_FOUND;
    }

    // Convert from the enum for the priority and inserting the request into its queue
    if (strcmp(_priorita, "alta") == 0) {
        nuovaRichiesta->priorita = ALTA;
        if (enqueue(_manager->alta, nuovaRichiesta) != 0) {
            free(nuovaRichiesta);
            return ZOO_ERROR_ALLOC;
        }
    }
    else if (strcmp(_priorita, "media") == 0) {
        nuovaRichiesta->priorita = MEDIA;
        if (enqueue(_manager->media, nuovaRichiesta) != 0) {
            free(nuovaRichiesta);
            return ZOO_ERROR_ALLOC;
        }
    }
    else if (strcmp(_priorita, "bassa") == 0) {
        nuovaRichiesta->priorita = BASSA;
        if (enqueue(_manager->bassa, nuovaRichiesta) != 0) {
            free(nuovaRichiesta);
            return ZOO_ERROR_ALLOC;
        }
    }
    else {
        free(nuovaRichiesta);
        return ZOO_ERROR_NOT_FOUND;
    }
    return ZOO_SUCCESS;
}


// Private function to find a node by value
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