#include <iostream>
#include <vector>
#include <cmath>

struct edge_t {
    int u;
    int v;
    size_t weight;
};

struct graph_t {
    int size;
    std::vector<int> ranks;
    std::vector<int> parents;
    std::vector<edge_t*> data;
};

void make_set(graph_t* graph, int x) {
    graph->parents[x] = x;
}

int find_set(graph_t* graph, int x) {
    if (x != graph->parents[x])
        graph->parents[x] = find_set(graph, graph->parents[x]);
    return graph->parents[x];
}

void link(graph_t* graph, int x, int y) {
    if (graph->ranks[x] > graph->ranks[y]) {
        graph->parents[y] = x;
    } else {
        graph->parents[x] = y;
        if (graph->ranks[x] == graph->ranks[y])
            graph->ranks[y]++;
    }
}

void node_union(graph_t* graph, int x, int y) {
    link(graph, find_set(graph, x), find_set(graph, y));
}

int pow10(int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++)
        result *= 10;
    return result;
}

static inline int nth_digit(size_t num, int n) {
    return (num / pow10(n)) % 10;
}

void counting_sort(graph_t* a, int n) {
    std::vector<int> c(10, 0);
    std::vector<edge_t*> b(a->size);
    
    for (int i = 0; i < a->size; i++)
        c[nth_digit(a->data[i]->weight, n)]++;
    
    for (int i = 1; i < 10; i++)
        c[i] += c[i-1];

    for (int i = a->size - 1; i > -1; i--) {
        b[c[nth_digit(a->data[i]->weight, n)]-1] = a->data[i];
        c[nth_digit(a->data[i]->weight, n)]--;
    }
    
    for (int i = 0; i < a->size; i++)
        a->data[i] = b[i];
}

void radix_sort(graph_t* graph, int d_count) {
    for (int i = 0; i < d_count; i++)
        counting_sort(graph, i);
}

size_t get_maximum_cost_spanning_tree(graph_t* graph, int d_count) {
    size_t result = 0;
    
    radix_sort(graph, d_count);

    for (int i = graph->size - 1; i > -1; i--) {   
        edge_t* edge = graph->data[i];
        if (find_set(graph, edge->u) != find_set(graph, edge->v)) {
            node_union(graph, edge->u, edge->v);
            result += edge->weight;
        }
        delete edge;
    }

    return result;
}

void read_input(graph_t *graph, size_t& max) {
    int v_count, e_count;
    std::cin >> v_count >> e_count;

    graph->size = 0;
    graph->parents = std::vector<int>(v_count);
    graph->ranks = std::vector<int>(v_count, 0);

    int id1, id2;
    size_t weight, min;
    for (int i = 0; i < e_count; i++) {
        std::cin >> id1 >> id2 >> weight;
        
        if (i == 0)
            min = weight;

        if (i < v_count || weight > min) {
            make_set(graph, --id1);
            make_set(graph, --id2);
            
            edge_t* new_edge =  new edge_t({.u = id1, .v = id2, .weight = weight});
            graph->data.push_back(new_edge);
            graph->size++;
            
            if (weight < min)
                min = weight;

            if (weight > max)
                max = weight;
        }
    }
}

int main(void) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    size_t max = 0;
    graph_t* graph = new graph_t();
    read_input(graph, max);
    
    std::cout << get_maximum_cost_spanning_tree(graph, floor(log10(max)) + 1) << std::endl;
    
    delete graph;
    
    return 0;
}