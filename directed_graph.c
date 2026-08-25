#include <stdlib.h>
#include "directed_graph.h"
#include "list_stack.h"
#include "list_queue.h"

// Adjacency Matrix
// i, j are the Matrix's cells, i * N * j calculates the equivalent index
#define MAP(i, j, N) ((i) * (N) + (j))

struct _direct_graph {
    void** nodes;
    int* edges;
    int size;
    int max_size;
};

direct_graph direct_graph_create(int _max_size) {
    if (_max_size <= 0) return NULL;

    direct_graph graph = malloc(sizeof(struct _direct_graph));
    if (graph == NULL) return NULL;

    graph->nodes = malloc(sizeof(void*) * _max_size);
    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }

    // Adjacency Matrix
    graph->edges = calloc(_max_size * _max_size, sizeof(int));
    if (graph->edges == NULL) {
        free(graph->edges);
        free(graph);
        return NULL;
    }

    graph->size = 0;
    graph->max_size = _max_size;

    return graph;
}

void direct_graph_destroy(direct_graph* _graph) {
    if (_graph == NULL || *_graph == NULL) return;

    free((*_graph)->nodes);
    free((*_graph)->edges);
    free(*_graph);

    *_graph = NULL;
}

node_id add_node(direct_graph _graph, void* value) {
    if (_graph == NULL) return DIRECT_GRAPH_ERROR_NULL;
    if (_graph->size == _graph->max_size) return DIRECT_GRAPH_ERROR_MEMORY;

    node_id new_node_id = _graph->size;
    _graph->nodes[new_node_id] = value;
    _graph->size++;

    return new_node_id;
}

int get_value(direct_graph _graph, node_id _node, void** _value_out) {
    if (_graph == NULL || _value_out == NULL) return DIRECT_GRAPH_ERROR_NULL;
    if (_node < 0 || _node >= _graph->size) return DIRECT_GRAPH_ERROR_INVALID_ID;

    *_value_out = _graph->nodes[_node];

    return DIRECT_GRAPH_SUCCESS;
}

int add_edge(direct_graph _graph, node_id _src, node_id _dst) {
    if (_graph == NULL) return DIRECT_GRAPH_ERROR_NULL;
    if (_src < 0 || _src >= _graph->size || _dst < 0 || _dst >= _graph->size)
        return DIRECT_GRAPH_ERROR_INVALID_ID;

    // pass in the source and dst nodes in MAP, and add an edge between them
    _graph->edges[MAP(_src, _dst, _graph->max_size)] = 1; // = 1 -> adds an edge
    return DIRECT_GRAPH_SUCCESS;
}

int remove_edge(direct_graph _graph, node_id _src, node_id _dst) {
    if (_graph == NULL) return DIRECT_GRAPH_ERROR_NULL;
    if (_src < 0 || _src >= _graph->size || _dst < 0 || _dst >= _graph->size)
        return DIRECT_GRAPH_ERROR_INVALID_ID;

    // pass in the source and dst nodes in MAP, and remove the edge between them
    _graph->edges[MAP(_src, _dst, _graph->max_size)] = 0; // = 0 -> removes the edge
    return DIRECT_GRAPH_SUCCESS;
}

int direct_graph_size(direct_graph _graph) {
    if (_graph == NULL) return DIRECT_GRAPH_ERROR_NULL;

    return _graph->size;
}

// 2 nodes are adjacent if there is an edge between them, if they are neighbors
int direct_graph_adjacent(direct_graph _graph, node_id _src, node_id _dst) {
    if (_graph == NULL) return DIRECT_GRAPH_ERROR_NULL;
    if (_src < 0 || _src >= _graph->size || _dst < 0 || _dst >= _graph->size)
        return DIRECT_GRAPH_ERROR_INVALID_ID;

    return _graph->edges[MAP(_src, _dst, _graph->max_size)];
}

