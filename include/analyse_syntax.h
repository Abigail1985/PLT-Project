/*
 * @Author: Abigail 
 * @Date: 2021-12-05 22:32:31 
 * @Last Modified by: Abigail
 * @Last Modified time: 2021-12-19 06:35:59
 */

#include "analyse_lexical.h"

int flag_error=0;

void errorinfo()
{
  printf("(error position:%d)\tsyntax error: \n",ch_index_in_str+1);
 flag_error=1;
  return;
}

void st()
{
  analyse_lexical();
  if(syn==28 || syn==29){//双引号或单引号
    analyse_lexical();
    if(syn==11){//数字
      statusList[numStatus].name[0]=ch;//将状态名char放入状态名数组
      statusList[numStatus].index=numStatus;
      numStatus++;//更新状态个数
    }
    else if(syn==10){
      if(ch_index_in_chinesetoken>0 && ch_index_in_token==0){//汉字
        strcpy(statusList[numStatus].name,chinese_token);//将状态名char放入状态名数组
        statusList[numStatus].index=numStatus;
        numStatus++;//更新状态个数
      }
      else if(ch_index_in_chinesetoken==0 && ch_index_in_token>0){//英文
        strcpy(statusList[numStatus].name,token);//将状态名char放入状态名数组
        statusList[numStatus].index=numStatus;
        numStatus++;//更新状态个数
      }
      else errorinfo();      
    }
    else errorinfo();
  }
  else errorinfo();

  analyse_lexical();
  if(syn==28 || syn==29)return;
  else{errorinfo();return;}
}

void etats_statement()
{
  printf("Now in etats_statement\n");

  analyse_lexical();
  if(syn==2){//etats
    analyse_lexical();
    if(syn==31){//=
      analyse_lexical();
      if(syn==23){//[
        st();
        analyse_lexical();
        while(syn==27){
          st();
          analyse_lexical();
        }
        if(syn==24 || flag_error==0) printf("etats analyse success\n\n");
        else errorinfo();
      }
      else errorinfo();
    }
    else errorinfo();
  }
  else errorinfo();
  return;
}

void initial_statement()
{
  printf("Now in initial_statement\n");  

  analyse_lexical();
  if(syn==3){
    analyse_lexical();
    if(syn==31){
      analyse_lexical();
      if(syn==11){
        index_status_initial=ch-'0'; //获取初始状态的索引，并从char转换为int
        printf("initial analyse success\n\n");
      }
      else errorinfo();
    }
    else errorinfo();
  }
  else errorinfo();
  return;
}

void final_statement()
{
  printf("Now in final_statement\n");

  analyse_lexical();
  if(syn==4){
    analyse_lexical();
    if(syn==31){
      analyse_lexical();
      if(syn==23){
        analyse_lexical();
        index_status_final[numStatusFinal]=ch-'0';
        numStatusFinal++;

        analyse_lexical();
        while(syn==27){
          analyse_lexical();
          index_status_final[numStatusFinal]=ch-'0';
          numStatusFinal++;
          analyse_lexical();
        }
        if(syn==24 || flag_error==0) printf("final analyse success\n\n");
        else errorinfo();      
      }
      else errorinfo();
    }
    else errorinfo();
  }
  else errorinfo();
  return;
}

void srcdst()
{
  analyse_lexical();
  if(syn==11){
    transList[numTransitions].index_status_from=ch-'0';
    analyse_lexical();
    if(syn==32){
      analyse_lexical();
      if(syn==11){
        transList[numTransitions].index_status_to=ch-'0';
        return;
      }
      else errorinfo();            
    }
    else errorinfo();
  }
  else errorinfo();
  return;
}

void trig()
{
  analyse_lexical();
  if(syn==28){
    analyse_lexical();
    if(syn==11 || syn==10){
      switch(syn)
      {
        case 11: transList[numTransitions].trigger[0]=ch;break;
        case 10: strcpy(transList[numTransitions].trigger,token);break;
      }
      analyse_lexical();
      if(syn==28) return;
      else errorinfo();
    }
    else errorinfo();
  }
  return;
}

void st_action1()
{
  analyse_lexical();
  if(syn==21){//(
    analyse_lexical();
    if(syn==22){//直接括号闭合
      transList[numTransitions].action_trigger_stack1=0;
      return;
    }

    else if(syn==32){//箭头
      transList[numTransitions].action_trigger_stack1=1;
      analyse_lexical();
      if(syn==27){//,
        analyse_lexical();
        if(syn==28){//前单引号
          analyse_lexical();
          if(syn==10 || syn==11){
            switch(syn)
            {
              case 11: transList[numTransitions].target_trigger_stack1[0]=ch;break;
              case 10: strcpy(transList[numTransitions].target_trigger_stack1,token);break;
            }
            analyse_lexical();//后单引号
            analyse_lexical();//)
            if(syn==22) return;
            else errorinfo();
          }
          else errorinfo();
        }
        else errorinfo();
      }
      else errorinfo();
    }


    else if(syn==28){//trigger的前单引号‘
      transList[numTransitions].action_trigger_stack1=-1;
      analyse_lexical();
      if(syn==10 || syn==11){
        switch(syn)
        {
          case 11: transList[numTransitions].target_trigger_stack1[0]=ch;break;
          case 10: strcpy(transList[numTransitions].target_trigger_stack1,token);break;
        }
        analyse_lexical();//后单引号
        analyse_lexical();//,
        analyse_lexical();//→
        analyse_lexical();//)
        if(syn==22) return;
        else errorinfo();
      }
      else errorinfo();
    }
    else errorinfo();
  }
  return;
}

