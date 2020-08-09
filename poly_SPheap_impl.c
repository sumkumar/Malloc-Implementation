#include<stdio.h>
#include<stdlib.h>
#include "poly_header.h"
#include "header.h"

unsigned int BASE = 10000;

void print_digit(digit* dig){
    if(dig->next == NULL){
        printf("%04d", dig->val);
        return ;
    }
    print_digit(dig->next);
    printf(",%04d", dig->val);
}

void print_num(digit* num){
    print_digit(num);
    printf("$\n");
}

digit* add_dig(digit* head, unsigned int val){
    digit* new_dig = (digit*) SPheap_alloc(sizeof(digit));
    new_dig->val = val;
    new_dig->next = head;
    return new_dig;
}

void add_num(digit* a_num, digit* b_num, unsigned int carry){
    unsigned int val;
    digit* b_next =NULL;
    if(b_num == NULL && carry ==0)
        return ;
    if(a_num->next == NULL){
        if(b_num!=NULL){
            a_num->next = b_num->next;
            b_num->next = NULL;
        }
        else if(carry != 0 && (a_num->val+carry) >= BASE){
            digit* new_dig = (digit*) SPheap_alloc(sizeof(digit));
            new_dig->val = val/BASE;
            new_dig->next = NULL;
            a_num->next = new_dig;
            a_num->val = (a_num->val+carry) % BASE;
            return ;
        }
    }
    val = a_num->val;
    if(b_num != NULL)
        val+=b_num->val;
    carry = val/BASE;
    val%=BASE;
    a_num->val = val;
    if(b_num!=NULL){
        b_next = b_num->next;
    }
    add_num(a_num->next, b_next, carry);
}

digit* get_dig_mul(digit* a_num, unsigned int b_dig){
    digit* add_step_num = NULL, *p=NULL;
    unsigned int carry=0, val;
    while(a_num!=NULL || carry!=0){
        val=0;
        if(a_num!=NULL){
            val = a_num->val*b_dig;
            a_num=a_num->next;
        }
        digit* new_dig = (digit*) SPheap_alloc(sizeof(digit));
        new_dig->val = (val%BASE + carry)%BASE;
        carry=(val+carry)/BASE;
        new_dig->next = NULL;
        if(p!=NULL)
        p->next = new_dig;
        p=new_dig;
        if(add_step_num == NULL)
            add_step_num = new_dig;
    }
    return add_step_num;
}

digit* mul_num(digit* a_num, digit* b_num){
    digit* result = NULL, *add_step_num, *p=NULL;
    while(b_num!=NULL){
        add_step_num = get_dig_mul(a_num, b_num->val);
        if(result == NULL){
            result = add_step_num;
            p = add_step_num;
        }
        else
        add_num(p, add_step_num, 0);
        p=p->next;
        b_num=b_num->next;
    }
    return result;
}

digit* op_num(digit* a_num, digit* b_num, char op){
    if(op == '+'){
        add_num(a_num, b_num, 0);
    }
    else if(op == '*'){
        return mul_num(a_num, b_num);
    }
    else{
        printf("Please enter valid input\n");
        exit(-1);
    }
    return a_num;
}

digit* get_number_input(){
    unsigned int dig;
    digit *num = NULL;
    printf("Enter your number: ");
    char ch=',';
    while(ch==','){
        scanf("%d", &dig);
        num = add_dig(num, dig%BASE);
        ch=' ';
        while(ch==' ')
        scanf("%c", &ch);
        if(ch=='$')
            break;
    }
    return num;
}

void get_Results(){
    SPheap_print_diagnostics();
    SPheap_int_frag();
}
