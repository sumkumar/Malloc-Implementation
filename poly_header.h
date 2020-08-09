typedef struct digit{
    unsigned int val;
    struct digit* next;
}digit;

digit* add_dig(digit* head, unsigned int val);

digit* op_num(digit* a_num, digit* b_num, char op);

unsigned int BASE;

void print_num(digit* num);

digit* get_number_input();

void get_Results();
