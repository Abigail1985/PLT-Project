/*
 * @Author: Abigail 
 * @Date: 2021-12-30 22:25:58 
 * @Last Modified by: Abigail
 * @Last Modified time: 2022-01-01 00:57:16
 */

#include "analyse_semantic.h"

/*
在0号地址，指出了堆栈的数量，
在1号地址，指出了初始状态的地址，
在2号地址，指出了接受状态的数量，
在接下来的方框中，接受状态的地址为VM
*/

/*Zpile
n0为从状态开始的转换次数，
c为触发转换的字符，
k为地址。
*/

int vminfo[400];
int flag;

int search_address_given_index(int index_status)
{
    for(int i =0;i<numStatus;i++)
    {
        if(statusList[i].index==index_status)
            return statusList[i].address_in_vm;
    }
    return 1024;//得到这个值说明出现了错误
}

int search_index_given_address(int address)
{
    for(int i =0;i<numStatus;i++)
    {
        if(statusList[i].address_in_vm==address)
            return statusList[i].index;
    }
    return 1024;//得到这个值说明出现了错误
}

char* search_name_given_index(int index_status)
{
    for(int i =0;i<numStatus;i++)
    {
        if(statusList[i].index==index_status)
            return statusList[i].name;
    }
    return "false";//得到这个值说明出现了错误
}

char* search_name_given_address(int address)
{
    for(int i =0;i<numStatus;i++)
    {
        if(statusList[i].address_in_vm==address)
            return statusList[i].name;
    }
    return "false";//得到这个值说明出现了错误
}


//这个函数，把一个transion的信息都按照规定的格式存蠢到一个数组里，但写入的地址是index不是address_in_index
void transition_info_from(int index)//flag是当前第一个空位置的索引
{
    //从当前状态出发，有几条转移方向，flag=3，
    int sum=0;
    for(int i=0;i<numTransitions;i++)
        if(transList[i].index_status_from==index) sum++;
    vminfo[flag]=sum; statusList[index].address_in_vm=flag; flag++; //先把这状态的地址记下来再更新flag

    //把转移的trigger写下来
    for(int i=0;i<numTransitions;i++)
        if(transList[i].index_status_from==index)
        {
            vminfo[flag]=(int)transList[i].trigger[0]; flag++;//因为之前trigger被定义成了char trigger[2] //c
            vminfo[flag]=transList[i].index_status_to; flag++;//k
            switch(numStacks)
            {
                case(0):
                break;

                case(1):
                if(transList[i].action_trigger_stack1!=0)//只有当要进行操作时才录入要操作的对象
                    {vminfo[flag]=(int)transList[i].target_trigger_stack1[0]; flag++;}//这个也被定义为char数组而不是char了 //p
                else
                    {vminfo[flag]=0; flag++;}//如果没有操作，就设置操作对象为0              
                vminfo[flag]=transList[i].action_trigger_stack1; flag++;//a
                break;

                case(2):
                if(transList[i].action_trigger_stack1!=0)//只有当要进行操作时才录入要操作的对象
                    {vminfo[flag]=(int)transList[i].target_trigger_stack1[0]; flag++;}//这个也被定义为char数组而不是char了 //p
                else
                    {vminfo[flag]=0; flag++;}//如果没有操作，就设置操作对象为0               
                vminfo[flag]=transList[i].action_trigger_stack1; flag++;//a

                if(transList[i].action_trigger_stack2!=0)//只有当要进行操作时才录入要操作的对象
                    {vminfo[flag]=(int)transList[i].target_trigger_stack2[0]; flag++;}//这个也被定义为char数组而不是char了 //p
                else
                    {vminfo[flag]=0; flag++;}//如果没有操作，就设置操作对象为0
                vminfo[flag]=transList[i].action_trigger_stack2; flag++;//a
                break;
            }
        } 
    return;
}

void update_transition_info_from(int index)//flag是当前第一个空位置的索引
{
    //从当前状态出发，有几条转移方向，flag=3，
    int sum=0;
    for(int i=0;i<numTransitions;i++)
        if(transList[i].index_status_from==index) sum++;
    vminfo[flag]=sum; flag++; //address_in_vm已经有了，不需要再写入

    //把转移的trigger写下来
    for(int i=0;i<numTransitions;i++)
        if(transList[i].index_status_from==index)
        {
            vminfo[flag]=(int)transList[i].trigger[0]; flag++;//因为之前trigger被定义成了char trigger[2] //c
            vminfo[flag]=search_address_given_index(transList[i].index_status_to); flag++;//k //这个地方修改了写入vm的地址不是index而是address
            switch(numStacks)
            {
                case(0):
                break;

                case(1):
                if(transList[i].action_trigger_stack1!=0)//只有当要进行操作时才录入要操作的对象
                    {vminfo[flag]=(int)transList[i].target_trigger_stack1[0]; flag++;}//这个也被定义为char数组而不是char了 //p
                else
                    {vminfo[flag]=0; flag++;}//如果没有操作，就设置操作对象为0              
                vminfo[flag]=transList[i].action_trigger_stack1; flag++;//a
                break;

                case(2):
                if(transList[i].action_trigger_stack1!=0)//只有当要进行操作时才录入要操作的对象
                    {vminfo[flag]=(int)transList[i].target_trigger_stack1[0]; flag++;}//这个也被定义为char数组而不是char了 //p
                else
                    {vminfo[flag]=0; flag++;}//如果没有操作，就设置操作对象为0               
                vminfo[flag]=transList[i].action_trigger_stack1; flag++;//a

                if(transList[i].action_trigger_stack2!=0)//只有当要进行操作时才录入要操作的对象
                    {vminfo[flag]=(int)transList[i].target_trigger_stack2[0]; flag++;}//这个也被定义为char数组而不是char了 //p
                else
                    {vminfo[flag]=0; flag++;}//如果没有操作，就设置操作对象为0
                vminfo[flag]=transList[i].action_trigger_stack2; flag++;//a
                break;
            }
        } 
    return;
}


int compile()
{
    vminfo[0]=numStacks;
    vminfo[1]=index_status_initial;
    vminfo[2]=numStatusFinal;


    flag=3;
    for(int i=0;i<numStatusFinal;i++)
        {vminfo[flag]=index_status_final[i]; flag++;}
    //第一篇运行transition_info_from,先临时写状态的index
    for(int i=0;i<numStatus;i++)
        transition_info_from(i);


    FILE* TSptr;
    TSptr=fopen("../log/TS.txt","w");
    for(int i=0;i<numStatus;i++)
        fprintf(TSptr,"name:%s ,index:%d ,address:%d \n",statusList[i].name,statusList[i].index,statusList[i].address_in_vm);
    
    fclose(TSptr);

    flag=3;
    for(int i=0;i<numStatusFinal;i++)
        {vminfo[flag]=search_address_given_index(index_status_final[i]); flag++;}
    for(int i=0;i<numStatus;i++)
        update_transition_info_from(i);

    FILE* VMptr;
    VMptr=fopen("../log/VM.txt","w");
    for(int i=0;i<flag;i++)
        fprintf(VMptr,"%d ",vminfo[i]);

    fclose(VMptr);
    return 0;
}