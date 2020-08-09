#include <stdio.h>
#include "header.h"

void main(){
    int i,j,p,q,size;
    printf("1. SPheap\n2. OneBin\nPlease Enter your choice: ");
    scanf("%d", &p);
    void *add;
    if(p==1){
        SPheap_init();
        while(1){
            printf("1. Alloc\n2. Dealloc\n3. Exit\nPlease Enter your choice: ");
            scanf("%d",&q);
            if(q==1){
                printf("Enter Allocation Size: ");
                scanf("%d",&size);
                add = SPheap_alloc(size);
                if(add != NULL)
                    printf("Address: %p", add);
            }
            else if(q==2){
                printf("Enter Address: ");
                scanf("%p",&add);
                SPhead_free(add);
            }
            else if(q==3)
                break;
        }
    }
    if(p==2){
        printf("Enter Bin Size: ");
        scanf("%d", &q);
        OneBin_init(q);
        while(1){
            printf("1. Alloc\n2. Dealloc\n3. Exit\nPlease Enter your choice: ");
            scanf("%d",&q);
            if(q==1){
                add = OneBin_alloc();
                if(add != NULL)
                    printf("Address: %p", add);
            }
            else if(q==2){
                printf("Enter Address: ");
                scanf("%p",&add);
                OneBin_free(add);
            }
            else if(q==3)
                break;
            else
                printf("Invalid Option\n");
        }
    }
}