void st_action2()
{
  analyse_lexical();
  if(syn==21){//(
    analyse_lexical();

    if(syn==22){//直接括号闭合
      transList[numTransitions].action_trigger_stack2=0;
      return;
    }

    else if(syn==32) {//箭头
      transList[numTransitions].action_trigger_stack2=1;
      analyse_lexical();
      if(syn==27){//,
        analyse_lexical();
        if(syn==28){//前单引号
          analyse_lexical();
          if(syn==10 || syn==11 ){
            switch(syn)
            {
              case 11: transList[numTransitions].target_trigger_stack2[0]=ch;break;
              case 10: strcpy(transList[numTransitions].target_trigger_stack2,token);break;
            }
            analyse_lexical();//后单引号
            analyse_lexical();//)
            if(syn==22) return;
            else errorinfo();
          }
          else errorinfo();
        }
        else errorinfo();
      }
      else errorinfo();
    }


    else if(syn==28) {//trigger的前单引号‘
      transList[numTransitions].action_trigger_stack2=-1;
      analyse_lexical();
      if(syn==10 || syn==11){
        switch(syn)
        {
          case 11: transList[numTransitions].target_trigger_stack2[0]=ch;break;
          case 10: strcpy(transList[numTransitions].target_trigger_stack2,token);break;
        }
        analyse_lexical();//后单引号
        analyse_lexical();//,
        analyse_lexical();//→
        analyse_lexical();//)
        if(syn==22) return;
        else errorinfo();
      }
      else errorinfo();
    }
    else errorinfo();
  }
  return;
}


void trans()
{
  switch(numStacks)
  {
    case 0:
      analyse_lexical();
      if(syn==21){//(
        srcdst();
        analyse_lexical();
        if(syn==27){//,
          trig();
          analyse_lexical();
          if(syn==22) return;
          else errorinfo();
        }
        else errorinfo();
      }
      else errorinfo();
    break;

    case 1:
      analyse_lexical();
      if(syn==21){//(
        srcdst();
        analyse_lexical();
        if(syn==27){//,
          trig();
          analyse_lexical();
          if(syn==22){
            transList[numTransitions].action_trigger_stack1=0;
            return;
          }
          else if(syn==27){
            st_action1();
            analyse_lexical();
            if(syn==22) return;
            else errorinfo();
          }
          else errorinfo();
        }
        else errorinfo();
      }
      else errorinfo();    
    break;

    case 2:
      analyse_lexical();
      if(syn==21){//(
        srcdst();
        analyse_lexical();
        if(syn==27){//,
          trig();
          analyse_lexical();
          if(syn==27){//,
            st_action1();
            analyse_lexical();
            if(syn==22){
              transList[numTransitions].action_trigger_stack2=0;
              return;
            }
            else if(syn==27){//,
              st_action2();
              analyse_lexical();
              if(syn==22) return;
              else errorinfo();
            }
            else errorinfo();
          }
          else errorinfo();
        }
        else errorinfo();
      }
      else errorinfo();    
    break;
  }
  return;
}

void transitions_statement()
{
    printf("Now in transitions_statement\n");  

    analyse_lexical();
    if(syn==5){//transition
        analyse_lexical();
        if(syn==31){//=
            analyse_lexical();
            if(syn==23){//[
                trans();
                numTransitions++;
                analyse_lexical();
                while(syn==27){
                    trans();
                    numTransitions++;
                    analyse_lexical();
                }
                if(syn==24 || flag_error==0 ) printf("transition analyse success\n\n");
                else errorinfo();
            }       
            else errorinfo();      
        }
        else errorinfo();
    }
    else errorinfo();
  return;
}

int analyse_syntax()
{
  printf("Now in analyse_lexical\n");  

  if(syn==1){//开始
    analyse_lexical();
    if(syn==21){//(开头
      analyse_lexical();
      if(syn==11){//给了stack个数
        numStacks=ch-'0';//读取stack个数,char转int,利用char使用ascii进行运算的特点
        analyse_lexical();
        if(syn==22){//)结束
          analyse_lexical();
          if(syn==31){//=
            analyse_lexical();
            if(syn==25){//{
              etats_statement();//分析etats
              initial_statement(); //分析intials
              final_statement();//分析final
              transitions_statement();//分析transitions
              analyse_lexical();
              if(syn==26 && flag_error==0 ){//走到最后的}处
                printf("Analyse syntax success!\n");
                return 0;
              }
              else errorinfo();
            }
            else errorinfo();
          }
          else errorinfo();
        }
        else errorinfo();
      }
      else errorinfo();
    }
    else errorinfo();
  }
  else errorinfo();

  return flag_error;
}

void print_syn_result()
{
    freopen("../log/testsyn_result.log","w",stdout);
    for(int i=0;i<numStatus;i++)
        printf("Status: %s\n",statusList[i].name);
    printf("numStacks: %d\n",numStacks);  
    printf("index_status_initial: %d\n",index_status_initial);
    for(int i=0;i<numStatusFinal;i++)
        printf("index_status_final: %d\n",index_status_final[i]);
    for(int i=0;i<numTransitions;i++)
    {
      printf("from:%d,\tto:%d,\ttrigger:%c,\nstack1:%c,\taction1:%d,\tstack2:%c,\taction2:%d\n\n",
              transList[i].index_status_from,transList[i].index_status_to,transList[i].trigger[0],
              transList[i].target_trigger_stack1[0],transList[i].action_trigger_stack1,
              transList[i].target_trigger_stack2[0],transList[i].action_trigger_stack2);
    }
    freopen("/dev/tty","w",stdout);//!!血泪教训，不是console也不是CON！
    return;
}