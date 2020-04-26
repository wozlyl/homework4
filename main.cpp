#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#include<math.h>
using namespace std;
#define MAXSIZE 100//����ת�����ʽ�����
char str[MAXSIZE],str2[MAXSIZE];
int answer_count=0;
double answer[500];//��¼���ʽ��
int testnum=-1;//��Ŀ����
int maxnum=-1;//�����
int symbolnum[4]={0};//�������
int digitnum=-1;//��Ŀ���ָ���
int bracketflag=-1;//�Ƿ�������
int littleflag=-1;//�Ƿ���С��
int fileflag=-1;//�Ƿ�������ļ�

void file_clear() //��������ļ�����
{
    FILE *fp;
    fp=fopen("D:test_question.txt","w");
    fclose(fp);
    fp=fopen("D:test_question2.txt","w");
    fclose(fp);
    fp=fopen("D:test_answer.txt","w");
    fclose(fp);
}
void input_fun()//������������,�г�����
{
    printf("*  ��������Ŀ��Ŀ(����0):                        *\n    ");
    while(testnum<=0)
    {
        scanf("%d",&testnum);
        if(testnum<=0)
        {
            printf("*  ��ĿӦ����0,����������:                       *\n    ");
        }
    }
    printf("*  ��������Ŀ������ֵ�������(����0):            *\n    ");
    while(maxnum<=0)
    {
        scanf("%d",&maxnum);
        if(maxnum<=0)
        {
            printf("*  �����Ӧ����0,����������:                     *\n    ");
        }
    }
    printf("*  ������ÿ����ֵ����Ŀ(����1):                  *\n    ");
    while(digitnum<=1)
    {
        scanf("%d",&digitnum);
        if(digitnum<=1)
        {
            printf("*  ��ĿӦ����0,����������:                       *\n    ");
        }
    }
    int flag=1,symnum;
    printf("*  �����������(������1 2 -1,����+ -���������)*\n");
    printf("*  ��Ŷ�Ӧ����� 1:+,2:-,3:��,4:��; ����-1����  *\n    ");
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
    printf("*  �Ƿ�������(����1:��,0:û��):                  *\n    ");
    while(bracketflag<0)
    {
        scanf("%d",&bracketflag);
        if(bracketflag!=1 && bracketflag!=0)
        {
            printf("*  �������,����������:                          *\n    ");
        }
    }
    printf("*  �Ƿ���С��(����1:��,0:û��):                  *\n    ");
    while(littleflag<0)
    {
        scanf("%d",&littleflag);
        if(littleflag!=1 && littleflag!=0)
        {
            printf("*  �������,����������:                          *\n    ");
        }
    }
    printf("*  �Ƿ�д���ļ�(����1:д,0:��д):                *\n    ");
    while(fileflag<0)
    {
        scanf("%d",&fileflag);
        if(fileflag!=1 && fileflag!=0)
        {
            printf("*  �������,����������:                          *\n    ");
        }
    }
}
//*****������ɱ��ʽ����*****
int randdigit()//�����������
{
    int n;
    n=rand()%maxnum;
    return n;
}
float randlittle()//�������С��
{
    float n;
    n = rand()%maxnum + rand()%100*0.01;
    return n;
}
int randsymbol()//������������
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
int bracket_fun()//�ж���������λ��
{
    int n;
    n=rand()%(digitnum-1)-1;//�������Χ-1��digitnum-2
    return n;
}

//*****���ʽת�����沨��ʽ�����*****
void GetStr(char *str) //��ȡ���ʽ
{
    FILE *fp;
    int i=0;
    fp=fopen("D:test_question2.txt","rb");
    rewind(fp);//��λ�ļ�ָ��Ϊ��λ
    while ((str[i] = fgetc(fp)) != EOF)
    {
        if (str[i] == ' ')//ȥ������Ŀո�
        {
            i--;
        }
        i++;
    }
    str[i++] = '#';
    str[i] = '\0';
    fclose(fp);
}
void show_answer(char *str,double answer,int no) //д�𰸵��ļ�
{
    FILE *fp;
    fp=fopen("D:test_answer.txt","aw");
    fprintf(fp,"(%d) %.2f\n",no,answer);
    fclose(fp);
}
double CharToDouble(char *str)//���ַ���ת��Ϊ������
{
    double sumInt=0,sumDec=0,e=1;
    int i=0;
    while((str[i] >= '0')&&(str[i] <= '9'))//ʶ������
    {
        sumInt=sumInt*10 + str[i++]-'0';
    }
    if(str[i] == '.') //�����С��������С������
    {
        while(str[++i] != '\0')
        {
            sumDec=sumDec*10 + str[i]-'0';
            e *= 10;
        }
    }
    return sumInt + sumDec / e;
}
void Translate(char str[], char exp[]) //ת��Ϊ�沨�����ʽ
{
    char stack[MAXSIZE];
    int i=0, k=0, top=-1;
    while(str[i] != '#')
    {
        if(str[i] == '(')//ֱ�ӽ�'('��ջ
        {
            stack[++top]=str[i++];
        }
        else if(str[i] == ')')
        {
            while(top >= 0 && stack[top] != '(') //��'('֮ǰ�ķ��ų�ջ�����洢���沨�����ʽ
            {
                exp[k++]=stack[top--];
            }
            if(top < 0) //'('���㣬���ж����')'
            {
                puts("'('��ƥ��\n");
                exp[k++] = '\0';
                return ;
            }
            top--;//ȥ�� '('
            i++;
        }
        else if(str[i] == '+' || str[i] == '-')
        {
            while(top >= 0 && stack[top] != '(') //�����'('����'('֮ǰ�ķ��ų�ջ���������з��ų�ջ�����洢���沨�����ʽ
            {
                exp[k++]=stack[top--];
            }
            stack[++top]=str[i++]; //���µ����������ջ
        }
        else if(str[i] == '*' || str[i] == '/')
        {
            while(top >= 0 && (stack[top] == '*' || stack[top] == '/')) //��'*'��'/'֮ǰ�ķ��ų�ջ�����洢���沨�����ʽ
            {
                exp[k++]=stack[top--];
            }
            stack[++top]=str[i++]; //���µ����������ջ
        }
        else
        {
            while((str[i] >= '0' && str[i] <= '9') || str[i] == '.') //��������ֱ�Ӵ洢���沨�����ʽ
            {
                exp[k++]=str[i++];
            }
            exp[k++]='#'; //����һ���������������ţ��Ա���ȷ��ȡ������
        }
    }
    while(top >= 0) //��ջ������������Ŵ洢���沨�����ʽ
    {
        if(stack[top] == '(') //�ж����'('
        {
            puts("'('��ƥ��\n");
            exp[k++]='\0';
            return;
        }
        exp[k++]=stack[top--];
    }
    exp[k++]='\0';
}
double CompValue(char *exp) //���沨�����ʽ��ֵ
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
            i++;//����'#'
        }
        else
        {
            switch(exp[i++]) //����������ջ�����˳����������
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
            top--;//�˳����������
        }
    }
    return stack[top];
}

