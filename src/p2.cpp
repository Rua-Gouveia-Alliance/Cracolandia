#include <iostream>
#include <vector>

#define FINISHED 0
#define VISITING 1
#define NOTVISITED 2

#define LOG(x) std::cout << x << std::endl

struct tree_node_t {
    tree_node_t* p;
    size_t rank;
    int id;
};

struct edge_t {
    tree_node_t* u;
    tree_node_t* v;
    size_t weight;
};

struct heap_t {
    int size;
    std::vector<edge_t> edges;
};

static inline int parent(int i) {
    return i >> 1;
}

static inline int left(int i) {
    return i << 1;
}

static inline int right(int i) {
    return (i << 1) + 1;
}

void swap(heap_t* heap, int i, int j) {
    edge_t aux = heap->edges[i];
    heap->edges[i] = heap->edges[j];
    heap->edges[j] = aux;
}

void max_heapify(heap_t* heap, int i) {
    int l = left(i), r = right(i), largest = i;
    
    if (l < heap->size && heap->edges[l].weight > heap->edges[i].weight)
        largest = l;
    
    if (r < heap->size && heap->edges[r].weight > heap->edges[largest].weight)
        largest = r;
    
    if (largest != i) {
        swap(heap, i, largest);
        max_heapify(heap, largest);
    }
}

void build_max_heap(heap_t* heap) {
    size_t size = heap->size >> 1;
    for (size_t i = size; i > 0; i--)
        max_heapify(heap, i);
}

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

unsigned long int get_maximum_cost_spanning_tree(heap_t* heap, std::vector<tree_node_t*>& nodes) {
    unsigned long int result = 0;
    
    for (auto & node : nodes)
        make_set(node);

    for (int i = 0; i < heap->size; i++) {
        if (find_set(heap->edges[i].u) != find_set(heap->edges[i].v)) {
            node_union(heap->edges[i].u, heap->edges[i].v);
            result += heap->edges[i].weight;
        }
    }

    return result;
}

unsigned long int compute_graph(heap_t* heap, std::vector<tree_node_t*> &nodes) {
    unsigned long int result = 0;
    result += get_maximum_cost_spanning_tree(heap, nodes);
    return result;
}


int main(void) {
    int v_count, e_count;
    std::cin >> v_count >> e_count;
    
    heap_t* heap = new heap_t();
    std::vector<tree_node_t*> nodes = std::vector<tree_node_t*>(v_count, nullptr);;
    for (int i = 0; i < e_count; i++) {
        int id1, id2;
        size_t weight;
        scanf("%d %d %ld", &id1, &id2, &weight);
        
        if (nodes[--id1] == nullptr)
            nodes[id1] = new tree_node_t({.id = id1});

        if (nodes[--id2] == nullptr)
            nodes[id2] = new tree_node_t({.id = id2});

        heap->edges.push_back(edge_t({.u = nodes[id1], .v = nodes[id2], .weight = weight}));
    }

    heap->size = e_count;
    build_max_heap(heap);

    std::cout << compute_graph(heap, nodes) << std::endl;
    
    for (auto & node : nodes)
        delete node;

    delete heap;
}