list direct_graph_neighbors(direct_graph _graph, node_id _node) {
    if (_graph == NULL || _node < 0 || _node >= _graph->size) return NULL;

    list _list = list_create();
    if (_list == NULL) return NULL;

    for (int i = 0; i < _graph->size; i++) {

        if (_graph->edges[MAP(_node, i, _graph->max_size)] == 1) {
            /*
             * The linked list is generic, and expects void*
             * i is an int, so we allocate an int to store the node ID inside it
             */
            int* neighbor = malloc(sizeof(int));

            if (neighbor == NULL) {
                list_destroy(&_list);
                return NULL;
            }
            *neighbor = i;
            list_insert_back(_list, neighbor);
        }
    }
    return _list;
}

list direct_graph_dfs(direct_graph _graph, node_id _start) {
    if (_graph == NULL || _start < 0 || _start >= _graph->size) return NULL;

    bool* visited = calloc(_graph->size, sizeof(bool));
    if (visited == NULL) return NULL;

    list result = list_create();
    if (result == NULL) {
        free(visited);
        return NULL;
    }

    stack s = create_stack(_graph->size);
    if (s == NULL) {
        list_destroy(&result);
        free(visited);
        return NULL;
    }

    /*
     * Since the stack is generic, we can't push _start directly.
     * We allocate an int containing the node ID.
     */
    int* start_id = malloc(sizeof(int));
    if (start_id == NULL) {
        destroy_stack(&s);
        list_destroy(&result);
        free(visited);
        return NULL;
    }

    *start_id = _start;
    stack_push(s, start_id);

    while (!stack_is_empty(s)) {
        void* value;
        stack_pop(s, &value);

        int* current_ptr = (int*)value;
        int current = *current_ptr;
        free(current_ptr);

        if (!visited[current]) {
            visited[current] = true;

            // store the node ID in the generic list
            int* result_id = malloc(sizeof(int));
            if (result_id == NULL) {
                destroy_stack(&s);
                list_destroy(&result);
                free(visited);
                return NULL;
            }
            *result_id = current;
            list_insert_back(result, result_id);

            // Getting neighbors
            list neighbors = direct_graph_neighbors(_graph, current);

            // list iterator
            iterator it = list_iterator_create(neighbors);

            while (iterator_has_next(it)) {
                int* neighbor = (int*)iterator_next(it);

                int* stack_id = malloc(sizeof(int));
                if (stack_id == NULL) {
                    iterator_destroy(&it);
                    list_destroy(&neighbors);
                    list_destroy(&result);
                    destroy_stack(&s);
                    free(visited);
                    return NULL;
                }
                *stack_id = *neighbor;
                stack_push(s, stack_id);
            }
            iterator_destroy(&it);


            // Freeing the int* values returned by direct_graph_neighbors().
            iterator it_free = list_iterator_create(neighbors);
            while (iterator_has_next(it_free)) {
                int* neighbor = (int*)iterator_next(it_free);
                free(neighbor);
            }
            iterator_destroy(&it_free);
            list_destroy(&neighbors);
        }
    }
    destroy_stack(&s);
    free(visited);
    return result;
}

