#include "analyse_semantic.h"


int main() 
{
    printf("Enter the name of the file that you want to compile:");
    scanf("%s",targetfile);
    readin_without_comment();//把文件写入str字符串

    freopen("../log/testsyn.log","w",stdout);
    analyse_lexical();//逐字符进行词法分析 
    analyse_syntax();
    freopen("/dev/tty","w",stdout);//!!血泪教训，不是console也不是CON！

    print_syn_result();

    freopen("../log/testsem.log","w",stdout);
    analyse_semantic();
    freopen("/dev/tty","w",stdout);//!!血泪教训，不是console也不是CON！    

    printf("Analyse semantic finish!\nRead testsem.log for more information\n");
    return 0;
}
