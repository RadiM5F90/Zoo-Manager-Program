#include <stdio.h>
#include "zooManager.h"
#include "directed_graph.h"

typedef enum {SIMBA, RAJAH, GALENA, RIO, KAA, SHELLY} animali;
const char* animal_name[] = {"Simba", "Rajah", "Galena", "Rio", "Kaa", "Shelly"};


// ####### TURN print_node_list GENERIC #########

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
    direct_graph grafoClassificazione = direct_graph_create(200);

    // Categoria generale
    node_id classificazione = add_node(grafoClassificazione, "Classificazione");
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


    /*
    printf("DFS from node n1: ");
    list _dfs = direct_graph_dfs(grafoClassificazione, classificazione);
    print_node_list(grafoClassificazione, _dfs, print_string);
    list_destroy(&_dfs);
    */

    printf("BFS from node n1: ");
    list _bfs = direct_graph_bfs(grafoClassificazione, classificazione);
    print_node_list(grafoClassificazione, _bfs, print_string);
    list_destroy(&_bfs);


/*
    // Animals hashmap test
    hashmap hashAnimals = hashmap_create(10);

    hashmap_set(hashAnimals, "L1", 0);
    hashmap_set(hashAnimals, "T1", 1);
    hashmap_set(hashAnimals, "G1", 2);
    hashmap_set(hashAnimals, "A1", 3);
    hashmap_set(hashAnimals, "P1", 4);
    hashmap_set(hashAnimals, "TR1", 5);

    int value;

    // If... successful, then print...
    if (hashmap_get(hashAnimals, "L1", &value) == 0) printf("L1 -> %d, name -> %s\n", value, animal_name[value]);
    if (hashmap_get(hashAnimals, "T1", &value) == 0) printf("T1 -> %d, name -> %s\n", value, animal_name[value]);
    if (hashmap_get(hashAnimals, "G1", &value) == 0) printf("G1 -> %d, name -> %s\n", value, animal_name[value]);
    if (hashmap_get(hashAnimals, "A1", &value) == 0) printf("A1 -> %d, name -> %s\n", value, animal_name[value]);
    if (hashmap_get(hashAnimals, "P1", &value) == 0) printf("P1 -> %d, name -> %s\n", value, animal_name[value]);
    if (hashmap_get(hashAnimals, "TR1", &value) == 0) printf("TR1 -> %d, name -> %s\n", value, animal_name[value]);


    // Areas hashmap test

*/

    return 0;
}