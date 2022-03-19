#include<stdio.h>

/*定义栈结构*/
typedef struct TRANSITIONS
{
    int index_status_from; 

    int index_status_to;
    
    char trigger[2];

    char target_trigger_stack1[20];
    int action_trigger_stack1;//-1,0,1

    char target_trigger_stack2[20];
    int action_trigger_stack2;//-1,0,1

}TRANSITION;

typedef struct STATUS
{
    char name[10];
    int index;
    int address_in_vm;
}STATUS;

