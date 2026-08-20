#include <stdlib.h>
#include "directed_graph.h"
#include "list_stack.h"
#include "list_queue.h"

// ########  Turn the directed graph into a generic graph #########


// Adjacency Matrix
// i, j are the Matrix's cells, i * N * j calculates the equivalent index
#define MAP(i, j, N) ((i) * (N) + (j))

struct _direct_graph {
    int* nodes;
    int* edges;
    int size;
    int max_size;
};

direct_graph direct_graph_create(int _max_size) {
    if (_max_size <= 0) return NULL;

    direct_graph graph = malloc(sizeof(struct _direct_graph));
    if (graph == NULL) return NULL;

    graph->nodes = malloc(sizeof(int) * _max_size);
    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }

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

node_id add_node(direct_graph _graph, int value) {
    if (_graph == NULL) return DIRECT_GRAPH_ERROR_NULL;
    if (_graph->size == _graph->max_size) return DIRECT_GRAPH_ERROR_MEMORY;

    node_id new_node_id = _graph->size;
    _graph->nodes[new_node_id] = value;
    _graph->size++;

    return new_node_id;
}

int get_value(direct_graph _graph, node_id _node, int* _value_out) {
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

    for (int i = 0; i < _graph->size; i++) {
        // insert_back(_list, int value) -> function to insert an element in the back of the list
        if (_graph->edges[MAP(_node, i, _graph->max_size)] == 1) list_insert_back(_list, i);
    }
    return _list;
}

list direct_graph_dfs(direct_graph _graph, node_id _start) {
    if (_graph == NULL || _start < 0 || _start >= _graph->size) return NULL;

    bool* visited = calloc(_graph->size, sizeof(bool));
    list result = list_create();
    stack s = create_stack(_graph->size);

    stack_push(s, _start);
    while (!stack_is_empty(s)) {
        int current;
        stack_pop(s, &current);

        if (!visited[current]) {
            visited[current] = true;
            list_insert_back(result, current);

            list neighbors = direct_graph_neighbors(_graph, current);

            // list iterator
            iterator it = list_iterator_create(neighbors);

            while (iterator_has_next(it)) {
                int neighbor = iterator_next(it);
                stack_push(s, neighbor);
            }
            iterator_destroy(&it);
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
    list result = list_create();
    queue q = create_queue(_graph->size);

    enqueue(q, _start);
    while (!queue_is_empty(q)) {
        int current;
        dequeue(q, &current);

        if (!visited[current]) {
            visited[current] = true;
            list_insert_back(result, current);

            list neighbors = direct_graph_neighbors(_graph, current);
            iterator it = list_iterator_create(neighbors);

            while (iterator_has_next(it)) {
                int neighbor = iterator_next(it);
                enqueue(q, neighbor);
            }
            iterator_destroy(&it);
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
    stack s = create_stack(_graph->size);
    bool found = false;

    stack_push(s, _src);
    while (!stack_is_empty(s) && !found) {
        int current;
        stack_pop(s, &current);

        if (!visited[current]) {
            if (current == _dst) found = true;
            else {
                visited[current] = true;

                list neighbors = direct_graph_neighbors(_graph, current);
                iterator it = list_iterator_create(neighbors);

                while (iterator_has_next(it)) {
                    int neighbor = iterator_next(it);
                    stack_push(s, neighbor);
                }
                iterator_destroy(&it);
                list_destroy(&neighbors);
            }
        }
    }
    destroy_stack(&s);
    free(visited);

    return found;
}