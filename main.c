#include <stdio.h>
#include "zooManager.h"


int main() {

    zooManager _manager = zooManager_create();

    char nomeAnimale[50];
    char codiceAnimale[20];
    int etaAnimale;
    char statoSalute[50];
    char nomeSpecie[50];
    char famiglia[50];

    char nomeArea[50];
    char codiceArea[20];
    char tipologiaArea[50];
    int maxCapacity;

    char tipologiaRichiesta[50];
    char prioritaRichiesta[20];
    char codiceRichiesta[20];
    char descrizioneRichiesta[500];

    char padre[50];
    char elemento[50];


    int choice;
    do {
        printf("##### Sistema di gestione dello zoo | Menu principale #####\nScegli un opzione:\n");
        printf("0. Esci dal programma.\n");
        printf("1. Inserisci nuova richiesta operativa.\n");
        printf("2. Gestisci prossima richiesta.\n");
        printf("3. Completa richiesta già inizializzata.\n");
        printf("4. Annulla l'ultima operazione effettuata.\n");
        printf("5. Inserisci nuovo elemento nella classificazione.\n");
        printf("6. Verifica appartenenza di una specie a una categoria/famiglia.\n");
        printf("7. Inserisci nuova area.\n");
        printf("8. Inserisci nuovo animale.\n");
        scanf("%d", &choice);

        switch(choice) {
            case 0:
                printf("Uscita dal programma...\n");
                break;

            case 1:
                printf("Codice richiesta: ");
                scanf("%19s", codiceRichiesta);

                printf("\nTipologia: ");
                scanf(" %49[^\n]", tipologiaRichiesta);

                printf("\nAnimale Coinvolto: ");
                scanf(" %49[^\n]", nomeAnimale);

                printf("\nInserisci area:" );
                scanf(" %49[^\n]", nomeArea);

                printf("\nInserisci priorità richiesta: ");
                scanf(" %19[^\n]", prioritaRichiesta);

                printf("\nInserisci descrizione richiesta: ");
                scanf(" %499[^\n]", descrizioneRichiesta);

                int risultatoRichiesta = registraNuovaRichiesta(_manager, codiceRichiesta, tipologiaRichiesta, nomeAnimale, nomeArea, prioritaRichiesta, descrizioneRichiesta);
                if(risultatoRichiesta == ZOO_SUCCESS) printf("Nuova richiesta effettuata\n");
                else if (risultatoRichiesta == ZOO_ERROR_NULL) printf("\nErrore! Informazione inserite errate.\n");
                else if (risultatoRichiesta == ZOO_ERROR_AREA) printf("\nErrore! L'area inserita non esiste.\n");
                else if (risultatoRichiesta == ZOO_ERROR_ANIMAL) printf("\nErrore! %s non esiste.\n", nomeAnimale);

                break;

            case 2:
                richiesta richiestaInCarico;
                if(gestisciRichiesta(_manager, &richiestaInCarico) == ZOO_SUCCESS) {
                    printf("\nProssima richiesta da gestire individuata correttamente.");
                    printf("\nCodice richiesta presa in carico: %s", getRequestId(richiestaInCarico)); // Returns the request's code
                }
                else printf("\nNon ci sono richieste da gestire.\n");
                break;

            // TO FINISH
            case 3:
                int daCompletare = completaRichiesta(_manager);
                if (daCompletare == ZOO_SUCCESS) printf("\nRichiesta completata con successo.\n");
                else if (daCompletare == ZOO_ERROR_NULL) printf("\nErrore! Richiesta non valida.\n");
                else if (daCompletare == ZOO_ERROR_NOT_FOUND) printf("\nErrore! La richiesta non è stata ancora presa in carico.\n");

                break;

            case 4:
                printf("\nAnnullamento dell'ultima operazione effettuata in corso...\n");
                if (annullaUltimaOperazione(_manager) == ZOO_SUCCESS) printf("Annullamento ultima operazione eseguito.\n");
                else printf("Errore. Non ci sono operazioni da annullare.\n");
                break;

            case 5:
                printf("Inserisci il nome del padre a cui aggiungere il nuovo elemento: ");
                scanf("%49[^\n]", padre);

                printf("\nInserisci il nome dell'elemento da aggiungere: ");
                scanf(" %49[^\n]", elemento);

                int risultatoAggiungiElemento = aggiungiElemento(_manager, padre, elemento);

                if (risultatoAggiungiElemento == ZOO_SUCCESS) printf("\nElemento aggiunto con successo alla classificazione.\n");
                else if (risultatoAggiungiElemento == ZOO_ERROR_NOT_FOUND) printf("\nErrore! Elemento padre non trovato.\n");
                else if (risultatoAggiungiElemento == ZOO_ERROR_ALREADY_EXISTS) printf("\nErrore! l'elemento è già presente nella classificazione.\n");
                break;

            case 6:
                printf("\nInserisci il nome della specie da verificare: ");
                scanf("%49[^\n]", nomeSpecie);

                printf("\nInserisci la famiglia/categoria da controllare: ");
                scanf(" %49[^\n]", famiglia);

                int risultatoVerifica = verificaSpecie(_manager, nomeSpecie, famiglia);

                if (risultatoVerifica == ZOO_SUCCESS) printf("\n%s appartiene a %s.\n", nomeSpecie, famiglia);
                else if (risultatoVerifica == ZOO_ERROR_ANIMAL) printf("\nErrore! Specie non esistente.\n");
                else if (risultatoVerifica == ZOO_ERROR_FAMILY) printf("\nErrore! Famiglia non esistente.\n");
                break;

            case 7:
                printf("\nInserisci il codice della nuova area: ");
                scanf("%19s", codiceArea);

                printf("\nInserisci nome area: ");
                scanf(" %49[^\n]", nomeArea);

                printf("\nInserisci tipologia area:");
                scanf(" %49[^\n]", tipologiaArea);

                printf("\nInserisci capacità massima: ");
                scanf("%d", &maxCapacity);

                int result = aggiungiArea(_manager, codiceArea, nomeArea, tipologiaArea, maxCapacity);

                if (result == ZOO_SUCCESS) printf("\nArea aggiunta con successo.\n");
                else if (result == ZOO_ERROR_CAPACITY) printf("\nCapacity can't be < 0.\n");
                else if (result == ZOO_ERROR_AREA) printf("\nErrore! L'area esiste già.\n");
                else printf("\nErrore! Informazioni non valide.\n");
                break;

            case 8:
                printf("\nInserisci il codice del nuovo animale: ");
                scanf("%49[^\n]", codiceAnimale);

                printf("\nInserisci nome animale: ");
                scanf(" %49[^\n]", nomeAnimale);

                printf("\nInserisci la specie del nuovo animale: ");
                scanf(" %49[^\n]", nomeSpecie);

                printf("\nInserisci l'età del nuovo animale: ");
                scanf("%d", &etaAnimale);

                printf("\nInserisci l'area iniziale: ");
                scanf(" %49[^\n]", nomeArea);

                printf("\nInserisci stato salute: ");
                scanf(" %49[^\n]", statoSalute);

                int risultatoAggiungiAnimale = aggiungiAnimale(_manager, codiceAnimale, nomeAnimale, nomeSpecie, etaAnimale, nomeArea, statoSalute);

                if (risultatoAggiungiAnimale == ZOO_SUCCESS) printf("\nNuovo animale aggiunto.\n");
                else if (risultatoAggiungiAnimale == ZOO_ERROR_NULL) printf("\nErrore! Informazioni inserite non valide.\n");
                else if (risultatoAggiungiAnimale == ZOO_ERROR_ANIMAL) printf("\nErrore! Animale già presente");
                else if (risultatoAggiungiAnimale == ZOO_ERROR_AREA) printf("\nErrore! L'area inserita non esiste.\n");
                else if (risultatoAggiungiAnimale == ZOO_ERROR_CAPACITY) printf("\nErrore! Capienza massima dell'area raggiunta.\n");
                else if (risultatoAggiungiAnimale == ZOO_ERROR_NOT_FOUND) printf("\nErrore! Stato salute non valido.\n");
                break;

            default:
                printf("Opzione non valida.\n");
                break;
        }
    } while (choice != 0);


    return 0;
};
