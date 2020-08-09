#include <stdio.h>
#include <stdlib.h>

#define SIZE 300

int SPheap_alloc_req, SPheap_dealloc_req, SPheap_splits, SPheap_coalesce_count, SPheap_alloc_total;

typedef struct SPheap_node{
    int KVAL, TYPE, TAG;
    void *add;
    struct SPheap_node *prev, *next;
}SPheap_node;

typedef struct SPheap_size{
    //int size;
    SPheap_node *head;
}SPheap_size;

void* SPhead_get_buddy(SPheap_node *x){
    void *b;
    int m = 1<<(x->KVAL+2), t=6;
    if((long)(x->add)%m == 0 && x->TYPE == 3){
        return x->add+3*(1<<(x->KVAL));
    }
    if((long)(x->add)%m == 3*(1<<(x->KVAL)) && x->TYPE == 3){
        return x->add-3*(1<<(x->KVAL));
    }
    if(x->TYPE == 1)
        return x->add+(1<<(x->KVAL));
    if(x->TYPE == 2)
        return x->add-(1<<(x->KVAL));
    printf("Error finding buddy\n");
    return NULL;
}

SPheap_size ASL[SIZE];

void SPheap_init(){
    int i;
    for(i=0;i<SIZE;i++){
        ASL[i].head = NULL;
    }
    SPheap_node *new_node = (SPheap_node*)malloc(sizeof(SPheap_node));
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->add = (void*)malloc(64 * sizeof(int)); //  assuming int as 4 Bytes
    new_node->KVAL = 28;    // 2^28 bytes
    new_node->TAG = 0;
    new_node->TYPE = 3;
    ASL[256].head = new_node;
}

void print_ASL(){
    int i;
    for(i=1;i<SIZE;i++){
        if(ASL[i].head != NULL){
            SPheap_node *mem_node = ASL[i].head;
            while(mem_node != NULL){
                printf("i: %d tag: %d add: %p\n", i, mem_node->TAG, mem_node->add);
                mem_node = mem_node->next;
            }
        }
    }
}

void SPheap_remove_node(int block_size, SPheap_node *mem_node){
    SPheap_node *tmp = ASL[block_size].head, *tmp2;
    while(tmp != NULL){
        if(tmp->add == mem_node->add){
            tmp2 = tmp->prev;
            if(tmp2 != NULL){
                tmp2->next = tmp->next;
            }
            else{
                ASL[block_size].head = tmp->next;
            }
            if(tmp->next != NULL){
                tmp->next->prev = tmp2;
            }
            free(tmp);
            return ;
        }
    }
}

int is_2power(int val){
    int i=1;
    while(i<val){
        i*=2;
    }
    if(i==val)
        return 1;
    return 0;
}

