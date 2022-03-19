#include<stdio.h>

#define STACK_SIZE 64 /*栈大小*/
#define TOP_OF_STACK -1 /*栈顶位置*/
typedef int ElementType; /*栈元素类型*/

#define SUCCESS 0
#define FAILURE -1

/*定义栈结构*/
typedef struct myStack
{
    int topOfStack; /*记录栈顶位置*/
    ElementType stack[STACK_SIZE]; /*栈数组，也可以使用动态数组实现*/
}myStack;


/*函数声明*/
int stack_push(myStack *s,ElementType value);
int stack_pop(myStack *s);
int stack_top(myStack *s,ElementType *value);
int stack_is_full(myStack *s);
int stack_is_empty(myStack *s);


/*入栈，0表示成，非0表示出错*/
int stack_push(myStack *s,ElementType value)
{
    // printf("\nnow in stack_push\n");
    if(stack_is_full(s))
        return FAILURE;
    /*先增加topOfStack，再赋值*/
    s->topOfStack++;
    s->stack[s->topOfStack] = value;
    return SUCCESS;
}

/*出栈*/
int stack_pop(myStack *s)
{
    // printf("\nnow in stack_pop\n");
    /*首先判断栈是否为空*/
    if(stack_is_empty(s))
        return FAILURE;
    s->topOfStack--;
    return SUCCESS;
}

/*访问栈顶元素*/
int stack_top(myStack *s,ElementType *value)
{
    /*首先判断栈是否为空*/
    if(stack_is_empty(s))
        return FAILURE;
    *value = s->stack[s->topOfStack];
    return SUCCESS;
}

/*判断栈是否已满，满返回1，未满返回0*/
int stack_is_full(myStack *s)
{
    return s->topOfStack == STACK_SIZE - 1;
}

/*判断栈是否为空，空返回1，非空返回0*/
int stack_is_empty(myStack *s)
{
    return s->topOfStack ==  - 1;
}

void print_stack(myStack *s)
{
    char charValue[2];
    if(stack_is_empty(s)==1)
        printf("Empty");
    else
    {
        for(int i=0;i<=s->topOfStack;i++)
        {
            sprintf(charValue,"%c",s->stack[i]);//将int转换为字符串
            printf("%s",charValue);
        }
    }
    printf("\t");
    return;
}