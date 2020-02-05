#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80                     /* 指令最大长度 */
char buf[MAX_LINE / 2 + 1][MAX_LINE];   /* 当前指令的缓冲区 */
char hty[10][MAX_LINE];                 /* 历史记录数组 */
int count = 0;                          /* 历史记录数量 */
int flag = 0;                           /* 指令特征 */
int num = 0;                            /* 调用执行历史记录函数的次数 */

int execset(char *args[],char str[]);   //初始化
void exe(char *args[]);                 //执行指令
void exe(char *args[],char ch[]);       //执行历史记录
void history(char str[]);               //创建历史记录

/** 
* 设置args，分词
* 普通指令，返回0
* 存在&，返回1
* 存在！，返回2
* 指令是history，返回3
**/
int execset(char *args[],char str[])
{
    int i = 0, j = 0, k = 0;
    memset(buf,0,MAX_LINE * (MAX_LINE/2+1));
    while(j < MAX_LINE/2+1)
        args[j++] = NULL;
    while(str[i] != '\0'){
        for(j = 0;str[i] != ' ' && str[i] != '\0';i ++, j ++){
            buf[k][j] = str[i];
        }
        i ++;
        buf[k][j] = '\0';
        args[k] = buf[k];
        k++;
    }
    if(strcmp(args[k-1],"&") == 0)
    {
        args[k-1] = NULL;
        return 1;
    }
    else if(args[0][0] == '!')
    {
        return 2;
    }
    else if (strcmp(args[0],"history") == 0)
    {
        return 3;
    }
    else
        return 0;
}

/** 执行指令 **/
void exe(char *args[])
{
    switch(flag){
        case 0: 
        case 1: {
            pid_t fpid = fork();
            if (fpid < 0)   
                printf("error in fork!");   
            else if(fpid == 0){
                execvp(args[0],args);
            }
            else{
                if(flag)
                    wait(NULL);
            }
            break;
        }
        case 2:
        case 3: {
            exe(args,args[0]);
            break;
        }
        default: printf("flag出错!\n"); break;
    }
}
/** 执行历史记录 **/
void exe(char *args[],char ch[])
{
    int last = count > 10 ? 9 : count-1;
    if(count == 0)
        printf("尚未创建历史记录\n");
    else if (ch[1] == '!')
    {
        flag = execset(args,hty[last-num++]);
        exe(args);
    }
    else if (strcmp(ch,"history") == 0)
    {
        for(int i = 0;i < count && i < 10;i ++)
            printf("%d %s\n",i+1,hty[i]);
    }
    else
    {
        if(count < (int)(args[0][1] - '0'))
            printf("没有此条历史记录\n");
        else
        {
            flag = execset(args,hty[ch[1]-'1']);
            exe(args);
        }
    }
}

/** 创建历史记录 **/
void history(char str[])
{
    if(count < 10){
        strcpy(hty[count++],str);
    }
    else{
        for(int i = 0;i < 9; i ++)
            strcpy(hty[i],hty[i+1]);
        strcpy(hty[9],str);
    }
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    int i = 0,length = 0; /* i 公共的计数器，length 读入的长度 */
    char tmps[81]; /* 读入的缓冲区 */

    /* args初始化 */
    while(i++ < MAX_LINE / 2 + 1)
        args[i] = NULL;
    
    /* 主体 */
    while (should_run){
        printf("osh>");
        fflush(stdout);
        memset(tmps,0,85);
        length = read(STDIN_FILENO,tmps,MAX_LINE);
        tmps[length-1] = '\0';
        if(strcmp(tmps,"exit") == 0)
            should_run = 0;
        else
        {
            flag = execset(args,tmps);
            exe(args);
            history(tmps);
            num = 0;
        }
    }

    return 0;
}
