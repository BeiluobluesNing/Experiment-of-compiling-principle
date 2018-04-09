//有效字符串: IF,ELSE,THEN, a,b,x,>,=


/*
 文法:
 
 S->id=id | if E then S else S
 E->id < id
 E->id > id
 
 
 */
#include<iostream>
#include<fstream>
#include<string>


#define $ASSIGN 249
#define $IF 250
#define $THEN 251
#define $ELSE 252
#define $GREAT 253
#define $LESS 248
#define $ID 254

using namespace std;
typedef struct Wtoken
{
    int type;
    char ch;
    
}Wtoken;

typedef enum
{
    JUMP,JG,JL,ASSIGN,END//跳转，较大，较小，赋值，结束
}OpKind;

typedef struct
{
    int label;//标号
    OpKind op;
    char par1,par2;
    union
    {
        char result;
        int address;
    };
}Fourtable;//四元式

#define MAX_TOKEN 256 //Wtoken表大小
#define MAX_QUAD 256 //四元式数组大小

Wtoken tokentable[MAX_TOKEN];
Fourtable quad[MAX_QUAD];       //四元式结构体数组
int token_index;//token表索引
int total_len;//token表有效长度

int quad_len;//四元式表有效长度
int quad_index;//四元式索引

int label;
ifstream ins;

bool init(char filename[255]);
bool cifafenxi();
void print();

void ERROR();
void S(int,int);

void E(int,int,int);
bool nexttoken();
int newlabel();
void yufafenxi();

void printFourtable();
int main()
{

    cout<<"Please input the name of file:";
    char fname[200];
    cin>>fname;
    if(!init(fname))
        return 0;
    if(!cifafenxi())
        return 0;
    char ch;
    while(1)
    {
        if(ins.eof())
            break;
        ins>>ch;
    }
    cout<<"The result of CIFA analyse :"<<endl;
    print();
    cout<<endl<<endl;
    nexttoken();
    cout<<"The Grammar:"<<endl;
    yufafenxi();
    cout<<"The FourTable as followings:"<<endl;

    printFourtable();
}

int newlabel()
{
    return label++;
}

bool init(char filename[255])
{
    token_index = 0;
    total_len = 0;
    quad_len = 0;
    quad_index = 0;
    label = 100;
    ins.open(filename, ios::in);
    if(ins.fail())
    {
        cout<<"文件打开出错!"<<endl;
        return false;
    }
    
    return true;
}
//

bool cifafenxi()
{
    char buf[16];
    char ch;
    while(1)
    {
        ins>>ch;
        if(ins.fail())
            break;
        if(ch=='I')
        {
            ins>>buf;
            if(strcmp(buf,"F")==0)
                tokentable[total_len++].type=$IF;
        }
        else if(ch=='T')
        {
            ins>>buf;
            if(strcmp(buf,"HEN")==0)
                tokentable[total_len++].type=$THEN;
        }
        else if(ch=='E')
        {
            ins>>buf;
            if(strcmp(buf,"LSE")==0)
                tokentable[total_len++].type=$ELSE;
        }
        else if(ch=='>')
        {
            tokentable[total_len++].type=$GREAT;
        }
        else if(ch=='<')
        {
            tokentable[total_len++].type=$LESS;
        }
        else if(ch=='=')
        {
            tokentable[total_len++].type=$ASSIGN;
        }
        else if(ch=='x' || (ch>='a' && ch<='z'))
        {
            tokentable[total_len].type=$ID;
            tokentable[total_len++].ch=ch;
        }
    }
    return true;
    
}

void AD_RESULT(int nlabel,OpKind nop,char npar1,char npar2,int nresult)
{
    quad[quad_len].label = nlabel;
    quad[quad_len].op = nop;
    quad[quad_len].par1 = npar1;
    quad[quad_len].par2 = npar2;
    quad[quad_len].result = nresult;
    quad_len++;
}


void AD_ADDRESS(int nlabel,OpKind nop,char npar1,char npar2,int naddress)
{
    quad[quad_len].label=nlabel;
    quad[quad_len].op=nop;
    quad[quad_len].par1=npar1;
    quad[quad_len].par2=npar2;
    quad[quad_len].address=naddress;
    quad_len++;
}

Wtoken cur; //关键字

bool nexttoken()
{
    if(token_index>=total_len)
        return false;
    cur.type = tokentable[token_index].type;
    cur.ch = tokentable[token_index].ch;
    token_index++;
    return true;
}



void ERROR(char str[20])
{
    cout<<"error!  "<<str<<endl;
}