list direct_graph_bfs(direct_graph _graph, node_id _start) {
    if (_graph == NULL || _start < 0 || _start >= _graph->size) return NULL;

    bool* visited = calloc(_graph->size, sizeof(bool));
    if (visited == NULL) return NULL;

    list result = list_create();
    if (result == NULL) {
        free(visited);
        return NULL;
    }

    queue q = create_queue(_graph->size);
    if (q == NULL) {
        list_destroy(&result);
        free(visited);
        return NULL;
    }

    /*
     * Since the queue is generic, we allocate an int
     * containing the starting node ID.
     */
    int* start_id = malloc(sizeof(int));
    if (start_id == NULL) {
        destroy_queue(&q);
        list_destroy(&result);
        free(visited);
        return NULL;
    }

    *start_id = _start;
    enqueue(q, start_id);

    while (!queue_is_empty(q)) {
        void* value;
        dequeue(q, &value);

        int* current_ptr = (int*)value;
        int current = *current_ptr;
        free(current_ptr);

        if (!visited[current]) {
            visited[current] = true;

            // store the node ID into the result list
            int* result_id = malloc(sizeof(int));
            if (result_id == NULL) {
                destroy_queue(&q);
                list_destroy(&result);
                free(visited);
                return NULL;
            }
            *result_id = current;
            list_insert_back(result, result_id);

            list neighbors = direct_graph_neighbors(_graph, current);
            if (neighbors == NULL) continue;

            iterator it = list_iterator_create(neighbors);

            while (iterator_has_next(it)) {
                int* neighbor = (int*)iterator_next(it);

                // Creating a separate int for the queue
                int* queue_id = malloc(sizeof(int));
                if (queue_id == NULL) {
                    iterator_destroy(&it);
                    list_destroy(&neighbors);
                    destroy_queue(&q);
                    list_destroy(&result);
                    free(visited);
                    return NULL;
                }
                *queue_id = *neighbor;
                enqueue(q, queue_id);
            }
            iterator_destroy(&it);

            // Freeing the neighbor values stored in the temporary list
            iterator it_free = list_iterator_create(neighbors);
            while (iterator_has_next(it_free)) {
                int* neighbor = (int*)iterator_next(it_free);
                free(neighbor);
            }
            iterator_destroy(&it_free);
            list_destroy(&neighbors);
        }
    }
    destroy_queue(&q);
    free(visited);
    return result;
}

int direct_graph_path_exists(direct_graph _graph, node_id _src, node_id _dst) {
    if (_graph == NULL) return DIRECT_GRAPH_ERROR_NULL;
    if (_src < 0 || _src >= _graph->size || _dst < 0 || _dst >= _graph->size)
        return DIRECT_GRAPH_ERROR_INVALID_ID;

    bool* visited = calloc(_graph->size, sizeof(bool));
    if (visited == NULL) return DIRECT_GRAPH_ERROR_MEMORY;

    stack s = create_stack(_graph->size);
    if (s == NULL) {
        free(visited);
        return DIRECT_GRAPH_ERROR_MEMORY;
    }

    bool found = false;

    int* start_id = malloc(sizeof(int));
    if (start_id == NULL) {
        destroy_stack(&s);
        free(visited);
        return DIRECT_GRAPH_ERROR_MEMORY;
    }
    *start_id = _src;
    stack_push(s, start_id);

    while (!stack_is_empty(s) && !found) {
        void* value;
        stack_pop(s, &value);

        int* current_ptr = (int*)value;
        int current = *current_ptr;
        free(current_ptr);

        if (!visited[current]) {
            if (current == _dst) found = true;

            else {
                visited[current] = true;

                list neighbors = direct_graph_neighbors(_graph, current);
                if (neighbors == NULL) continue;

                iterator it = list_iterator_create(neighbors);

                while (iterator_has_next(it)) {
                    int* neighbor = (int*)iterator_next(it);

                    int* stack_id = malloc(sizeof(int));
                    if (stack_id == NULL) {
                        iterator_destroy(&it);
                        list_destroy(&neighbors);
                        destroy_stack(&s);
                        free(visited);
                        return DIRECT_GRAPH_ERROR_MEMORY;
                    }
                    *stack_id = *neighbor;
                    stack_push(s, stack_id);
                }
                iterator_destroy(&it);

                // Freeing the temporary neighbor IDs.
                iterator it_free = list_iterator_create(neighbors);
                while (iterator_has_next(it_free)) {
                    int* neighbor = (int*)iterator_next(it_free);
                    free(neighbor);
                }
                iterator_destroy(&it_free);
                list_destroy(&neighbors);
            }
        }
    }
    /*
     * If we stopped because we found the destination,
     * there may still be dynamically allocated node IDs
     * inside the stack. Clean them up before destroying it.
     */
    while (!stack_is_empty(s)) {
        void* value;
        stack_pop(s, &value);
        free(value);
    }

    destroy_stack(&s);
    free(visited);

    return found;
}