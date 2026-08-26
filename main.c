#include <stdio.h>
#include "zooManager.h"
#include "directed_graph.h"

typedef enum {SIMBA, RAJAH, GALENA, RIO, KAA, SHELLY} animali;
const char* animal_name[] = {"Simba", "Rajah", "Galena", "Rio", "Kaa", "Shelly"};



void print_node_list(direct_graph _g, const list _list, void(*print_value)(void*)) {
    iterator it = list_iterator_create(_list);

    while (iterator_has_next(it)) {
        int* id = (int*)iterator_next(it);
        void* value;

        if (get_value(_g, *id, &value) == DIRECT_GRAPH_SUCCESS) {
            print_value(value);
            printf(" ");
        }
    }
    iterator_destroy(&it);
    printf("\n");
}

void print_string(void* value) {
    printf("%s ", (char*)value);
}

int main() {
    // New zooManager
    zooManager _manager = zooManager_create();

    // Add animals
    aggiungiAnimale(_manager, "L1", "Simba", "Leone", 8, "area A2", "Sano");
    aggiungiAnimale(_manager, "T1", "Rajah", "Tigre", 6, "area A2", "Sano");
    aggiungiAnimale(_manager, "G1", "Galena", "Giraffa", 9, "area A1", "sotto osservazione");
    aggiungiAnimale(_manager, "A1", "Rio", "Ara", 4, "area A3", "Sano");
    aggiungiAnimale(_manager, "P1", "Kaa", "Pitone Reale", 10, "area A4", "in cura");
    aggiungiAnimale(_manager, "TR1", "Shelly", "Tartaruga Gigante", 35, "area A4", "Sano");


    aggiungiElemento(_manager, "Classificazione", "Mammiferi");
    aggiungiElemento(_manager, "Classificazione", "Rettili");
    aggiungiElemento(_manager, "Classificazione", "Uccelli");

    aggiungiElemento(_manager, "Mammiferi", "Felidi");
    aggiungiElemento(_manager, "Mammiferi", "Giraffidi");

    aggiungiElemento(_manager, "Felidi", "Leone");
    aggiungiElemento(_manager, "Felidi", "Tigre");


/*
    printf("DFS from root node: ");
    list _dfs = direct_graph_dfs(grafoClassificazione, classificazione);
    print_node_list(grafoClassificazione, _dfs, print_string);
    list_destroy(&_dfs);
*/

    return 0;
}
