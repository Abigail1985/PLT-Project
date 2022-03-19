/*
 * @Author: Abigail 
 * @Date: 2021-12-05 22:32:42 
 * @Last Modified by: Abigail
 * @Last Modified time: 2022-01-06 16:40:22
 */


#include "compile_automate.h"
#include "mystack.h"


myStack * stack1;
myStack * stack2;

int flag_exist=0;//用来检测一个status有没有指定的trigger

int execute()
{

    stack1=(struct myStack*)malloc(sizeof(struct myStack));
    stack2=(struct myStack*)malloc(sizeof(struct myStack));

    stack1->topOfStack=TOP_OF_STACK;
    stack2->topOfStack=TOP_OF_STACK;

    int index_status_now=index_status_initial;
    int address_status_now=search_address_given_index(index_status_initial);

    int gap;
    switch(numStacks)
    {
        case 0:
            gap=2;
            break;
        case 1:
            gap=4;
            break;
        case 2:
            gap=6;
            break;
    }

    getchar();//消除掉换行符
    char c;
    printf("Enter the string for the VM: ");
    c=getchar();
    printf("\n");

    
    //打印初始状态
    printf("  -> Status: %s\t",search_name_given_index(index_status_now));
    if (numStacks>=1) {
        printf("Stack1: Empty\t");
    }
    if (numStacks==2) {
        printf("Stack2: Empty\t");
    }
    printf("\n\n");


    int numTransFromNow;
    while(c!='\n')
    {
        numTransFromNow=vminfo[address_status_now]; 
        for(int i=0;i<numTransFromNow;i++)
        {
            if(vminfo[address_status_now+i*gap+1] == (int)c)//vminfo这个位置的数据都表示trigger，这是在对比trigger
            {
                printf("%c -> Status: %s\t",c,search_name_given_address(vminfo[address_status_now+i*gap+2]));   

                //获取对于stack的操作信息
                switch(numStacks)
                {
                    case 1:
                        if(vminfo[address_status_now+i*gap+4]==1)
                        {
                            if(stack_push(stack1,vminfo[address_status_now+i*gap+3])!=SUCCESS) 
                             {printf("\nError : Stack 1 full !\n");return 1;}
                        }
                        if(vminfo[address_status_now+i*gap+4]==-1)
                        {
                            if(stack_pop(stack1)!=SUCCESS) 
                              {printf("\nError : Stack 1 empty !\n");return 1;}
                        }

                        printf("Stack1: ");
                        print_stack(stack1);
                        printf("\n");

                    break;

                    case 2:
                        if(vminfo[address_status_now+i*gap+4]==1)
                        {
                            if(stack_push(stack1,vminfo[address_status_now+i*gap+3])!=SUCCESS) 
                             {printf("\nError : Stack 1 full !\n");return 1;}
                        }
                        if(vminfo[address_status_now+i*gap+4]==-1)
                        {
                            if(stack_pop(stack1)!=SUCCESS) 
                             {printf("\nError : Stack 1 empty !\n");return 1;}
                        }
                        printf("Stack1: ");
                        print_stack(stack1);


                        if(vminfo[address_status_now+i*gap+6]==1)
                        {
                            if(stack_push(stack2,vminfo[address_status_now+i*gap+5])!=SUCCESS) 
                             {printf("\nError : Stack 2 full !\n");return 1;}
                        }
                        if(vminfo[address_status_now+i*gap+6]==-1)
                        {
                            if(stack_pop(stack2)!=SUCCESS) 
                            {printf("\nError : Stack 2 empty !\n");return 1;}
                        }
                        printf("Stack2: ");
                        print_stack(stack2);
                        printf("\n");                        

                    break;
                }
                printf("\n");
                address_status_now=vminfo[address_status_now+i*gap+2];//这个位置的数据都是要转移到那个状态的address         
                flag_exist=1;
                break;
            }
        }
        if(flag_exist==0){
            printf("\nThe string is not recognized!\nThe string is refused!\n");
            return 1;
        }
        flag_exist=0;
        c=getchar();
    }

    // printf("address now:%d \n",address_status_now);
    // printf("index now:%d \n",search_index_given_address(address_status_now));
    // printf("name now:%s \n",search_name_given_address(address_status_now));

    int b=0; //pour justifier qu'on arrive a l'etat final
    for (int i=0; i<numStatusFinal; i++){
        if (address_status_now==vminfo[3+i]){
            b=1;
            break;
        }
    }
    if (b==0){
        printf("Error: Fail to arrive at these status finals!\nThe string is refused!\n");
        return 1;
    }
    if (stack1->topOfStack!=-1 || stack2->topOfStack!=-1){ //pour justifier les Stacks sont emptys
        printf("Error: The stack is not empty in the end!\nThe string is refused!\n");
        return 1 ;
    }
    printf("The string is accepted!\n");
    return 0;
}


