typedef struct SPheap_node{
    int KVAL, TYPE, TAG;
    void *add;
    struct SPheap_node *prev, *next;
}SPheap_node;

typedef struct SPheap_size{
    //int size;
    SPheap_node *head;
}SPheap_size;

typedef struct OneBin_node{
    void *add;
    int TAG;
    struct OneBin_node *next, *prev;
}OneBin_node;

typedef struct OneBin{
    OneBin_node *head;
}OneBin;

void SPheap_init();

void* SPheap_alloc(int size);

void SPhead_free(void *add);

void SPheap_print_diagnostics();

void SPheap_int_frag();

void OneBin_init(int size);

void* OneBin_alloc();

void OneBin_free(void* add);

void OneBin_int_frag();