void create()//���ɱ��ʽ
{
    int nu1;
    float nu2;
    int i,sy;
    int bf=-2;//ƥ������
    int num1=testnum;
    int num2=digitnum;
    printf("*                  ������Ŀ����                  *\n");
    FILE *fp1=fopen("D:test_question.txt","w");
    FILE *fp2;
    while(num1>0)
    {
        fp2=fopen("D:test_question2.txt","w+");
        if(bracketflag==1)//�ж��Ƿ�������
        {
            bf=bracket_fun();
        }
        printf("*  (%d) ",testnum-num1+1);
        if(fileflag==1)
        {
            fprintf(fp1,"(%d) ",testnum-num1+1);
        }
        nu1=randdigit();//��ȡ�������
        nu2=randlittle();
        if(bf==-1)//������λ��
        {
            printf("(");
            fprintf(fp2,"( ");
        }
        if(littleflag==1)//��С��
        {
            printf("%.2f",nu2);
            fprintf(fp2,"%.2f ",nu2);
        }
        else
        {
            printf("%d",nu1);
            fprintf(fp2,"%d ",nu1);
        }
        if(fileflag==1)//д�ļ�ʱ
        {
            if(bf==-1)
            {
                fprintf(fp1,"(");
            }
            if(littleflag==1)//��С��
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
                    printf("��");
                    fprintf(fp2,"* ");
                    if(fileflag==1)
                    {
                        fprintf(fp1,"��");
                    }
                    break;
                case 4:
                    printf("��");
                    fprintf(fp2,"/ ");
                    if(fileflag==1)
                    {
                        fprintf(fp1,"��");
                    }
                    break;
            }
            nu1=randdigit();//��ȡ�������
            nu2=randlittle();
            if(i==bf)//������λ��
            {
                printf("(");
                fprintf(fp2,"( ");
            }
            if(littleflag==1)//��С��
            {
                printf("%.2f",nu2);
                fprintf(fp2,"%.2f ",nu2);
            }
            else
            {
                printf("%d",nu1);
                fprintf(fp2,"%d ",nu1);
            }
            if((i==0 && bf==-1) || (i==(bf+1)))//������λ��
            {
                printf(")");
                fprintf(fp2,")");
            }
            if(fileflag==1)//д�ļ�ʱ
            {
                if(i==bf)
                {
                    fprintf(fp1,"(");
                }
                if(littleflag==1)//��С��
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
        GetStr(str);//��ȡ���ʽ
        Translate(str,str2);//�沨��ʽ���
        answer[answer_count] = CompValue(str2);//�����������¼
        show_answer(str,answer[answer_count],(testnum-num1+1)); //�����
        answer_count++;
        num1--;//������ֵ����
        printf("=\n");
        if(fileflag==1)
        {
            fprintf(fp1,"=\n");
        }
    }
    fclose(fp1);
}

void print_answer()//��ӡ��
{
    printf("*                  ��Ŀ������                  *\n");
    for(int i=0;i<answer_count;i++)
    {
        printf("*  (%d) %.2f\n",i+1,answer[i]);
    }
    printf("*------------------------------------------------*\n");
}
int main()
{
    printf("**************************************************\n");
    printf("*        ��ӭʹ�����������Զ�����ϵͳ��          *\n");
    printf("*        �������밴��ϵͳ��ʾ���в���!           *\n");
    printf("*------------------------------------------------*\n");
    file_clear();//�ļ���ʼ��
    srand((int)time(0));//ÿ�β�����ͬ�������
    input_fun();//���������
    printf("*------------------------------------------------*\n");
    create();//���ɱ��ʽ
    printf("*------------------------------------------------*\n");
    print_answer();
    printf("*------------��л����ʹ��,ףѧϰ����!------------*\n");
    printf("**************************************************\n");
    return 0;
}