void S(int begin,int next)
{
//    Fourtable quads[MAX_QUAD];       //四元式结构体数组
//    
//    quads[MAX_QUAD] = quad[MAX_QUAD];
    
    if(cur.type==$ID)
    {
        char a,b;
        cout<<"S->"<<cur.ch;
        a=cur.ch;
        if(!nexttoken())
            ERROR("S");
        if(cur.type!=$ASSIGN)
            ERROR("S");
        cout<<"=";
        if(!nexttoken())
            ERROR("S");
        if(cur.type!=$ID)
            ERROR("S");
        cout<<cur.ch<<endl;
        b=cur.ch;
        
        AD_RESULT(begin,ASSIGN,b,0,a);
        AD_ADDRESS(-1,JUMP,0,0,next);
        nexttoken();
    }
    else if(cur.type==$IF)
    {
        if(!nexttoken())
            ERROR("S");
        {
            cout<<"S->if E then S else S"<<endl;
            int etrue=newlabel();       //判断正确的情况
            int efalse=newlabel();      //判断为错误的情况
            
            E(begin,etrue,efalse);
            if(cur.type==$THEN)
            {
                if(!nexttoken())
                    ERROR("S");
                
                S(etrue,next);
                
                if(cur.type==$ELSE)
                {
                    if(!nexttoken())
                        ERROR("S");
                    S(efalse,next);
                }
                else
                {
                    ERROR("S");
                }
            }
            else
                ERROR("S");
        }
    }
}

void E(int begin,int etrue,int efalse)
{
//    cout<<cur.type<<endl;
    if(cur.type==$ID)
    {
        char a,b;
        int mark=0;
        a=cur.ch;
        cout<<"E->"<<cur.ch;
        if(!nexttoken())
            ERROR("E()");
        if(cur.type==$GREAT)
        {
            cout<<'>';
            mark=1;
        }
        else if(cur.type==$LESS)
            cout<<'<';
        else
            ERROR("E()");
        
        if(!nexttoken())
            ERROR("E()");
        if(cur.type!=$ID)
            ERROR("E()");
        cout<<cur.ch<<endl;
        b=cur.ch;
        
        if(mark==0)
            AD_ADDRESS(begin,JL,a,b,etrue);
        if(mark==1)
            AD_ADDRESS(begin,JG,a,b,etrue);
        
        AD_ADDRESS(-1,JUMP,0,0,efalse);
        if(!nexttoken())
            ERROR("E()");
    }
    else
        ERROR("E()");
}

void yufafenxi()
{
    S(-1,200);
    AD_RESULT(200,END,0,0,'-');
}


void print()
{
    for(token_index=0;token_index<total_len;token_index++)
    {
        if(tokentable[token_index].type==$IF)
            cout<<"IF"<<"      "<<"关键字"<<endl;
        if(tokentable[token_index].type==$ELSE)
            cout<<"ELSE"<<"    "<<"关键字"<<endl;
        if(tokentable[token_index].type==$THEN)
            cout<<"THEN"<<"    "<<"关键字"<<endl;
        if(tokentable[token_index].type==$ID)
            cout<<tokentable[token_index].ch<<"       "<<"标识符"<<endl;
        if(tokentable[token_index].type==$ASSIGN)
            cout<<'='<<"       "<<"运算符"<<endl;
        if(tokentable[token_index].type==$GREAT)
            cout<<'>'<<"       "<<"运算符"<<endl;
        if(tokentable[token_index].type==$LESS)
            cout<<'<'<<"       "<<"运算符"<<endl;
        
    }
    token_index=0;
}


void printFourtable()
{
    for(int i=0;i<quad_len;i++)
    {
        if(quad[i].label>-1)
            cout<<"L"<<quad[i].label<<":";
        else cout<<"     ";
        if(quad[i].op==JG)
        {
            cout<<"(j>,  "<<quad[i].par1<<","<<quad[i].par2<<",L"<<quad[i].address<<")"<<endl;
        }
        else if(quad[i].op==JL)
        {
            cout<<"(j<,  "<<quad[i].par1<<","<<quad[i].par2<<",L"<<quad[i].address<<")"<<endl;
        }
        else if(quad[i].op==JUMP)
        {
            cout<<"(j,  "<<"-,-,L"<<quad[i].address<<")"<<endl;
        }
        else if(quad[i].op==ASSIGN)
        {
            cout<<"(= ,  "<<quad[i].par1<<",-,"<<quad[i].result<<")"<<endl;
        }
        else if(quad[i].op==END)
        {
            cout<<"(-,-,-,-)"<<endl;
        }
    }
}
