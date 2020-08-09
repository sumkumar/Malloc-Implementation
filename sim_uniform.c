#include <stdio.h>
#include "header.h"
#include <stdlib.h>
#include <time.h>

#define IT 100

typedef struct node{
    void *arr;
    int time, size;
}node;

node mem_arr[1005];

void main(){
    srand(time(0));
    int i, j, time, size, sum=0, k=0;
    int arr[1005];
    SPheap_init();
    for(i=1;i<=IT;i++){
        for(j=1;j<=1000;j++){
            if(mem_arr[j].time == i){
                SPhead_free(mem_arr[j].arr);
                sum -= mem_arr[j].size;
            }
        }
        size = rand()%(256 - sum + 1);
        time = i+rand()%(IT - i + 1);
        sum += size;
        printf("i: %d size: %d time: %d\n", i, size, time);
        mem_arr[k].arr = SPheap_alloc(size);
        mem_arr[k].time = time;
        mem_arr[k].size = size;
        k++;
    }
    SPheap_int_frag();
}
