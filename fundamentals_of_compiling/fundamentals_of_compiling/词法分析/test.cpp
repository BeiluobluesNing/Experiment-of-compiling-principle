//
//  test.cpp
//  fundamentals_of_compiling
//
//  Created by 宁沛荣 on 2017/11/2.
//  Copyright © 2017年 宁沛荣. All rights reserved.
//


#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

#define ALPHA 7         //数字或者字符
#define LPARA 8         //左圆扩号 Parentheses
#define RPARA 9         //右圆扩号
#define COMMA 10        //,
#define SEMIC 11        //;
#define LBRAC 12        //左大括号braces
#define RBRAC 13        //右大括号
#define LSTHN 14        // <
#define LSOREQ 15       // <=
#define ASSIGN  16      // =
#define EQUAL 17        // ==
#define MRTHN 18        // >
#define MREQ 19         // >=
#define PLUS 20         //+
#define MINUS 21        //-
#define DIVIDE 22       // /
#define BY 23           // *

#define n 7

char *table[7]={" ","main","int","if","float","else","return"} , TOKEN[20] , ch;
FILE *pFile = fopen("/Users/ningpeirong/Desktop/fundamentals_of_compiling/fundamentals_of_compiling/output.txt","w");
//定义关键字
int lookup(char *token){                            //关键字匹配函数
    int m,i;
    for(i=1;i<6;i++){
        if((m=strcmp(token,table[i]))==0)
            return(i);
    }
    return(0);
}
void out(int c,char *TOKEN){                        //输出函数
    printf("(%d,%s)\n",c,TOKEN);
    fprintf(pFile, "(%d,%s)\n",c,TOKEN);
}
void scanner(FILE *fp)
{                               //扫描函数
    char TOKEN[20]={'\0'};
    char ch;
    int i,c;
    ch=fgetc(fp);              //获取字符指针fp并自动指向下一个字符
    if(ch == ' ')
        do
        {
            ch = fgetc(fp);
        }while(ch == ' ');
    
    if(isalpha(ch)){          //判断该字符是否是字母
        TOKEN[0]=ch;
        ch=fgetc(fp);
        i=1;
        while(isalnum(ch)){        //判断该字符串是否是字母或数字
            TOKEN[i]=ch;
            i++;
            ch=fgetc(fp);
        }
        TOKEN[i]='\0';
        fseek(fp,-1,1);            //回退一个字符
        c=lookup(TOKEN);            // 判断字符串是否为关键字
        if(c==0)
            out(ALPHA,TOKEN);        //输出标识符
        else out(c,TOKEN);         //输出关键字
    }
    else
        if(isdigit(ch))             //判断是否是数字
        {
            TOKEN[0]=ch;
            ch=fgetc(fp);
            i=1;
            if(isalpha(ch))
            {
                printf("have a error!\n");
                exit(0);
            }
            while(isdigit(ch))
            {
                TOKEN[i]=ch;
                i++;
                ch=fgetc(fp);
            }
            TOKEN[i]='\0';
            fseek(fp,-1,1);
            out(7,TOKEN);
        }
    
        else
        {
            TOKEN[0]=ch;
            switch(ch)
            {
                case'(':out(LPARA,TOKEN);
                    break;
                case')':out(RPARA,TOKEN);
                    break;
                case'{':out(LBRAC,TOKEN);
                    break;
                case'}':out(RBRAC,TOKEN);
                    break;
                case',':out(COMMA,TOKEN);
                    break;
                case';':out(SEMIC,TOKEN);
                    break;
                case '*':out(BY, TOKEN);
                    break;
                case '+':out(PLUS, TOKEN);
                    break;
                case '-':out(MINUS, TOKEN);
                    break;
                case '/':out(DIVIDE,TOKEN);
                    break;
                case'<':
                    ch=fgetc(fp);
                    TOKEN[1]=ch;
                    if(ch=='=')
                    {
                        out(LSOREQ,TOKEN);
                    }
                    else {
                        fseek(fp,-1,1);
                        out(LSTHN,TOKEN);
                    }
                    break;
                    
                case'=':
                    
                    ch=fgetc(fp);
                    TOKEN[1]=ch;
                    if(ch=='=')
                        out(EQUAL,TOKEN);
                    else if(isalpha(ch) && isalnum(ch))
                    {
                        printf("have a error!\n");
                        exit(0);
                    }
                    else
                    {
                        TOKEN[1]='\0';
                        fseek(fp,-1,1);
                        out(ASSIGN,TOKEN);
                    }
                    break;
                    
                case'>':
                    ch=fgetc(fp);
                    TOKEN[1]=ch;
                    if(ch=='=')
                        out(MRTHN,TOKEN);
                    else
                    {
                        fseek(fp,-1,1);
                        out(MRTHN,TOKEN);
                    }
                    break;
                default:
                    printf("error!\n");
                    break;
            }
        }
}

int main()
{   FILE *fp;
    if((fp=fopen("/Users/ningpeirong/Desktop/fundamentals_of_compiling/fundamentals_of_compiling/input.txt","r"))==NULL){
        fprintf(stderr,"error opening.\n");
        exit(1);
    }
    do{
        ch=fgetc(fp);
        if(ch=='#')                //文件以#结尾作为扫描结束条件
            break;
        if(ch=='\n')               //如果是换行符，则自动跳到下一个字符
            scanner(fp);

        else{
            fseek(fp,-1,1);   //如果不是空格，则回退一个字符并扫描 set_cur = 1
            scanner(fp);
        }
    }while(ch!='#');
    fclose(pFile);
    fclose(fp);
    return(0);
}

