#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#include<math.h>
#include<windows.h>
using namespace std;
#define MAXSIZE 100//用于转换表达式与求解
char str[MAXSIZE],str2[MAXSIZE];
int answer_count=0;
double answer[500];//记录表达式答案
int testnum=-1;//题目数量
int maxnum=-1;//最大数
int symbolnum[4]={0};//运算符数
int digitnum=-1;//题目数字个数
int bracketflag=-1;//是否有括号
int littleflag=-1;//是否有小数
int fileflag=-1;//是否输出到文件

void file_clear() //清空所有文件内容
{
    FILE *fp;
    fp=fopen("D:test_question.txt","w");
    fclose(fp);
    fp=fopen("D:test_question2.txt","w");
    fclose(fp);
    fp=fopen("D:test_answer.txt","w");
    fclose(fp);
}
void input_fun()//参数输入引导,有出错处理
{
    printf("*  请输入题目数目(大于0):                        *\n    ");
    while(testnum<=0)
    {
        scanf("%d",&testnum);
        if(testnum<=0)
        {
            printf("*  数目应大于0,请重新输入:                       *\n    ");
        }
    }
    printf("*  请输入题目运算数值的最大数(大于0):            *\n    ");
    while(maxnum<=0)
    {
        scanf("%d",&maxnum);
        if(maxnum<=0)
        {
            printf("*  最大数应大于0,请重新输入:                     *\n    ");
        }
    }
    printf("*  请输入每题数值的数目(大于1):                  *\n    ");
    while(digitnum<=1)
    {
        scanf("%d",&digitnum);
        if(digitnum<=1)
        {
            printf("*  数目应大于0,请重新输入:                       *\n    ");
        }
    }
    int flag=1,symnum;
    printf("*  请输入运算符(如输入1 2 -1,则含有+ -两个运算符)*\n");
    printf("*  编号对应运算符 1:+,2:-,3:×,4:÷; 输入-1结束  *\n    ");
    while(flag)
    {
        scanf("%d",&symnum);
        switch(symnum)
        {
            case 1:symbolnum[0]=1;break;
            case 2:symbolnum[1]=1;break;
            case 3:symbolnum[2]=1;break;
            case 4:symbolnum[3]=1;break;
            default:flag=0;
        }
    }
    printf("*  是否有括号(输入1:有,0:没有):                  *\n    ");
    while(bracketflag<0)
    {
        scanf("%d",&bracketflag);
        if(bracketflag!=1 && bracketflag!=0)
        {
            printf("*  输入错误,请重新输入:                          *\n    ");
        }
    }
    printf("*  是否有小数(输入1:有,0:没有):                  *\n    ");
    while(littleflag<0)
    {
        scanf("%d",&littleflag);
        if(littleflag!=1 && littleflag!=0)
        {
            printf("*  输入错误,请重新输入:                          *\n    ");
        }
    }
    printf("*  是否写入文件(输入1:写,0:不写):                *\n    ");
    while(fileflag<0)
    {
        scanf("%d",&fileflag);
        if(fileflag!=1 && fileflag!=0)
        {
            printf("*  输入错误,请重新输入:                          *\n    ");
        }
    }
}
//*****随机生成表达式函数*****
int randdigit()//随机生成整数
{
    int n;
    n=rand()%maxnum;
    return n;
}
float randlittle()//随机生成小数
{
    float n;
    n = rand()%maxnum + rand()%100*0.01;
    return n;
}
int randsymbol()//随机生成运算符
{
    int n,flag=1;
    while(flag)
    {
        n = rand()%4;
        if(symbolnum[n]!=0)
        {
            flag=0;
        }
    }
    return n+1;
}
int bracket_fun()//判断生成括号位置
{
    int n;
    n=rand()%(digitnum-1)-1;//随机数范围-1至digitnum-2
    return n;
}

