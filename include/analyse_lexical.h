/*
 * @Author: Abigail 
 * @Date: 2021-12-19 06:06:39 
 * @Last Modified by: Abigail
 * @Last Modified time: 2021-12-19 06:11:25
 */
 
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "transition.h"

#define true 0
#define false 1

//不变变量
static char * wordtable[5]={"Automate","etats","initial","final","transitions"};//关键字表
static char * chinese[12]={"零","一","二","三","四","五","六","七","八","九","十","→"};

// static char * francais[30]={"À","Â","È","É","Ê","Ë","Î","Ï","Ô","Ö","Ù","Û","Ç",
//                             "Œ","Æ","à","â","è","é","ê","î","ï","ô","ö","ù","û",
//                             "ü","ç","œ","æ"};

static char boundary[10]={'(', ')' , '[', ']' , '{' , '}', ',' , '`', '"','=' };


//写入后不变的全局变量
char targetfile[64];//存储目标文件名
char str[1024];//存储从目标文件中读取出来的字符串（无注释，无空格换行）
int strlength=0;

int numStatus=0;
int numStatusFinal=0;
int numTransitions=0;
int numStacks=0;

STATUS statusList[10];
char triggerList[10];
int index_status_initial=0;
int index_status_final[10];
TRANSITION transList[10];//存储转换关系


//总是变化的全局变量
char ch;//当前字符
int ch_index_in_str=0;//当前字符在str中的下标
int syn;//当前字符的种别码

char token[20];//当前从str中获取的关键字
int ch_index_in_token=0;//当前指向token的指针

char chinese_token[4];
int ch_index_in_chinesetoken=0;//当前指向token的指针

// char francais_token[3];

//判断是否为字母
int isLetter()  
{
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return true;
    return false;
}

// //判断是否为法语特殊字符
// int isFrancais(char ch)  
// {
//     for(int i=0;i<30;i++)
//         if(francais[i]==ch)return true;
//     return false;
// }

//判断是否为数字
int isDigit()  
{
	if ((ch >= '0')&&(ch <= '9'))
		return true;
    return false;
}

//判断是否为边界字符
int isBoundary()
{
    for(int i=0;i<10;i++)
        if(boundary[i]==ch)return true;
    return false;    
}


//读取原文件，并舍弃注释
//修改str
int readin_without_comment()
{
    // printf("Now in readin_without_comment! \n");

    int index_str=0;
    int flag_comment=0;

    FILE *read_targetfile_ptr;
    read_targetfile_ptr=fopen(targetfile,"r");
    char chNow=' ';
    char chBefore=chNow;

    if (read_targetfile_ptr==NULL)
    {
        fclose(read_targetfile_ptr);
        printf("File not Find");
    }
    else
    {
        while(chNow!=EOF)
        {
            //必须要加上这一句，chNow不会自动更新！
            chNow=getc(read_targetfile_ptr);

            //遇到 //... 类型注释
            if(chNow=='/' && chBefore=='/' && flag_comment==0){flag_comment=11;}
            if(chNow=='\n' && flag_comment==11){flag_comment=0;}

            //遇到/*...*/类型注释
            if(chNow=='*'&& chBefore=='/' && flag_comment==0){flag_comment=12;}
            if(chNow=='/' && chBefore=='*' && flag_comment==12){flag_comment=0;}

            else if (chNow!='\n' && chNow!='\t' && chNow!=' ' 
                    && chNow!='/' && chNow!='*' && chNow!=EOF
                    && flag_comment==0){
                str[index_str]=chNow;
                index_str++;
                }

            //更新chBefore的值
            chBefore=chNow;
        }
    }
    fclose(read_targetfile_ptr);
    strlength=index_str;
    return 0;
}


//对读入的文本进行逐字符检查，将结果写入临时文件
//修改str
int analyse_lexical()
{
    // printf("Now in analyse_lexical!\n");

    //初始化token
    for(ch_index_in_token=0;ch_index_in_token<20;ch_index_in_token++)
        token[ch_index_in_token]='\0';


    //初始化chinese_token
    for(ch_index_in_chinesetoken=0;ch_index_in_chinesetoken<4;ch_index_in_chinesetoken++)
        token[ch_index_in_chinesetoken]='\0';

    ch_index_in_token=0;
    ch_index_in_chinesetoken=0;
    ch=str[ch_index_in_str++];


    //特殊字符的组成char都是小于零的
    if(ch<0)
    {
        while(ch<0)
        {
            chinese_token[ch_index_in_chinesetoken++]=ch;
            ch=str[ch_index_in_str++];
        }

        ch_index_in_str--;//这是因为跳出循环时，index比我们已分析到的多了一位

        //判断是不是汉字
        for(int i=0;i<12;i++)
            if(strcmp(chinese_token,chinese[i])==0)
            {
                if(i<11)
                {
                    syn=10;//是汉字
                    printf("%s\t%d\n",chinese_token,syn);
                    return 0;                  
                }
                else
                {
                    syn=32;//是→
                    printf("%s\t%d\n",chinese_token,syn);
                    return 0;
                }               
            } 
        printf("error lexical: Invalid special character %s \n",chinese_token);
        return 1;    
    }
    
    //如果ch是字母
    else if(isLetter()==true)
    {

        //对所有字母开头的，都计入token
        while(isLetter()==true)
        {
            token[ch_index_in_token++]=ch;
            ch=str[ch_index_in_str++];
        }
        ch_index_in_str--;//这是因为跳出循环时，index比我们已分析到的多了一位

        syn=10;//是一个标识符        

        //检查是不是保留字
        for(int i=0;i<5;i++)
            if(strcmp(token,wordtable[i])==0)
            {
                syn=i+1;
            }
        printf("%s\t%d\n",token,syn);  
        return 0;                
                   


    }//这样做有一个问题，拼写错误的保留字会被认为是id标识符

    //如果ch是数字
    else if(isDigit()==true)
    {
        syn=11;//因为本次项目中数字都是单个出现，所以不需要计算数字总和
        printf("%c\t%d\n",ch,syn);
        return 0;
    }

    //如果ch是符号
    else if(isBoundary()==true)
    {
        switch(ch)
        {
            case '(':
                syn=21;
            break;
            case ')':
                syn=22;
            break;
            case '[':
                syn=23;
            break;
            case ']':
                syn=24;
            break;
            case '{':
                syn=25;
            break;
            case '}'://标志着字符串的结尾
                syn=26;
                printf("%c\t%d\n",ch,syn);
                return 1;             
            case ',':
                syn=27;
            break;
            case '`':
                syn=28;
            break;
            case '"':
                syn=28;
            break;      
            case '=':
                syn=31;
            break;                  
        }
        printf("%c\t%d\n",ch,syn);
        return 0;
    }
    else if(ch!='\0')
    {
        printf("error lexical: Invalid character %c \n",ch);
        return 1;
    }
    else return 0;
}
