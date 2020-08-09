#include<stdio.h>
#include "poly_header.h"
#include "header.h"


void main(){
    OneBin_init(sizeof(digit));
    digit *a_head = NULL, *b_head  = NULL, *num;
    unsigned int dig;
    char ch=',', op = ' ';
    while(op != '='){
        num = NULL;
        ch=',';
        while(ch==','){
            scanf("%d", &dig);
            num = add_dig(num, dig%BASE);
            ch=' ';
            while(ch==' ')
            scanf("%c", &ch);
            if(ch=='$')
                break;
        }
        if(num == NULL)
            continue;
        else if(a_head == NULL)
            a_head = num;
        else{
            b_head = num;
            a_head = op_num(a_head, b_head, op);
            op=' ';
        }
        while(op == ' ')
        scanf("%c", &op);
    }
    print_num(a_head);
    get_Results();
}
