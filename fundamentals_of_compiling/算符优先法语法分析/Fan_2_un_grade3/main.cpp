#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
using namespace std;

typedef struct SqStack  //定义栈的数据结构
{
    char   *base;
    char   *top;
}SqStack;

struct Query{           //链表结点名
    char thename[45];
};

struct QueNode{     //链表
    Query query;
    QueNode *pnext;
};

void print(QueNode *ph)  //输出链表
{
    QueNode *p;
    p=ph->pnext;  //
    while (p!=NULL)
    {
        cout<<p->query.thename<<endl;
        p=p->pnext;
    }
}

QueNode *Addshu(QueNode *ph,char *str)
{
    QueNode *p1;
    p1=new QueNode();
    strcpy(p1->query.thename,str);
//    if (ph != NULL)
//    {
//        cout<<"ph为"<<endl;
//        print(ph);
//        cout<<"p1为"<<endl;
//        print(p1);
//    }
    p1->pnext=ph;   //在ph表头插入数据
    ph=p1;
//    if (ph != NULL)
//    {
//        cout<<"操作后的ph为"<<endl;
//        print(ph);
//    }
    return ph;
}
void Push(SqStack &S,char e){   // 将元素e插入到栈S中，成为新的栈顶元素
    *S.top++=e;
}

char Pop(SqStack &S,char &ch){  //字符型取栈顶元素并删除之
    if(S.top==S.base){
        ch=NULL;
    }
    else ch=*--S.top;
    return ch;
}
char Gettop(SqStack &S,char &e)     //字符型取栈顶元素
{
    if(S.base == S.top)
        e = NULL;
    else
        e = *(S.top-1);
    return e;
}
int in(char c){                 //判断是否为运算符或者为界限符
    char su[10]={ '+','-','*','/','(',')',';','=' };
    for(int i=0;i<8;i++)        //依次比对
        if(c==su[i])
            return 1;           //是则返回true
    return 0;
}
//
char precede(char x1,char x2){//优先级判断   x1为栈顶运算符x2为后读入的运算符
    char c1,c2,c = '\0';
    c1=x1;
    c2=x2;
    if(c1=='('){
        if(c2==')')
            c='=';
        else
            c='<';
    }
    else if(c1==')')        //右括号优先级最高
        c='>';
    else if(c1==';')        //终结符
    {
        if(c2==';')
            c='=';          //结束
        else
            c='<';          //否则;更高
    }
    else{
        int a = 0,b = 0;
        switch(c1)
        {
            case '+':
            case '-':a=2;
                break;
            case '*':
            case '/':a=3;
                break;
            case '(':a=4;break;
            case ')':a=1;break;
            case ';':a=-2;break;
            case '=':a=-1;break;
        }
        switch(c2){
            case '=':a=-1;
                break;
            case '+':
            case '-':b=2;
                break;
            case '*':
            case '/':b=3;break;
            case '(':b=4;break;
            case ')':b=1;break;
            case ';':b=-2;break;
        }
        
        if(a-b>=0)
            c='>';
        if(a-b<0)
            c='<';
    }
    return c;
}
int main(){
    cout<<"赋值语句翻译成四元式！！"<<endl;
    string ina,ona;
//    cout<<"请输入源文件名："<<endl;
//    cin>>ina;
    ina = "/Users/ningpeirong/Desktop/Fan_2_un_grade3/Fan_2_un_grade3/test.txt";
    ifstream fin;
    fin.open(ina.c_str(),ios::in);           //生成一个const char*指针，指向以空字符终止的数组,为输入打开文件
    if(!fin){
        cout<<"open error!"<<endl;
        exit(-1);
    }
//    cout<<"请输入要生成的文件名："<<endl;
//    cin>>ona;
    ona = "/Users/ningpeirong/Desktop/Fan_2_un_grade3/Fan_2_un_grade3/test_1.txt";
    ofstream quedata;
    quedata.open(ona.c_str(),ios::out);     //生成一个const char*指针，指向以空字符终止的数组,为输出打开文件
    
    if(!quedata)
    {
        cout<<"open error!"<<endl;
        exit(-1);           //退出程序
    }
    char c,e,theta,x;           //c为文件中读入字符中间变量 e为栈顶字符中间变量
    char left_op[20]="",right_op[20]="";
    int s = 0 , i = 1;
    char chs[20]= "", che[2] = "" ,kong[20]="";      //chend ch_string
    QueNode *ph = NULL;
    char str[50];
    
    SqStack stack;
    stack.base = stack.top = &str[0];           //初始化栈操作
    Push(stack,';');                            //栈底放入;做终结符
    
    fin.get(c);                                 //取第一个字符
    
    while(c!=';'||Gettop(stack,e)!=';')
    {
//        cout<<"c = "<<c<<endl;
        if(!in(c))      //非运算符情况
        {
            che[0]=c;       //字符串中首字母为从文本中提取的字符 供多个字符的长整数小数多字符数据使用
            if((c>='A'&&c<='Z')||(c>='a'&&c<='z')||c=='_')
                i=0;                                //判断是否为字母 中间可以加_
//            cout<<"che = "<<che<<endl;
            strcat(chs,che);
            
//            cout<<"chs = "<<chs<<endl;

            fin.get(c);         //再取一个字符

            if(in(c))           //词法分析 判断之前if中的类型 当下一个字符为运算符或者界限符号;时，对其进行判断
            {
                if(i==0)
                    quedata<<"< "<<chs<<" ,标识符>"<<endl;     //写入前一个字符的类型，含或不含字母
                else
                    quedata<<"< "<<chs<<" ,数字>"<<endl;
//                cout<<"chs = "<<chs<<endl;
                
                ph=Addshu(ph,chs);              //输入操作数

                strcpy(chs,kong);                //将chs赋值为空
                
                if(c!=';')
                    quedata<<"< "<<c<<" , 运算符>"<<endl;
                else
                    quedata<<"< "<<c<<"  ,界限符>"<<endl;
            }
        }
        else            //运算符的情况
        {
            char a2=s+'0';                  // a2 赋值为 's'
            char a1[]={'(','t',a2,')','\0'};
            i=1;                            //不是字符
            switch(precede(Gettop(stack,e),c))      //对运算符和栈顶的运算符进行判断
            {
                case '<':
                    if(c=='('||c==')')
                        quedata<<"< "<<c<<"  ,运算符>"<<endl;
                    Push(stack,c);                 //把该运算符也压入栈🙈
                    fin.get(c);
                    break;
                case '=':
                    x=Pop(stack,x);
                    fin.get(c);
                    break;
                case '>':
                    theta=Pop(stack,theta);                 //取栈顶这个比后一个运算符优先级高的运算符，并删除该运算符
                    strcpy(right_op,ph->query.thename);     //取运算符右边运算数 并删除该数
                    ph=ph->pnext;
                    strcpy(left_op,ph->query.thename);      //取运算符左边的运算数 并删除该数
                    ph=ph->pnext;
                    if(theta=='=')                          //遇到赋值号 结束
                    {
                        cout<<"("<<s<<")  "<<"("<<theta<<","<<right_op<<",-"<<","<<left_op<<")"<<endl;
                    }
                    else
                        cout<<"("<<s<<")  "<<"("<<theta<<","<<left_op<<","<<right_op<<",(t"<<s<<"))"<<endl;
                    ph=Addshu(ph,a1);       //把链表头节点换为
                    s++;        //从0开始的第i个4元式子
                    break;
            }
        }
    }
    quedata.close();
    fin.close();
}
