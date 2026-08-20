#include <stdio.h>
#include "zooManager.h"

typedef enum {SIMBA, RAJAH, GALENA, RIO, KAA, SHELLY} animali;
const char* animal_name[] = {"Simba", "Rajah", "Galena", "Rio", "Kaa", "Shelly"};

int main() {



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



    return 0;
}