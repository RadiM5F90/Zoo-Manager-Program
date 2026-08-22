#ifndef PROJECT_DEMO_ZOOMANAGER_H
#define PROJECT_DEMO_ZOOMANAGER_H

#include "hashmap.h"
#include "linked_list.h"
#include "directed_graph.h"
#include "list_queue.h"
#include "list_stack.h"


// Return codes
#define ZOO_SUCCESS 0;
#define ZOO_ERROR_NULL -1;
#define ZOO_ERROR_NOT_FOUND -2;
#define ZOO_ERROR_FAMILY -3;
#define ZOO_ERROR_CATEGORY -4;
#define ZOO_ERROR_ANIMAL -5;
#define ZOO_ERROR_ALLOC -6;
#define ZOO_ERROR_AREA -7;
#define ZOO_ERROR_CAPACITY -8;


// Structs
typedef struct _zooManager* zooManager;

typedef struct _animale* animale;

typedef struct _richiesta* richiesta;

typedef struct _area* area;

typedef struct _operazione* operazione;





// Functions

/*
 * Crea un nuovo gestore dello zoo. Alloca memoria per un nuovo zoo.
 * @return NULL se c'è stato un errore durante l'allocazione della memoria,
 *         o restituisce il nuovo zoo creato.
 */
zooManager zooManager_create();

/*
 * Aggiunge un nuovo elemento alla classificazione degli animali
 * @param _specieAnimali Grafo in cui inserire il nuovo elemento
 * @param _padre Elemento padre
 * @param _animale animale da aggiungere (di tipo struct _animale)
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_NOT_FOUND se _padre non esiste,
 *         ZOO_ERROR_NULL se _animale è già presente.
 */
int aggiungiElemento(direct_graph specieAnimali, node_id _padre, node_id _animale);

/*
 * Verifica se una specie appartiene a una determinata famiglia o categoria.
 * @param specieAnimali Grafo su cui operare.
 * @param _specie Specie da controllare.
 * @param _famiglia Famiglia o categoria da controllare.
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_FAMILY se _famiglia non esiste,
 *         ZOO_ERROR_ANIMAL se _specie non esiste.
 */
int verificaSpecie(direct_graph specieAnimali, node_id _specie, node_id _famiglia);

/*
 * Aggiunge una nuova area dello zoo.
 * L'area è di tipo struct _area. Crea una nuova area, e la inizializza con i valori passati come parametri.
 * Dato che l'area è nuova e appena aggiunta, currentAnimalNumber sarà 0.
 *
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_AREA se il codice dell'area è già esistente,
 *         ZOO_ERROR_CAPACITY se la capacità non è valida. (Creare parametri per la capacità non valida. EX: if capacity <= 0...)
 */
int aggiungiArea(zooManager _manager, char* codice, char* nome, char* tipologia, int maxCapacity);




#endif