void* SPheap_alloc_shuffle(int block_size, SPheap_node *mem_node, int alloc_size) {
    //printf("block_size: %d, mem_add: %p\n", block_size, mem_node->add);
    int tmp = block_size / 4;
    if (block_size == alloc_size || (is_2power(block_size) && alloc_size > block_size - tmp) ||
        (alloc_size > 2 * block_size / 3)) {  //  Block cannot be broken
        mem_node->TAG = 1;      //  Memory allocated
        return mem_node->add;
    }   //  Block can be divided
    SPheap_splits++;
    if (is_2power(block_size)) {
        int big_size = block_size - tmp;
        SPheap_node *big_node = (SPheap_node *) malloc(sizeof(SPheap_node));
        big_node->KVAL = mem_node->KVAL - 2;
        big_node->TAG = 0;
        big_node->TYPE = 3;     //  Left split is bigger block
        big_node->next = ASL[big_size].head;
        if(ASL[big_size].head != NULL){
            ASL[big_size].head->prev = big_node;
        }
        big_node->add = mem_node->add;
        ASL[big_size].head = big_node;

        SPheap_node *small_node = (SPheap_node *) malloc(sizeof(SPheap_node));
        small_node->KVAL = mem_node->KVAL - 2;
        small_node->TAG = 0;
        small_node->TYPE = 3;     //  Right split is smaller block
        small_node->next = ASL[tmp].head;
        if(ASL[tmp].head != NULL){
            ASL[tmp].head->prev = small_node;
        }
        small_node->add = mem_node->add + big_size;
        ASL[tmp].head = small_node;

        SPheap_remove_node(block_size, mem_node);
        if(alloc_size <= tmp){
            return SPheap_alloc_shuffle(tmp, small_node, alloc_size);
        } else{
            return SPheap_alloc_shuffle(big_size, big_node, alloc_size);
        }
    } else if (is_2power(block_size / 3)) {
        tmp = block_size / 3;
        int big_size = block_size - tmp;
        SPheap_node *big_node = (SPheap_node *) malloc(sizeof(SPheap_node));
        big_node->KVAL = mem_node->KVAL + 1;
        big_node->TAG = 0;
        big_node->TYPE = 1;     //  Left split is bigger block
        big_node->next = ASL[big_size].head;
        if(ASL[big_size].head != NULL){
            ASL[big_size].head->prev = big_node;
        }
        big_node->add = mem_node->add;
        ASL[big_size].head = big_node;

        SPheap_node *small_node = (SPheap_node *) malloc(sizeof(SPheap_node));
        small_node->KVAL = mem_node->KVAL;
        small_node->TAG = 0;
        small_node->TYPE = 2;     //  Right split is smaller block
        small_node->next = ASL[tmp].head;
        if(ASL[tmp].head != NULL){
            ASL[tmp].head->prev = small_node;
        }
        small_node->add = mem_node->add + big_size;
        ASL[tmp].head = small_node;

        SPheap_remove_node(block_size, mem_node);
        if(alloc_size <= tmp){
            return SPheap_alloc_shuffle(tmp, small_node, alloc_size);
        } else{
            return SPheap_alloc_shuffle(big_size, big_node, alloc_size);
        }
    }
}


void* SPheap_alloc(int size){
    int i;
    for(i=size;i<SIZE;i++){
        if(ASL[i].head != NULL){
            SPheap_node *mem_node = ASL[i].head;
            while(mem_node != NULL){
                if(mem_node->TAG == 0){
                    SPheap_alloc_req++;
                    SPheap_alloc_total += size;
                    return SPheap_alloc_shuffle(i, mem_node, size);
                }
                mem_node = mem_node->next;
            }
        }
    }
    if(size > 256){
        printf("Invalid Allocation\n");
        return NULL;
    }
    int i, sum=0;
    SPheap_node *tmp_node;
    for(i=1;i<SIZE;i++){
        if(ASL[i].head != NULL){
            tmp_node = ASL[i].head;
            while(tmp_node != NULL){
                sum += i;
                tmp_node = tmp_node->next;
            }
        }
    }
    double alloc_area = sum / 256 ;
    alloc_area *= 100;
    printf("External fragmentation\nAsize: %d\nAllocArea%: %lf\n", size, alloc_area);
    return NULL;
}

SPheap_node* SPhead_get_mem_node(void *add){

    return NULL;
}

/*void SPheap_coalesce(SPheap_node *mem_node, int mem_block_size){
    int i, buddy_block_size = -1;
    SPheap_node *buddy_node = SPhead_get_buddy(mem_node), *tmp_node;
    for(i=1;i<SIZE;i++){
        if(ASL[i].head != NULL){
            tmp_node = ASL[i].head;
            while(tmp_node != NULL){
                if(tmp_node == buddy_node){
                    buddy_block_size = i;
                    break;
                }
                tmp_node = tmp_node->next;
            }
            if(buddy_block_size != -1)
                break;
        }
    }
    if(buddy_node->TAG == 0){
        SPheap_node *merge_node = (SPheap_node *) malloc(sizeof(SPheap_node));
        merge_node->KVAL = mem_node->KVAL;
        merge_node->TAG = 0;
        merge_node->next = ASL[tmp].head;
        if(ASL[tmp].head != NULL) {
            ASL[tmp].head->prev = merge_node;
        }
        ASL[tmp].head = merge_node;
        if(mem_block_size > buddy_block_size){
            merge_node->add = mem_node->add;
            if(mem_block_size == 2*buddy_block_size)
                merge_node->TYPE = 3;
            else
                merge_node->TYPE = 3;
        }
        else{
            merge_node->add = buddy_node->add;
        }
    }
}*/

