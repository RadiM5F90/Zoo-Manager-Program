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

    direct_graph grafoClassificazione = direct_graph_create(200);

    node_id classificazione = add_node(grafoClassificazione, "Classificazione");

    // Add animals
    aggiungiAnimale(_manager, "L1", "Simba", "Leone", 8, "area A2", "Sano");
    aggiungiAnimale(_manager, "T1", "Rajah", "Tigre", 6, "area A2", "Sano");
    aggiungiAnimale(_manager, "G1", "Galena", "Giraffa", 9, "area A1", "sotto osservazione");
    aggiungiAnimale(_manager, "A1", "Rio", "Ara", 4, "area A3", "Sano");
    aggiungiAnimale(_manager, "P1", "Kaa", "Pitone Reale", 10, "area A4", "in cura");
    aggiungiAnimale(_manager, "TR1", "Shelly", "Tartaruga Gigante", 35, "area A4", "Sano");


    aggiungiElemento(grafoClassificazione, "Classificazione", "Mammiferi");
    aggiungiElemento(grafoClassificazione, "Classificazione", "Rettili");
    aggiungiElemento(grafoClassificazione, "Classificazione", "Uccelli");

    aggiungiElemento(grafoClassificazione, "Mammiferi", "Felidi");
    aggiungiElemento(grafoClassificazione, "Mammiferi", "Giraffidi");

    aggiungiElemento(grafoClassificazione, "Felidi", "Leone");
    aggiungiElemento(grafoClassificazione, "Felidi", "Tigre");




    /*
    // Categoria generale

    node_id Mammiferi = add_node(grafoClassificazione, "Mammiferi");
    node_id Rettili = add_node(grafoClassificazione, "Rettili");
    node_id Uccelli = add_node(grafoClassificazione, "Uccelli");

    add_edge(grafoClassificazione, classificazione, Mammiferi);
    add_edge(grafoClassificazione, classificazione, Rettili);
    add_edge(grafoClassificazione, classificazione, Uccelli);


    // Felidi
    node_id Felidi = add_node(grafoClassificazione, "Felidi");
    node_id leone = add_node(grafoClassificazione, "leone");
    node_id tigre = add_node(grafoClassificazione, "tigre");

    add_edge(grafoClassificazione, Mammiferi, Felidi);
    add_edge(grafoClassificazione, Felidi, leone);
    add_edge(grafoClassificazione, Felidi, tigre);

    // Giraffidi
    node_id Giraffidi = add_node(grafoClassificazione, "Giraffidi");
    node_id giraffa = add_node(grafoClassificazione, "giraffa");

    add_edge(grafoClassificazione, Mammiferi, Giraffidi);
    add_edge(grafoClassificazione, Giraffidi, giraffa);
*/




/*
    printf("DFS from node n1: ");
    list _dfs = direct_graph_dfs(grafoClassificazione, classificazione);
    print_node_list(grafoClassificazione, _dfs, print_string);
    list_destroy(&_dfs);

    // Verifica se il leone appartiene ai mammiferi usando direct_graph_path_exists
    printf("\nPath from Mammiferi to leone: %s\n",
        direct_graph_path_exists(grafoClassificazione, Mammiferi, leone) ? "Exists" : "Does not exist");
*/
    return 0;
}
