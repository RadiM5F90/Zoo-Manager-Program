cmake_minimum_required(VERSION 4.1)
project(Project_Demo C)

set(CMAKE_C_STANDARD 11)

add_executable(Project_Demo main.c
        linked_list.c
        linked_list.h
        hashmap.c
        hashmap.h
        zooManager.c
        zooManager.h
        directed_graph.c
        directed_graph.h
)