//*****表达式转换成逆波兰式并求解*****
void GetStr(char *str) //读取表达式
{
    FILE *fp;
    int i=0;
    fp=fopen("D:test_question2.txt","rb");
    rewind(fp);//定位文件指针为首位
    while ((str[i] = fgetc(fp)) != EOF)
    {
        if (str[i] == ' ')//去掉多余的空格
        {
            i--;
        }
        i++;
    }
    str[i++] = '#';
    str[i] = '\0';
    fclose(fp);
}
void show_answer(char *str,double answer,int no) //写答案到文件
{
    FILE *fp;
    fp=fopen("D:test_answer.txt","aw");
    fprintf(fp,"(%d) %.2f\n",no,answer);
    fclose(fp);
}
double CharToDouble(char *str)//将字符串转换为浮点数
{
    double sumInt=0,sumDec=0,e=1;
    int i=0;
    while((str[i] >= '0')&&(str[i] <= '9'))//识别数字
    {
        sumInt=sumInt*10 + str[i++]-'0';
    }
    if(str[i] == '.') //如果含小数，处理小数部分
    {
        while(str[++i] != '\0')
        {
            sumDec=sumDec*10 + str[i]-'0';
            e *= 10;
        }
    }
    return sumInt + sumDec / e;
}
void Translate(char str[], char exp[]) //转换为逆波兰表达式
{
    char stack[MAXSIZE];
    int i=0, k=0, top=-1;
    while(str[i] != '#')
    {
        if(str[i] == '(')//直接将'('入栈
        {
            stack[++top]=str[i++];
        }
        else if(str[i] == ')')
        {
            while(top >= 0 && stack[top] != '(') //将'('之前的符号出栈，并存储到逆波兰表达式
            {
                exp[k++]=stack[top--];
            }
            if(top < 0) //'('不足，即有多余的')'
            {
                puts("'('不匹配\n");
                exp[k++] = '\0';
                return ;
            }
            top--;//去掉 '('
            i++;
        }
        else if(str[i] == '+' || str[i] == '-')
        {
            while(top >= 0 && stack[top] != '(') //如果有'('，将'('之前的符号出栈，否则所有符号出栈，并存储到逆波兰表达式
            {
                exp[k++]=stack[top--];
            }
            stack[++top]=str[i++]; //将新的运算符号入栈
        }
        else if(str[i] == '*' || str[i] == '/')
        {
            while(top >= 0 && (stack[top] == '*' || stack[top] == '/')) //将'*'和'/'之前的符号出栈，并存储到逆波兰表达式
            {
                exp[k++]=stack[top--];
            }
            stack[++top]=str[i++]; //将新的运算符号入栈
        }
        else
        {
            while((str[i] >= '0' && str[i] <= '9') || str[i] == '.') //将浮点数直接存储到逆波兰表达式
            {
                exp[k++]=str[i++];
            }
            exp[k++]='#'; //增加一个浮点数结束符号，以便正确提取浮点数
        }
    }
    while(top >= 0) //将栈中所有运算符号存储到逆波兰表达式
    {
        if(stack[top] == '(') //有多余的'('
        {
            puts("'('不匹配\n");
            exp[k++]='\0';
            return;
        }
        exp[k++]=stack[top--];
    }
    exp[k++]='\0';
}
double CompValue(char *exp) //求逆波兰表达式的值
{
    char tempStr[MAXSIZE];
    double stack[MAXSIZE];
    int i=0,k=0,top=-1;
    while(exp[i] != '\0')
    {
        if(exp[i] >= '0' && exp[i] <= '9')
        {
            k=0;
            while(exp[i] != '#')
            {
                tempStr[k++]=exp[i++];
            }
            tempStr[k]='\0';
            stack[++top] = CharToDouble(tempStr);
            i++;//跳过'#'
        }
        else
        {
            switch(exp[i++]) //将计算结果入栈，并退出多余的数字
            {
                case '+':
                    stack[top-1] += stack[top];
                    break;
                case '-':
                    stack[top-1] -= stack[top];
                    break;
                case '*':
                    stack[top-1] *= stack[top];
                    break;
                case '/':
                    if(stack[top] != 0)
                    {
                        stack[top-1] /= stack[top];
                    }
                    else
                    {
                        return 0;
                    }
                    break;
            }
            top--;//退出多余的数字
        }
    }
    return stack[top];
}

