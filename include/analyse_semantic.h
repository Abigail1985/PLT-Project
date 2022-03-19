/*
 * @Author: Abigail 
 * @Date: 2021-12-19 22:27:44 
 * @Last Modified by: Abigail
 * @Last Modified time: 2021-12-19 22:53:09
 */


#include "analyse_syntax.h"

int analyse_semantic()
{
    if(numStatusFinal>numStatus)
    {
        printf("semantic error: the number of status final should be less than the total number of status\n");
        return 1;
    }

    if(index_status_initial>numStatus-1 || index_status_initial<0)
    {
        printf("semantic error: the initial status is undefinied.\n");
        return 1;        
    }

    for(int i=0;i<numStatus;i++)
    {
        if(transList[i].index_status_from>=numStatus-1 || transList[i].index_status_from<0 || transList[i].index_status_to>=numStatus-1 || transList[i].index_status_to<0 ){
            printf("semantic error: the status in transition is un definied\n");
            return 1;
        }
    }

    printf("No semantic error!\n");
    return 0;
    
}