int execute_without_debug()
{

    stack1=(struct myStack*)malloc(sizeof(struct myStack));
    stack2=(struct myStack*)malloc(sizeof(struct myStack));

    stack1->topOfStack=TOP_OF_STACK;
    stack2->topOfStack=TOP_OF_STACK;

    // int index_status_now=index_status_initial;
    int address_status_now=search_address_given_index(index_status_initial);

    int gap;
    switch(numStacks)
    {
        case 0:
            gap=2;
            break;
        case 1:
            gap=4;
            break;
        case 2:
            gap=6;
            break;
    }

    getchar();//消除掉换行符
    char c;
    printf("Enter the string for the VM: ");
    c=getchar();


    int numTransFromNow;
    while(c!='\n')
    {
        numTransFromNow=vminfo[address_status_now]; 
        for(int i=0;i<numTransFromNow;i++)
        {
            if(vminfo[address_status_now+i*gap+1] == (int)c)//vminfo这个位置的数据都表示trigger，这是在对比trigger
            { 
                //获取对于stack的操作信息
                switch(numStacks)
                {
                    case 1:
                        if(vminfo[address_status_now+i*gap+4]==1)
                        {
                            if(stack_push(stack1,vminfo[address_status_now+i*gap+3])!=SUCCESS) 
                             {    printf("The string is refused!\n");return 1;}
                        }
                        if(vminfo[address_status_now+i*gap+4]==-1)
                        {
                            if(stack_pop(stack1)!=SUCCESS) 
                              {    printf("The string is refused!\n");return 1;}
                        }

                    break;

                    case 2:
                        if(vminfo[address_status_now+i*gap+4]==1)
                        {
                            if(stack_push(stack1,vminfo[address_status_now+i*gap+3])!=SUCCESS) 
                             {    printf("The string is refused!\n");return 1;}
                        }
                        if(vminfo[address_status_now+i*gap+4]==-1)
                        {
                            if(stack_pop(stack1)!=SUCCESS) 
                             {    printf("The string is refused!\n");return 1;}
                        }


                        if(vminfo[address_status_now+i*gap+6]==1)
                        {
                            if(stack_push(stack2,vminfo[address_status_now+i*gap+5])!=SUCCESS) 
                             {    printf("The string is refused!\n");return 1;}
                        }
                        if(vminfo[address_status_now+i*gap+6]==-1)
                        {
                            if(stack_pop(stack2)!=SUCCESS) 
                            {    printf("The string is refused!\n");return 1;}
                        }                       

                    break;      
                }
                address_status_now=vminfo[address_status_now+i*gap+2];//这个位置的数据都是要转移到那个状态的address           
                flag_exist=1;        
                break;  
            }
        }
        if(flag_exist==0){
            printf("\nThe string is refused!\n");
            return 1;
        }
        flag_exist=0;
        c=getchar();
    }

    // printf("address now:%d \n",address_status_now);
    // printf("index now:%d \n",search_index_given_address(address_status_now));
    // printf("name now:%s \n",search_name_given_address(address_status_now));

    int b=0; //pour justifier qu'on arrive a l'etat final
    for (int i=0; i<numStatusFinal; i++){
        if (address_status_now==vminfo[3+i]){
            b=1;
            break;
        }
    }
    if (b==0){
        printf("The string is refused!\n");
        return 1;
    }
    if (stack1->topOfStack!=-1 || stack2->topOfStack!=-1){ //pour justifier les Stacks sont emptys
        printf("The string is refused!\n");
        return 1 ;
    }
    printf("The string is accepted!\n");
    return 0;
}