void create()//生成表达式
{
    int nu1;
    float nu2;
    int i,sy;
    int bf=-2;//匹配括号
    int num1=testnum;
    int num2=digitnum;
    printf("*                  生成题目如下                  *\n");
    FILE *fp1=fopen("D:test_question.txt","w");
    FILE *fp2;
    while(num1>0)
    {
        fp2=fopen("D:test_question2.txt","w+");
        if(bracketflag==1)//判断是否有括号
        {
            bf=bracket_fun();
        }
        printf("*  (%d) ",testnum-num1+1);
        if(fileflag==1)
        {
            fprintf(fp1,"(%d) ",testnum-num1+1);
        }
        nu1=randdigit();//获取随机数字
        nu2=randlittle();
        if(bf==-1)//左括号位置
        {
            printf("(");
            fprintf(fp2,"( ");
        }
        if(littleflag==1)//有小数
        {
            printf("%.2f",nu2);
            fprintf(fp2,"%.2f ",nu2);
        }
        else
        {
            printf("%d",nu1);
            fprintf(fp2,"%d ",nu1);
        }
        if(fileflag==1)//写文件时
        {
            if(bf==-1)
            {
                fprintf(fp1,"(");
            }
            if(littleflag==1)//有小数
            {
                fprintf(fp1,"%.2f",nu2);
            }
            else
            {
                fprintf(fp1,"%d",nu1);
            }
        }
        for(i=0;i<num2-1;i++)
        {
            sy=randsymbol();
            switch(sy)
            {
                case 1:
                    printf("+");
                    fprintf(fp2,"+ ");
                    if(fileflag==1)
                    {
                        fprintf(fp1,"+");
                    }
                    break;
                case 2:
                    printf("-");
                    fprintf(fp2,"- ");
                    if(fileflag==1)
                    {
                        fprintf(fp1,"-");
                    }
                    break;
                case 3:
                    printf("×");
                    fprintf(fp2,"* ");
                    if(fileflag==1)
                    {
                        fprintf(fp1,"×");
                    }
                    break;
                case 4:
                    printf("÷");
                    fprintf(fp2,"/ ");
                    if(fileflag==1)
                    {
                        fprintf(fp1,"÷");
                    }
                    break;
            }
            nu1=randdigit();//获取随机数字
            nu2=randlittle();
            if(i==bf)//左括号位置
            {
                printf("(");
                fprintf(fp2,"( ");
            }
            if(littleflag==1)//有小数
            {
                printf("%.2f",nu2);
                fprintf(fp2,"%.2f ",nu2);
            }
            else
            {
                printf("%d",nu1);
                fprintf(fp2,"%d ",nu1);
            }
            if((i==0 && bf==-1) || (i==(bf+1)))//右括号位置
            {
                printf(")");
                fprintf(fp2,")");
            }
            if(fileflag==1)//写文件时
            {
                if(i==bf)
                {
                    fprintf(fp1,"(");
                }
                if(littleflag==1)//有小数
                {
                    fprintf(fp1,"%.2f",nu2);
                }
                else
                {
                    fprintf(fp1,"%d",nu1);
                }
                if((i==0 && bf==-1) || (i==(bf+1)))
                {
                    fprintf(fp1,")");
                }
            }
        }
        fclose(fp2);
        GetStr(str);//获取表达式
        Translate(str,str2);//逆波兰式求解
        answer[answer_count] = CompValue(str2);//解存入答案数组记录
        show_answer(str,answer[answer_count],(testnum-num1+1)); //输出答案
        answer_count++;
        num1--;//控制数值个数
        printf("=\n");
        if(fileflag==1)
        {
            fprintf(fp1,"=\n");
        }
    }
    fclose(fp1);
}

void print_answer()//打印答案
{
    printf("*                  题目答案如下                  *\n");
    for(int i=0;i<answer_count;i++)
    {
        printf("*  (%d) %.2f\n",i+1,answer[i]);
    }
    printf("*------------------------------------------------*\n");
}
int color(int c)//更改颜色
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);
    return 0;
}
int main()
{
    int ctrl;
    color(11);
    while(1)
    {

        printf("**************************************************\n");
        printf("*--------------自动生成四则运算系统--------------*\n");
        printf("*----------------输入“1”进入系统---------------*\n");
        printf("*----------------输入“0”退出系统---------------*\n");
        printf("**************************************************\n");
        printf("*-----请输入: ");
        scanf("%d",&ctrl);
        if(ctrl==0)
        {
            printf("*-----已退出系统\r\n");
            return 0;
        }
        else if(ctrl==1)
        {
            system("cls");
            break;
        }
    }
    printf("**************************************************\n");
    printf("*        欢迎使用四则运算自动生成系统！          *\n");
    printf("*        接下来请按照系统提示进行操作!           *\n");
    printf("*------------------------------------------------*\n");
    file_clear();//文件初始化
    srand((int)time(0));//每次产生不同的随机数
    input_fun();//输入各参数
    printf("*------------------------------------------------*\n");
    create();//生成表达式
    printf("*------------------------------------------------*\n");
    print_answer();
    printf("*------------感谢您的使用,祝学习进步!------------*\n");
    printf("**************************************************\n");
    return 0;
}
