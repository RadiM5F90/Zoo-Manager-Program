#ifndef PROJECT_DEMO_ZOOMANAGER_H
#define PROJECT_DEMO_ZOOMANAGER_H

#include "hashmap.h"
#include "linked_list.h"


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
 * @param _animal animale da aggiungere (di tipo struct animal)
 */


#endif