#include "analyse_lexical.h"


int main() 
{
    printf("Enter the name of the file that you want to compile:");
    scanf("%s",targetfile);
    readin_without_comment();//把文件写入str字符串
  
    freopen("../log/testlex.log","w",stdout);
    while(ch_index_in_str<strlength && analyse_lexical()==0)
        analyse_lexical();//逐字符进行词法分析 
    freopen("/dev/tty","w",stdout);//!!血泪教训，不是console也不是CON！

    
    printf("Analyse lexical finish!\nRead testlex.log for more information\n");
    return 0;
}
