#ifndef PROJECT_DEMO_DIRECTED_GRAPH_H
#define PROJECT_DEMO_DIRECTED_GRAPH_H

#include "linked_list.h"

#define DIRECT_GRAPH_SUCCESS 0;
#define DIRECT_GRAPH_ERROR_NULL -1;
#define DIRECT_GRAPH_ERROR_INVALID_ID -2;
#define DIRECT_GRAPH_ERROR_MEMORY -3;

typedef struct _direct_graph* direct_graph;
typedef int node_id;

direct_graph direct_graph_create(int _max_size);

void direct_graph_destroy(direct_graph* _graph);

node_id add_node(direct_graph _graph, int value);

int get_value(direct_graph _graph, node_id _node, int* _value_out);

int add_edge(direct_graph _graph, node_id _src, node_id _dst);

int remove_edge(direct_graph _graph, node_id _src, node_id _dst);

int direct_graph_size(direct_graph _graph);

int direct_graph_adjacent(direct_graph _graph, node_id _src, node_id _dst);

int direct_graph_path_exists(direct_graph _graph, node_id _src, node_id _dst);


list direct_graph_neighbors(direct_graph _graph, node_id _node);

list direct_graph_dfs(direct_graph _graph, node_id _start);

list direct_graph_bfs(direct_graph _graph, node_id _start);


#endif //PROJECT_DEMO_DIRECTED_GRAPH_H