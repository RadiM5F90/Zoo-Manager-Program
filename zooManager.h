#ifndef PROJECT_DEMO_ZOOMANAGER_H
#define PROJECT_DEMO_ZOOMANAGER_H

#include "hashmap.h"
#include "linked_list.h"
#include "directed_graph.h"
#include "list_queue.h"
#include "list_stack.h"


// Return codes
#define ZOO_SUCCESS 0
#define ZOO_ERROR_NULL -1
#define ZOO_ERROR_NOT_FOUND -2
#define ZOO_ERROR_FAMILY -3
#define ZOO_ERROR_CATEGORY -4
#define ZOO_ERROR_ANIMAL -5
#define ZOO_ERROR_ALLOC -6
#define ZOO_ERROR_AREA -7
#define ZOO_ERROR_CAPACITY -8
#define ZOO_ERROR_ALREADY_EXISTS -9


typedef struct _zooManager* zooManager;
typedef struct _animale* animale;
typedef struct _richiesta* richiesta;
typedef struct _area* area;
typedef struct _operazione* operazione;


/* F0
 * Crea un nuovo gestore dello zoo. Alloca memoria per un nuovo zoo.
 * @return NULL se c'è stato un errore durante l'allocazione della memoria,
 *         o restituisce il nuovo zoo creato.
 */
zooManager zooManager_create();

/* F1
 * Aggiunge un nuovo elemento alla classificazione degli animali
 * Prende l'animale/categoria già creati e lo aggiunge alla classificazione
 * @param _manager Manager su cui operare
 * @param _padre Elemento padre
 * @param _elemento Elemento da aggiungere
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_NOT_FOUND se _padre non esiste,
 *         ZOO_ERROR_NULL se _animale è già presente.
 */
int aggiungiElemento(zooManager _manager, char* _padre, char* _elemento);

/* F2
 * Verifica se una specie appartiene a una determinata famiglia o categoria.
 * @param _manager Manager dello zoo su cui operare
 * @param _specie Specie da controllare.
 * @param _famiglia Famiglia o categoria da controllare.
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_FAMILY se _famiglia non esiste,
 *         ZOO_ERROR_ANIMAL se _specie non esiste.
 */
int verificaSpecie(zooManager _manager, char* _specie, char* _famiglia);

/* F3
 * Aggiunge una nuova area dello zoo.
 * @param _manager Manager su cui operare.
 * @param _codice Codice identificativo dell'area.
 * @param _nome Nome dell'area.
 * @param _tipologia Tipologia di area.
 * @param _maxCapacity Capienza massima dell'area.
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_AREA se il codice dell'area è già esistente,
 *         ZOO_ERROR_CAPACITY se la capacità non è valida. (Creare parametri per la capacità non valida. EX: if capacity <= 0...)
 */
int aggiungiArea(zooManager _manager, char* _codice, char* _nome, char* _tipologia, int _maxCapacity);


/* F4
 * Aggiunge un nuovo animale alla classificazione.
 * @param _manager Manager su cui operare.
 * @param codice Codice identificativo dell'animale.
 * @param nome Nome dell'animale.
 * @param specie Specie dell'animale.
 * @param eta Eta dell'animale.
 * @param areaIniziale Area in cui l'animale verrà inserito inizialmente.
 * @param statoSalute Stato di salute dell'animale.
 * @return ZOO_SUCCESS se l'operazione di aggiunta riesce,
 *         ZOO_ERROR_ANIMAL se l'animale è già presente,
 *         ZOO_ERROR_NULL se _manager, codice, nome, specie, areaIniziale o salute sono NULL,
 *         ZOO_ERROR_CAPACITY se la capienza massima dell'area viene superata.
 */
int aggiungiAnimale(zooManager _manager, char* _codice, char* _nome, char* _specie, int _eta, char* _areaIniziale, char* _statoSalute);


/* F5
 * Registra una nuova richiesta operativa.
 * @param _manager Zoo Manager su cui operare.
 * @param _codice Codice della richiesta.
 * @param _tipologia Tipo di richiesta da effettuare.
 * @param _animaleCoinvolto Animale su cui effettuare la richiesta.
 * @param _area Area coinvolta.
 * @param _priorita Priorità della richiesta.
 * @param _descrizione Descrizione aggiuntiva sulla richiesta da effettuare.
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_ANIMAL se _animaleCoinvolto non è valido,
 *         ZOO_ERROR_AREA se _area non è valida,
 *         ZOO_ERROR_NULL se _manager è NULL.
 */
int registraNuovaRichiesta(zooManager _manager, char* _codice, char* _tipologia, char* _animaleCoinvolto, char* _area, char* _priorita, char* _descrizione);


/* F6
 * @param _manager Manager su cui operare.
 * @param _richiesta_out Puntatore su cui scrivere la richiesta selezionata dalla funzione
 * @return ZOO_SUCCESS se l'operazione riesce,
 *         ZOO_ERROR_NULL se _manager o _richiesta_out è NULL,
 *         ZOO_ERROR_NOT_FOUND se non ci sono altre richieste da eseguire e selezionare
 */
int gestisciRichiesta(zooManager _manager, richiesta* _richiesta_out);



#endif