void SPheap_coalesce(SPheap_node *mem_node, int mem_block_size){
    //printf("block_size: %d, mem_add: %p\n", mem_block_size, mem_node->add);
    int i, buddy_block_size = -1;
    SPheap_node *buddy_node = NULL, *tmp_node;
    for(i=1;i<SIZE;i++){
        if(ASL[i].head != NULL){
            tmp_node = ASL[i].head;
            while(tmp_node != NULL){
                if(tmp_node->add == mem_node->add + mem_block_size){
                    buddy_node = tmp_node;
                    buddy_block_size = i;
                    break;
                }
                tmp_node = tmp_node->next;
            }
            if(buddy_block_size != -1)
                break;
        }
    }
    if(buddy_block_size == -1){
        for(i=1;i<SIZE;i++){
            if(ASL[i].head != NULL){
                tmp_node = ASL[i].head;
                while(tmp_node != NULL){
                    if(tmp_node->add + i == mem_node->add){
                        buddy_node = tmp_node;
                        buddy_block_size = i;
                        break;
                    }
                    tmp_node = tmp_node->next;
                }
                if(buddy_block_size != -1)
                    break;
            }
        }
    }
    //printf("buddy\nblock_size: %d, mem_add: %p\n", buddy_block_size, buddy_node->add);
    int merge_block_size = mem_block_size + buddy_block_size;
    if(buddy_node->TAG == 0){
        SPheap_coalesce_count++;
        SPheap_node *merge_node = (SPheap_node *) malloc(sizeof(SPheap_node));
        merge_node->KVAL = mem_node->KVAL;
        merge_node->TAG = 0;
        merge_node->next = ASL[merge_block_size].head;
        if(ASL[merge_block_size].head != NULL) {
            ASL[merge_block_size].head->prev = merge_node;
        }
        ASL[merge_block_size].head = merge_node;
        if(mem_block_size > buddy_block_size){
            merge_node->add = mem_node->add;
            merge_node->TYPE = 3;
        }
        else{
            merge_node->add = buddy_node->add;
        }
        SPheap_remove_node(mem_block_size, mem_node);
        SPheap_remove_node(buddy_block_size, buddy_node);
        print_ASL();
        if(merge_block_size != 256)
            SPheap_coalesce(merge_node, merge_block_size);
    }
}

void SPhead_free(void *add){
    int i, mem_block_size= -1;
    SPheap_node *mem_node = NULL, *tmp_node;
    for(i=1;i<SIZE;i++){
        if(ASL[i].head != NULL){
            tmp_node = ASL[i].head;
            while(tmp_node != NULL){
                if(tmp_node->add == add){
                    mem_node = tmp_node;
                    mem_block_size = i;
                }
                tmp_node = tmp_node->next;
            }
        }
    }
    if(mem_node == NULL){
        printf("Invalid Address\n");
        return ;
    }
    mem_node->TAG = 0;
    SPheap_dealloc_req++;
    SPheap_coalesce(mem_node, mem_block_size);
}

void SPheap_print_diagnostics(){
    printf("Numbers of allocation requests: %d\nDeallocation requests: %d\nSPheap area splits: %d\nBuddy recombinations: %d\n", SPheap_alloc_req, SPheap_dealloc_req, SPheap_splits, SPheap_coalesce_count);
}

void SPheap_int_frag(){
    int i, sum=0;
    SPheap_node *tmp_node;
    for(i=1;i<SIZE;i++){
        if(ASL[i].head != NULL){
            tmp_node = ASL[i].head;
            while(tmp_node != NULL){
                sum += i;
                tmp_node = tmp_node->next;
            }
        }
    }
    double int_frag = sum - SPheap_alloc_total;
    int_frag /= SPheap_alloc_total;
    printf("Internal fragmentation: %lf\n", int_frag);
}

int main(){
    int i,p;
    SPheap_init();
    printf("start\n%p\n", ASL[256].head->add);
    for(i=1;i<=1;i++){
        scanf("%d", &p);
        SPheap_alloc(p);
        print_ASL();
    }
    for(i=1;i<=1;i++){
        SPhead_free(ASL[1].head->add);
        print_ASL();
    }
}

