#include <iostream>
#include <vector>
#include <cmath>

struct tree_node_t {
    tree_node_t* p;
    size_t rank;
};

struct edge_t {
    tree_node_t* u;
    tree_node_t* v;
    size_t weight;
};

struct vector_t {
    int size;
    std::vector<edge_t*> data;
};

void make_set(tree_node_t* x) {
    x->p = x;
    x->rank = 0;
}

tree_node_t* find_set(tree_node_t* x) {
    if (x != x->p)
        x->p = find_set(x->p);
    return x->p;
}

void link(tree_node_t* x, tree_node_t* y) {
    if (x->rank > y->rank) {
        y->p = x;
    } else {
        x->p = y;
        if (x->rank == y->rank)
            y->rank++;
    }
}

void node_union(tree_node_t* x, tree_node_t* y) {
    link(find_set(x), find_set(y));
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

void counting_sort(vector_t* a, int n) {
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

void radix_sort(vector_t* vector, int d_count) {
    for (int i = 0; i < d_count; i++)
        counting_sort(vector, i);
}

size_t get_maximum_cost_spanning_tree(vector_t* vector, int d_count) {
    size_t result = 0;
    
    radix_sort(vector, d_count);

    for (int i = vector->size - 1; i > -1; i--) {   
        edge_t* edge = vector->data[i];
        if (find_set(edge->u) != find_set(edge->v)) {
            node_union(edge->u, edge->v);
            result += edge->weight;
        }
        delete edge;
    }

    return result;
}

void read_input (vector_t *vector, std::vector<tree_node_t*> &nodes, size_t& max) {
    int v_count, e_count;
    std::cin >> v_count >> e_count;

    vector->size = 0;
    nodes = std::vector<tree_node_t*>(v_count, nullptr);

    int id1, id2;
    size_t weight, min;
    for (int i = 0; i < e_count; i++) {
        std::cin >> id1 >> id2 >> weight;
        
        if (i == 0)
            min = weight;

        if (i < v_count || weight > min) {
            if (nodes[--id1] == nullptr) {
                nodes[id1] = new tree_node_t();
                make_set(nodes[id1]);
            }
            if (nodes[--id2] == nullptr) {
                nodes[id2] = new tree_node_t();
                make_set(nodes[id2]);
            }
            
            edge_t* new_edge =  new edge_t({.u = nodes[id1], .v = nodes[id2], .weight = weight});
            vector->data.push_back(new_edge);
            vector->size++;
            
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
    vector_t* vector = new vector_t();
    std::vector<tree_node_t*> nodes;
    read_input(vector, nodes, max);
    
    std::cout << get_maximum_cost_spanning_tree(vector, floor(log10(max)) + 1) << std::endl;
    
    for (auto & node : nodes)
        if (node != nullptr)
            delete node;

    delete vector;
    
    return 0;
}