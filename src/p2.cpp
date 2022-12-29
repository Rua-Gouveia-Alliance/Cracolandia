#include <iostream>
#include <vector>

struct tree_node_t {
    tree_node_t* p;
    size_t rank;
    size_t id;
};

struct edge_t {
    tree_node_t* u;
    tree_node_t* v;
    size_t weight;
};

struct heap_t {
    int size;
    std::vector<edge_t*> edges;
};

static inline int parent(int i) {
    return (i >> 1);
}

static inline int left(int i) {
    return (i << 1);
}

static inline int right(int i) {
    return (i << 1) + 1;
}

void swap(heap_t* heap, int i, int j) {
    edge_t* aux = heap->edges[i];
    heap->edges[i] = heap->edges[j];
    heap->edges[j] = aux;
}

void max_heapify(heap_t* heap, int i) {
    int l = left(i), r = right(i), largest = i;
    
    if (l < heap->size + 1 && heap->edges[l]->weight > heap->edges[i]->weight)
        largest = l;
    
    if (r < heap->size + 1 && heap->edges[r]->weight > heap->edges[largest]->weight)
        largest = r;
    
    if (largest != i) {
        swap(heap, i, largest);
        max_heapify(heap, largest);
    }
}

void heap_increase_key(heap_t *heap, int i, size_t key) {
    if (heap->edges[i]->weight == key)
        return;

    heap->edges[i]->weight = key;

    while (i > 1 && heap->edges[parent(i)]->weight < heap->edges[i]->weight) {
        swap(heap, i, parent(i));
        i = parent(i);
    }
}

void max_heap_insert(heap_t* heap, edge_t* edge) {
    heap->size++;
    size_t key = edge->weight;
    edge->weight = 0;
    heap->edges.push_back(edge);
    heap_increase_key(heap, heap->size, key);
}

edge_t* extract_max(heap_t* heap) {
    edge_t* max = heap->edges[1];
    heap->edges[1] = heap->edges[heap->size];
    heap->size--;
    max_heapify(heap, 1);
    return max;
}

void build_max_heap(heap_t* heap) {
    int size = heap->size >> 1;
    for (int i = size; i > 0; i--)
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

unsigned long int get_maximum_cost_spanning_tree(heap_t* heap) {
    unsigned long int result = 0;

    while (heap->size > 0) {   
        edge_t* edge = extract_max(heap);
        if (find_set(edge->u) != find_set(edge->v)) {
            node_union(edge->u, edge->v);
            result += edge->weight;
        }
        delete edge;
    }

    return result;
}

void read_input (heap_t *heap, std::vector<tree_node_t*> &nodes) {
    int v_count, e_count;

    scanf("%d", &v_count);
    scanf("%d", &e_count);


    for (int i = 0; i < v_count; i++) {
        nodes.push_back(new tree_node_t());
        nodes[i]->id = i;
        make_set(nodes[i]);
    }

    int id1, id2;
    size_t weight;
    heap->edges.push_back(nullptr);
    for (int i = 0; i < e_count; i++) {
        scanf("%d %d %ld", &id1, &id2, &weight);
        edge_t* new_edge =  new edge_t({.u = nodes[id1-1], .v = nodes[id2-1], .weight = weight});
        max_heap_insert(heap, new_edge);
    }

    heap->size = e_count;
}

int main(void) {
    heap_t* heap = new heap_t();
    std::vector<tree_node_t*> nodes;
    read_input(heap, nodes);
    
    std::cout << get_maximum_cost_spanning_tree(heap) << std::endl;
    
    for (auto & node : nodes)
        delete node;

    delete heap;
    
    return 0;
}