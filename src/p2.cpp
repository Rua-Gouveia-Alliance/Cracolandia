#include <iostream>
#include <vector>

#define FINISHED 0
#define VISITING 1
#define NOTVISITED 2

typedef struct {
    int id;
    size_t weight;
} Vertex;

typedef struct {
    size_t vertex_count;
    std::vector<std::vector<Vertex>> adjacent_vertices;
} Graph;

unsigned long int get_tree_cost(std::vector<size_t> &key) {
    unsigned long int cost = 0;
    for (const auto & w : key)
        cost += w;
    return cost;
}

int get_maximum_edge(std::vector<size_t> &key, std::vector<bool> &in_queue, std::vector<int> &considered_vertices) {
    int max = 0;
    for (const auto & i : considered_vertices) {
        if (in_queue[i] && key[i] > key[max]) 
            max = i;
        else if (!in_queue[max] && in_queue[i])
            max = i;
    }
    return max;
}

bool doesnt_contain(std::vector<int> &vec, int i) {
    for (const auto & n : vec)
        if (i == n)
            return false;
    return true;
}

// Prim's algorithm variation, we want the most expensive spanning tree, not the least expensive one
unsigned long int get_maximum_cost_spanning_tree(Graph* graph, std::vector<int> &considered_vertices) {
    int edge_count = considered_vertices.size() - 1;
    std::vector<bool> in_queue(graph->vertex_count, true);
    std::vector<size_t> key(graph->vertex_count, 0);

    for (size_t i = 0; i < graph->vertex_count; i++)
        if (doesnt_contain(considered_vertices, i))
            in_queue[i] = false;

    for (int tree_size = 0; tree_size < edge_count; tree_size++) {
        int u = get_maximum_edge(key, in_queue, considered_vertices);
        in_queue[u] = false;
        for (const auto & v : graph->adjacent_vertices[u])
            if (in_queue[v.id] && v.weight > key[v.id])
                key[v.id] = v.weight;
    }
    return get_tree_cost(key);
}

void visit(Graph* graph, int vertex, std::vector<int> &stack, std::vector<int> &status) {
    status[vertex] = VISITING;
    for (const auto & v : graph->adjacent_vertices[vertex])
        if (status[v.id] == NOTVISITED)
            visit(graph, v.id, stack, status);
    status[vertex] = FINISHED;
    stack.push_back(vertex);
}

void dfs(Graph* graph, std::vector<int> &stack) {
    std::vector<int> status(graph->vertex_count, NOTVISITED);
    for (size_t i = 0; i < graph->vertex_count; i++)
        if (status[i] == NOTVISITED)
            visit(graph, i, stack, status);
}

// Applying the algorithm relying on dfs to get the graph's sccs
void get_sccs(Graph* graph, std::vector<std::vector<int>> &sccs) {
    std::vector<int> status(graph->vertex_count, NOTVISITED);
    std::vector<int> stack;
    dfs(graph, stack);
    // No need to compute G^t since all edges are bi-directional, G == G^t
    for (const auto & vertex : stack) {
        if (status[vertex] == NOTVISITED) {
            std::vector<int> scc;
            visit(graph, vertex, scc, status);
            sccs.push_back(scc);
        }
    }
}

unsigned long int compute_graph(Graph* graph) {
    unsigned long int result = 0;
    
    if (graph->vertex_count == 0)
        return 0;

    std::vector<std::vector<int>> sccs;
    get_sccs(graph, sccs);

    for (auto & vertices : sccs)
        result += get_maximum_cost_spanning_tree(graph, vertices);

    return result;
}

Graph* read_input(void) {
    size_t edge_count;
    Graph* graph = new Graph();
    std::cin >> graph->vertex_count >> edge_count;
    graph->adjacent_vertices = std::vector<std::vector<Vertex>>(graph->vertex_count);
    for (size_t i = 0; i < edge_count; i++) {
        int id1, id2;
        size_t weight;
        scanf("%d %d %ld", &id1, &id2, &weight);
        graph->adjacent_vertices[id1-1].push_back(Vertex({.id = id2-1, .weight = weight}));
        graph->adjacent_vertices[id2-1].push_back(Vertex({.id = id1-1, .weight = weight}));
    }
    return graph;
}

int main(void) {
    Graph* graph = read_input();
    std::cout << compute_graph(graph) << std::endl;
    delete graph;
}