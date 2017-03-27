#include<bits/stdc++.h>
using namespace std;
#define beginsize 8
#define LENGTH 3
typedef struct      // һ�ε�ַ
{
	int data[LENGTH];
}Indireone;
typedef struct      //���μ�ַ
{
    int one[LENGTH];
	Indireone *first[LENGTH];
}Indiretwo;

typedef struct      //���μ�ַ
{
    int two[LENGTH];
	Indiretwo * second[LENGTH];
}Indirethree;

typedef struct Node
{
	int begin[beginsize];       //ֱ�ӵ�ַ
	Indireone * one;
	int oneblock,twoblock,threeblock;
	Indiretwo * two;
	Indirethree * three;
}Mixtab;

typedef struct node
{
    string name;
    string time;
    int  type;//0�����ļ���,1�����ļ�.
    int blocksize;
    int startblock;
    struct node *brother;
    struct node *child;
    struct node *father;
    Mixtab *table;
    int index_list[200];
}FCB;

FCB *root;
FCB *present;
FCB *finding;//һЩ�����м����ҵ����Ǹ��ļ���Ŀ¼
string stringname="C:\\";
stack<int>sta;
struct block
{
    int b[10];
}blo[105];
int SUM;

int cal(int x)
{
    if(x<=beginsize) return x;
    else{
        int sum=x;
        x-=beginsize;
        if(x/LENGTH==0)  sum+=1;
        else if(x/LENGTH<=4) { x-=LENGTH;sum+=2+x/LENGTH+(x%LENGTH==0 ? 0:1);}
        else {x-=(LENGTH*4);int t=(x/LENGTH)+(x%LENGTH==0 ? 0:1);sum+=6+t+(t/LENGTH)+(t%LENGTH==0? 0: 1);}
        return sum;
    }
}

void showsystem()
{
   cout<<"Microsoft Windows [�汾 10.0.10586]\n(c) 2015 Microsoft Corporation����������Ȩ����"<<endl<<endl;
}

char* get_time()
{
    time_t t=time(0);
    char ti[1000];
    strftime(ti,sizeof(ti),"%Y/%m/%d  %X",localtime(&t));
    return ti;
}

void recover_block(int x)
{
    if(sta.size()<10){
        sta.push(x);
    }
    else{
        for(int i=9;i>=0;i--){
            int t=sta.top();sta.pop();
            blo[x].b[i]=t;
        }
        sta.push(x);
    }
}

int distribute_block()
{
    if(sta.size()==1){
        int x=sta.top();sta.pop();
        for(int i=0;i<10;i++){
            sta.push(blo[x].b[i]);
        }
        return x;
    }
    else{
        int x=sta.top();sta.pop();
        return x;
    }
}

void all_init()
{
    root=new FCB;
    root->name="C:";
    root->brother=NULL;root->child=NULL;
    root->time=get_time();//cout<<root->time<<endl;
    root->table=NULL;
    present=root;
    sta.empty();
    for(int i=0;i<=100;i++){
        for(int j=0;j<10;j++){
            blo[i].b[j]=-1;
        }
    }
    for(int i=99;i>=0;i--){
        recover_block(i);
    }
    SUM=100;
    /*
    while(!sta.empty()){��ʾ����Ŀ���ջ��Ŀ��
        int x=sta.top();
        cout<<x<<endl;
        sta.pop();
    }*/
}

bool check(string name,int type)
{
    finding=present->child;
    while(finding){
        if(finding->name==name&&finding->type==type) return true;
        finding=finding->brother;
    }
    return false;
}

void add_point(FCB *f)
{
    if(present->child==NULL){
        present->child=f;
    }
    else{
       FCB *t=present->child;
       while(t->brother!=NULL) t=t->brother;
       t->brother=f;
    }
}

void Md(string name)
{
    if(check(name,0)||check(name,1)){
       cout<<"��Ŀ¼���ļ�"<<name<<"�Ѵ��ڡ�"<<endl;
    }
    else{
       FCB *f=new FCB;
       f->type=0;
       f->name=name;
       //cout<<"file name:"<<name<<endl;
       f->blocksize=0;
       f->time=get_time();
       f->father=present;
       f->brother=NULL;
       f->child=NULL;
       add_point(f);
    }
}

void Cd(string name)
{
   if(name=="."){
   }
   else if(name==".."){
       if(present==root) return;
       int l1=present->name.size(),l2=stringname.size();
       stringname.assign(stringname,0,l2-l1-1);
       present=present->father;
   }
   else if(name=="\\"||name=="/"){
       present=root;
       stringname="C:\\";
   }
   else if(check(name,1)){
       cout<<"Ŀ¼������Ч��"<<endl;
   }
   else if(!check(name,0)){
       cout<<"ϵͳ�Ҳ���ָ����·����"<<endl;
   }
   else{
       FCB *t=present->child;
       while(t->name!=name) t=t->brother;
       if(present==root) stringname+=(t->name);
       else stringname+=("\\"+t->name);
       present=t;
   }
}

void Looktree(FCB * f)
{
	int a[100];
	int n=-1;
	FCB * temp;
	if (f->father==present)
	{
		if (f->brother==NULL)
			cout<<"����"<<f->name<<endl;
		else
			cout<<"����"<<f->name<<endl;
	}
	else
	{
		temp=f->father;
		while(temp!=present)
		{
			n++;
			if(temp->brother==NULL)
			{//��0ʱ������ո�else�������
				a[n]=0;
			}
			else a[n]=1;
			temp=temp->father;
		}
		//��ס��n
		for(int i=n;i>=0;i--)
		{
			if(a[i]==1)
				cout<<"��  ";
			else cout<<"    ";
		}
		if(f->brother==NULL)
			cout<<"����"<<f->name<<endl;
		else
			cout<<"����"<<f->name<<endl;
	}
}


void Treeview(FCB * f)//���νṹ�鿴
{//�鿴����Ŀ¼
	FCB * temp,* temp2;
	if(present->child==NULL)
		cout<<"û�����ļ���"<<endl;
	else
	{
		if(f==NULL) return;
		else Looktree(f);
		if(f->child!=NULL) Treeview(f->child);
        if(f->brother!=NULL) Treeview(f->brother);
	}
}
/*
Mixtab* create_node(int x,int *a)
{
    Mixtab *M=new Mixtab;
    M->one=NULL;M->two=NULL;M->three=NULL;
    if(x<=beginsize){
       for(int i=0;i<x;i++) M->begin[i]=a[i];
    }
    else if(x<=beginsize+LENGTH){
       for(int i=0;i<beginsize;i++) M->begin[i]=a[i];
       int sum=x-beginsize;
       M->one=new Indireone;
       for(int i=0;i<sum;i++) M->one->data[i]=a[beginsize+i];
    }
    else if(x<=beginsize+LENGTH*4){
       //system("PAUSE");
       for(int i=0;i<beginsize;i++) M->begin[i]=a[i];
       int sum=x-beginsize;
       M->one=new Indireone;
       for(int i=0;i<LENGTH;i++) M->one->data[i]=a[beginsize+i];
       M->two=new Indiretwo;
       sum-=3;
       if(sum<=LENGTH){
           M->two->first[0]=new Indireone;M->two->first[1]=NULL;M->two->first[2]=NULL;
           for(int i=0;i<sum;i++) M->two->first[0]->data[i]=a[beginsize+LENGTH+i];
       }
       else if(sum<=LENGTH*2){
           M->two->first[0]=new Indireone;M->two->first[1]=new Indireone;M->two->first[2]=NULL;
           for(int i=0;i<3;i++) M->two->first[0]->data[i]=a[beginsize+LENGTH+i];
           sum-=3;
           for(int i=0;i<sum;i++) M->two->first[1]->data[i]=a[beginsize+LENGTH*2+i];
       }
       else{
           M->two->first[0]=new Indireone;M->two->first[1]=new Indireone;M->two->first[2]=new Indireone;
           for(int i=0;i<3;i++) M->two->first[0]->data[i]=a[beginsize+LENGTH+i];
           for(int i=0;i<3;i++) M->two->first[1]->data[i]=a[beginsize+LENGTH*2+i];
           sum-=6;
           for(int i=0;i<sum;i++) M->two->first[2]->data[i]=a[beginsize+LENGTH*3+i];
       }
    }
    else{
          for(int i=0;i<beginsize;i++) M->begin[i]=a[i];
          M->one=new Indireone;
          for(int i=0;i<LENGTH;i++) M->one->data[i]=a[beginsize+i];
          M->two=new Indiretwo;
          for(int i=0;i<LENGTH;i++) M->two->first[i]=new Indireone;
          for(int i=0;i<LENGTH;i++){
             for(int j=0;j<LENGTH;j++){
                M->two->first[i]->data[j]=a[beginsize+LENGTH+i*LENGTH+j];
             }
          }
          int sum=x-beginsize-LENGTH*4;
          if(sum<=9){
             M->three=new Indirethree;
             M->three->second[0]=new Indiretwo;
             if(sum<=3){
                M->three->second[0]->first[0]=new Indireone;
                for(int i=0;i<sum;i++) M->three->second[0]->first[0]->data[i]=a[beginsize+LENGTH*4+i];
             }
             else if(sum<=6){
                M->three->second[0]->first[0]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[0]->first[0]->data[i]=a[beginsize+LENGTH*4+i];
                M->three->second[0]->first[1]=new Indireone;
                sum-=3;
                for(int i=0;i<sum;i++) M->three->second[0]->first[1]->data[i]=a[beginsize+LENGTH*5+i];
             }
             else{
                M->three->second[0]->first[0]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[0]->first[0]->data[i]=a[beginsize+LENGTH*4+i];
                M->three->second[0]->first[1]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[0]->first[1]->data[i]=a[beginsize+LENGTH*5+i];
                M->three->second[0]->first[2]=new Indireone;
                sum-=6;
                for(int i=0;i<sum;i++) M->three->second[0]->first[2]->data[i]=a[beginsize+LENGTH*6+i];
             }
          }
          else if(sum<=18){
             M->three=new Indirethree;
             M->three->second[0]=new Indiretwo;
             for(int i=0;i<LENGTH;i++) M->three->second[0]->first[i]=new Indireone;
             for(int i=0;i<LENGTH;i++){
                for(int j=0;j<LENGTH;j++){
                    M->three->second[0]->first[i]->data[j]=a[beginsize+LENGTH*4+i*LENGTH+j];
                }
             }
             M->three->second[1]=new Indiretwo;
             sum-=9;
             if(sum<=3){
                 M->three->second[1]->first[0]=new Indireone;
                for(int i=0;i<sum;i++) M->three->second[1]->first[0]->data[i]=a[beginsize+LENGTH*7+i];
             }
             else if(sum<=6){
                M->three->second[1]->first[0]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[1]->first[0]->data[i]=a[beginsize+LENGTH*7+i];
                M->three->second[1]->first[1]=new Indireone;
                sum-=3;
                for(int i=0;i<sum;i++) M->three->second[1]->first[1]->data[i]=a[beginsize+LENGTH*8+i];
             }
             else{
                M->three->second[1]->first[0]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[1]->first[0]->data[i]=a[beginsize+LENGTH*7+i];
                M->three->second[1]->first[1]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[1]->first[1]->data[i]=a[beginsize+LENGTH*8+i];
                M->three->second[1]->first[2]=new Indireone;
                sum-=6;
                for(int i=0;i<sum;i++) M->three->second[1]->first[2]->data[i]=a[beginsize+LENGTH*9+i];
             }
          }
          else{
             M->three=new Indirethree;
             M->three->second[0]=new Indiretwo;
             M->three->second[1]=new Indiretwo;
             for(int i=0;i<LENGTH;i++) M->three->second[0]->first[i]=new Indireone;
             for(int i=0;i<LENGTH;i++) M->three->second[1]->first[i]=new Indireone;
             for(int i=0;i<LENGTH;i++){
                for(int j=0;j<LENGTH;j++){
                    M->three->second[0]->first[i]->data[j]=a[beginsize+LENGTH*4+i*LENGTH+j];
                }
             }
             for(int i=0;i<LENGTH;i++){
                for(int j=0;j<LENGTH;j++){
                    M->three->second[1]->first[i]->data[j]=a[beginsize+LENGTH*7+i*LENGTH+j];
                }
             }
             sum-=18;
             M->three->second[2]=new Indiretwo;
             if(sum<=3){
                M->three->second[2]->first[0]=new Indireone;
                for(int i=0;i<sum;i++) M->three->second[2]->first[0]->data[i]=a[beginsize+LENGTH*10+i];
             }
             else if(sum<=6){
                M->three->second[2]->first[0]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[2]->first[0]->data[i]=a[beginsize+LENGTH*10+i];
                M->three->second[2]->first[1]=new Indireone;
                sum-=3;
                for(int i=0;i<sum;i++) M->three->second[2]->first[1]->data[i]=a[beginsize+LENGTH*11+i];
             }
             else{
                 M->three->second[2]->first[0]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[2]->first[0]->data[i]=a[beginsize+LENGTH*10+i];
                M->three->second[2]->first[1]=new Indireone;
                for(int i=0;i<3;i++) M->three->second[2]->first[1]->data[i]=a[beginsize+LENGTH*11+i];
                M->three->second[2]->first[2]=new Indireone;
                sum-=6;
                for(int i=0;i<sum;i++) M->three->second[2]->first[2]->data[i]=a[beginsize+LENGTH*12+i];
             }
          }
    }
    return M;
}
*/
void Mk(string name,string Size)
{
   int size=0;
   for(int i=0;i<Size.size();i++) size=size*10+Size[i]-'0';
   if(check(name,1)||check(name,0)){
      cout<<"��Ŀ¼���ļ�"<<name<<"�Ѵ��ڡ�"<<endl;
   }
   else if(cal(size)>SUM){
      cout<<"������ʣ���ڴ治�㣬������һ�����ļ����ٳ��ԡ�"<<endl;
   }
   else if(size>beginsize+LENGTH+LENGTH*LENGTH+LENGTH*LENGTH*LENGTH){
      cout<<"��Ǹ,�����ļ�������������֧����������,����ϵͳ��Ĭ������ļ��ڴ档"<<endl;
   }
   else{
       size=cal(size);
       SUM-=size;
       FCB *f=new FCB;
       f->name=name;
       f->type=1;
       f->blocksize=size;
       f->time=get_time();
       //cout<<"begin:";
       for(int i=0;i<size;i++){
          f->index_list[i]=distribute_block();
         // cout<<f->index_list[i]<<" ";
       }
       //cout<<endl;
       f->startblock=f->index_list[0];
       //f->table=create_node(f->blocksize,temp);
       f->father=present;
       f->brother=NULL;
       f->child=NULL;
       add_point(f);
   }
}

bool Check(string s)
{
    for(int i=0;i<s.size();i++){
        if(s[i]=='?'||s[i]=='*') return false;
    }
    return true;
}

void show_dir(FCB *t)
{
    cout<<" ������ C �еľ��� ϵͳ\n ������к��� 0002-D884\n"<<endl;
    cout<<" "<<stringname<<" ��Ŀ¼"<<endl<<endl;
    cout<<t->time<<"      <DIR>         ."<<endl;
    cout<<t->time<<"      <DIR>         .."<<endl;
    FCB *f=t->child;
    int ft=0,mt=0,fmt=0;
    while(f){
        if(f->type==0) mt++;
        if(f->type==1) ft++,fmt+=f->blocksize;
        if(f->type==0){
            if(f->child!=NULL)  cout<<f->time<<"      <DIR>         "<<f->name<<endl;
            else  cout<<f->time<<"                    "<<f->name<<endl;
        }
        else cout<<f->time<<"                 "<<f->blocksize<<" "<<f->name<<endl;
        f=f->brother;
    }
    cout<<"            "<<ft<<"���ļ�"<<"        "<<fmt<<"�ֽ�"<<endl;
    cout<<"            "<<mt<<"��Ŀ¼"<<"        "<<beginsize+LENGTH*13-fmt<<"�����ֽ�"<<endl;
}

bool check1(string name)
{
   for(int i=0;i<name.size();i++){
      if(name[i]=='*') return true;
   }
   return false;
}

bool check2(string f,string s)
{
   for(int i=0;i<s.size();i++){
       if(s[i]!=f[i]) return false;
   }
   return true;
}

bool check3(string f,string s)
{
   for(int i=0;i<s.size();i++){
     if(f[f.size()-1-i]!=s[i]) return false;
   }
   return true;
}

void Dir(string name)
{
    if(name==""){
        show_dir(present);
    }
    else if(Check(name)){
       FCB *f=present->child;
       if(!check(name,0)&&!check(name,1)){
           cout<<" ������ C �еľ��� ϵͳ\n ������к��� 0002-D884\n"<<endl;
           cout<<" "<<stringname<<" ��Ŀ¼"<<endl<<endl;
           cout<<"�Ҳ����ļ�"<<endl;
           return;
       }
       while(f->name!=name) f=f->brother;
       if(f->type==0) show_dir(f);
       else if(f->type==1){
          cout<<f->time<<"                 "<<f->blocksize<<" "<<f->name<<endl;
          cout<<"            1"<<"���ļ�"<<"        "<<f->blocksize<<"�ֽ�"<<endl;
          cout<<"            0"<<"��Ŀ¼"<<"        "<<beginsize+LENGTH*13-f->blocksize<<"�����ֽ�"<<endl;
       }
    }
    else{
       if(check1(name)){
           if(name[0]!='*'){
               string pre="";
               for(int i=0;name[i]!='*';i++) pre+=name[i];
               cout<<" ������ C �еľ��� ϵͳ\n ������к��� 0002-D884\n"<<endl;
               cout<<" "<<stringname<<" ��Ŀ¼"<<endl<<endl;
               FCB *f=present->child;
               int fmt=0,ft=0,mt=0;
               while(f){
                  if(check2(f->name,pre)){
                     if(f->type==0){
                       if(f->child==NULL)   cout<<f->time<<"                    "<<f->name<<endl;
                       else cout<<f->time<<"      <DIR>         "<<f->name<<endl;
                       mt++;
                      }
                      else  cout<<f->time<<"                 "<<f->blocksize<<" "<<f->name<<endl,ft++,fmt+=f->blocksize;
                   }
                   f=f->brother;
               }
               cout<<"            "<<ft<<"���ļ�"<<"        "<<fmt<<"�ֽ�"<<endl;
               cout<<"            "<<mt<<"��Ŀ¼"<<"        "<<beginsize+LENGTH*13-fmt<<"�����ֽ�"<<endl;
           }
           else{
              string postfix="";
              for(int i=name.size()-1;name[i]!='*';i--) postfix+=name[i];
              cout<<" ������ C �еľ��� ϵͳ\n ������к��� 0002-D884\n"<<endl;
              cout<<" "<<stringname<<" ��Ŀ¼"<<endl<<endl;
              FCB *f=present->child;
              int fmt=0,ft=0,mt=0;
              while(f){
                  if(check3(f->name,postfix)){
                     if(f->type==0){
                       if(f->child==NULL)   cout<<f->time<<"                    "<<f->name<<endl;
                       else cout<<f->time<<"      <DIR>         "<<f->name<<endl;
                       mt++;
                      }
                      else  cout<<f->time<<"                 "<<f->blocksize<<" "<<f->name<<endl,ft++,fmt+=f->blocksize;
                   }
                   f=f->brother;
               }
               cout<<"            "<<ft<<"���ļ�"<<"        "<<fmt<<"�ֽ�"<<endl;
               cout<<"            "<<mt<<"��Ŀ¼"<<"        "<<beginsize+LENGTH*13-fmt<<"�����ֽ�"<<endl;
           }
       }
       else{

       }
    }
}

void Format(string name)
{
   if(present->child==NULL){
        cout<<"��ǰ���ļ��л�Ŀ¼·���£�û���ļ���Ϣ��"<<endl;
        return;
    }
    if(!check(name,1)){
        cout<<"�ļ�������Ч��"<<endl;
        return;
    }
    FCB *f=present->child;
    while(!(f->name==name&&f->type==1)) f=f->brother;
    cout<<"��ǰ�ļ��� ��" <<f->name<<endl;
    cout<<"�ļ�������ʱ�䣺"<<f->time<<endl;
    cout<<"�ļ���С��"<<f->blocksize<<endl;
    cout<<"ռ�ݴ��̿飺"<<f->blocksize<<endl;
    cout<<"�ļ���ʼλ��: "<<f->startblock<<endl;
    int num=f->blocksize,t=0;
    cout<<"ֱ�ӵ�ַ: ";
    for(int i=t;i<min(beginsize,num-t);i++) cout<<f->index_list[i]<<" "; cout<<endl;
    t+=beginsize; if(num<=t) return;
    cout<<"��һ�������Ĵ��̿�ţ�"; cout<<f->index_list[t++]; cout<<endl;
    cout<<"һ����ַ: "; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" "; cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"����������Ĵ��̿��: "; cout<<f->index_list[t++]<<endl;
    cout<<"������������һ��һ�������Ĵ��̿��:"; cout<<f->index_list[t++]<<endl;
    cout<<"������������һ��һ����ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" "; cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"�����������ڶ���һ�������Ĵ��̿��:"; cout<<f->index_list[t++]<<endl;
    cout<<"�����������ڶ���һ����ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" "; cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"����������������һ���������Ĵ��̿��:"; cout<<f->index_list[t++]<<endl;
    cout<<"����������������һ������ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" "; cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"�����������Ĵ��̿��:";  cout<<f->index_list[t++]<<endl;
    cout<<"������������ĵ�һ������������Ĵ��̿��:";cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ�һ������������ĵ�һ��һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ�һ������������ĵ�һ��һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"��������������ĵ�һ������������ĵڶ���һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ�һ������������ĵڶ���һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"��������������ĵ�һ������������ĵ�����һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ�һ������������ĵ�����һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"������������ĵڶ�������������Ĵ��̿��:";cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵڶ�������������ĵ�һ��һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵڶ�������������ĵ�һ��һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"��������������ĵڶ�������������ĵڶ���һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵڶ�������������ĵڶ���һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"��������������ĵڶ�������������ĵ�����һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵڶ�������������ĵ�����һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"������������ĵ���������������Ĵ��̿��:";cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ���������������ĵ�һ��һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ���������������ĵ�һ��һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"��������������ĵ���������������ĵڶ���һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ���������������ĵڶ���һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
    cout<<"��������������ĵ���������������ĵ�����һ��������Ĵ��̿�ţ�"; cout<<f->index_list[t++]<<endl;
    cout<<"��������������ĵ���������������ĵ�����һ�������µĵ�ַ:"; for(int i=t;i<min(num,t+LENGTH);i++) cout<<f->index_list[i]<<" ";cout<<endl;
    t+=LENGTH; if(num<=t) return;
}
/*
void Vol(string name)
{
    if(present->child==NULL){
        cout<<"��ǰ���ļ��л�Ŀ¼·���£�û���ļ���Ϣ��"<<endl;
        return;
    }
    if(!check(name,1)){
        cout<<"�ļ�������Ч��"<<endl;
        return;
    }
    FCB *f=present->child;
    while(!(f->name==name&&f->type==1)) f=f->brother;
    cout<<"��ǰ�ļ��� ��" <<f->name<<endl;
    cout<<"�ļ�������ʱ�䣺"<<f->time<<endl;
    cout<<"�ļ���С��"<<f->blocksize<<endl;
    cout<<"ռ�ݴ��̿飺"<<f->blocksize<<endl;
    cout<<"�ļ���ʼλ��: "<<f->startblock<<endl;
    int num=f->blocksize;
    if(num<=beginsize){
        cout<<"ֱ�ӵ�ַ:";
        for(int i=0;i<f->blocksize;i++) cout<<f->table->begin[i]<<" ";
        cout<<endl;
    }
    else if(num<=beginsize+LENGTH){
        cout<<"ֱ�ӵ�ַ: "; for(int i=0;i<beginsize;i++) cout<<f->table->begin[i]<<" "; cout<<endl;
        num-=beginsize;
        cout<<"һ�μ�ַ: "; for(int i=0;i<num;i++) cout<<f->table->one->data[i]<<" ";cout<<endl;
    }
    else if(num<=beginsize+LENGTH*4){
        cout<<"ֱ�ӵ�ַ: "; for(int i=0;i<beginsize;i++) cout<<f->table->begin[i]<<" "; cout<<endl;
        cout<<"һ�μ�ַ: "; for(int i=0;i<3;i++) cout<<f->table->one->data[i]<<" ";cout<<endl;
        num-=(beginsize+LENGTH);
        if(num<=3){
            cout<<"���μ�ַ: ";for(int i=0;i<num;i++) cout<<f->table->two->first[0]->data[i]<<" "; cout<<endl;
        }
        else if(num<=6){
           cout<<"���μ�ַ: ";
           for(int i=0;i<3;i++) cout<<f->table->two->first[0]->data[i]<<" ";
           num-=3;
           for(int i=0;i<num;i++) cout<<f->table->two->first[1]->data[i]<<" ";
           cout<<endl;
        }
        else{
           cout<<"���μ�ַ: ";
           for(int i=0;i<3;i++) cout<<f->table->two->first[0]->data[i]<<" ";
           for(int i=0;i<3;i++) cout<<f->table->two->first[1]->data[i]<<" ";
           num-=6;
           for(int i=0;i<num;i++) cout<<f->table->two->first[2]->data[i]<<" ";
           cout<<endl;
        }
    }
    else{
        cout<<"ֱ�ӵ�ַ: "; for(int i=0;i<beginsize;i++) cout<<f->table->begin[i]<<" "; cout<<endl;
        cout<<"һ�μ�ַ: "; for(int i=0;i<3;i++) cout<<f->table->one->data[i]<<" ";cout<<endl;
        cout<<"���μ�ַ: ";
        for(int i=0;i<3;i++) cout<<f->table->two->first[0]->data[i]<<" ";
        for(int i=0;i<3;i++) cout<<f->table->two->first[1]->data[i]<<" ";
        for(int i=0;i<3;i++) cout<<f->table->two->first[2]->data[i]<<" ";
        cout<<endl;
        num-=(beginsize+LENGTH*4);
        if(num<=9){
            if(num<=3){
                cout<<"���μ�ַ: ";for(int i=0;i<num;i++) cout<<f->table->three->second[0]->first[0]->data[i]<<" ";cout<<endl;
            }
            else if(num<=6){
                cout<<"���μ�ַ: ";
                for(int i=0;i<3;i++) cout<<f->table->three->second[0]->first[0]->data[i]<<" ";
                num-=3;
                for(int i=0;i<num;i++) cout<<f->table->three->second[0]->first[1]->data[i]<<" ";
                cout<<endl;
            }
            else{
                cout<<"���μ�ַ: ";
                for(int i=0;i<3;i++) cout<<f->table->three->second[0]->first[0]->data[i]<<" ";
                for(int i=0;i<3;i++) cout<<f->table->three->second[0]->first[1]->data[i]<<" ";
                num-=6;
                for(int i=0;i<num;i++) cout<<f->table->three->second[0]->first[2]->data[i]<<" ";
                cout<<endl;
            }
        }
        else if(num<=18){
             cout<<"���μ�ַ: ";
             for(int i=0;i<LENGTH;i++){
             for(int j=0;j<LENGTH;j++){
                cout<<f->table->three->second[0]->first[i]->data[j]<<" ";
              }
            }
            num-=9;
            if(num<=3){
                for(int i=0;i<num;i++) cout<<f->table->three->second[1]->first[0]->data[i]<<" ";cout<<endl;
            }
            else if(num<=6){
                for(int i=0;i<3;i++) cout<<f->table->three->second[1]->first[0]->data[i]<<" ";
                num-=3;
                for(int i=0;i<num;i++) cout<<f->table->three->second[1]->first[1]->data[i]<<" ";cout<<endl;
            }
            else{
                for(int i=0;i<3;i++) cout<<f->table->three->second[1]->first[0]->data[i]<<" ";
                for(int i=0;i<3;i++) cout<<f->table->three->second[1]->first[1]->data[i]<<" ";
                num-=6;
                for(int i=0;i<num;i++) cout<<f->table->three->second[1]->first[2]->data[i]<<" ";cout<<endl;
            }
        }
        else{
           cout<<"���μ�ַ: ";
           for(int i=0;i<LENGTH;i++){
             for(int j=0;j<LENGTH;j++){
                cout<<f->table->three->second[0]->first[i]->data[j]<<" ";
              }
            }
            for(int i=0;i<LENGTH;i++){
             for(int j=0;j<LENGTH;j++){
                cout<<f->table->three->second[1]->first[i]->data[j]<<" ";
              }
            }
            num-=18;
            if(num<=3){
                for(int i=0;i<num;i++) cout<<f->table->three->second[2]->first[0]->data[i]<<" ";cout<<endl;
            }
            else if(num<=6){
                for(int i=0;i<3;i++) cout<<f->table->three->second[2]->first[0]->data[i]<<" ";
                num-=3;
                for(int i=0;i<num;i++) cout<<f->table->three->second[2]->first[1]->data[i]<<" ";cout<<endl;
            }
            else{
                for(int i=0;i<3;i++) cout<<f->table->three->second[2]->first[0]->data[i]<<" ";
                for(int i=0;i<3;i++) cout<<f->table->three->second[2]->first[1]->data[i]<<" ";
                num-=6;
                for(int i=0;i<num;i++) cout<<f->table->three->second[2]->first[2]->data[i]<<" ";cout<<endl;
            }
        }
    }
}
*/

void Rd(string name)
{
    if(!check(name,0)){
        cout<<"ϵͳ�Ҳ���ָ����·����"<<endl;
        return;
    }
    else {
       FCB *t=present->child,*p=t;
       while(!(t->name==name&&t->type==0)) { p=t;t=t->brother;}
       if(t->child!=NULL){
           cout<<"Ŀ¼���ǿյġ�"<<endl;
           return;
       }
       if(present->child==t){
          present->child=t->brother;
          delete t;
       }
       else{
          p->brother=t->brother;
          delete t;
       }
    }
}
/*
void recover_f(FCB *f)
{
    int x=f->blocksize;
    int temp[x+1];
    Mixtab *M=f->table;
    if(x<=beginsize){
         for(int i=0;i<x;i++) temp[i]=M->begin[i];
    }
    else if(x<=beginsize+LENGTH){
       for(int i=0;i<beginsize;i++) temp[i]=M->begin[i];
       int sum=x-beginsize;
       for(int i=0;i<sum;i++) temp[beginsize+i]=M->one->data[i];
    }
    else if(x<=beginsize+LENGTH*4){
       for(int i=0;i<beginsize;i++) temp[i]=M->begin[i];
       int sum=x-beginsize;
       for(int i=0;i<LENGTH;i++) temp[beginsize+i]=M->one->data[i];
       sum-=3;
       if(sum<=LENGTH){
           for(int i=0;i<sum;i++) temp[beginsize+LENGTH+i]=M->two->first[0]->data[i];
       }
       else if(sum<=LENGTH*2){
           for(int i=0;i<3;i++) temp[beginsize+LENGTH+i]=M->two->first[0]->data[i];
           sum-=3;
           for(int i=0;i<sum;i++) temp[beginsize+LENGTH*2+i]=M->two->first[1]->data[i];
       }
       else{
           for(int i=0;i<3;i++) temp[beginsize+LENGTH+i]=M->two->first[0]->data[i];
           for(int i=0;i<3;i++) temp[beginsize+LENGTH*2+i]=M->two->first[1]->data[i];
           sum-=6;
           for(int i=0;i<sum;i++) temp[beginsize+LENGTH*3+i]=M->two->first[2]->data[i];
       }
    }
    else{
          for(int i=0;i<beginsize;i++) temp[i]=M->begin[i];
          for(int i=0;i<LENGTH;i++) temp[beginsize+i]=M->one->data[i];
          for(int i=0;i<LENGTH;i++){
             for(int j=0;j<LENGTH;j++){
                temp[beginsize+LENGTH+i*LENGTH+j]=M->two->first[i]->data[j];
             }
          }
          int sum=x-beginsize-LENGTH*4;
          if(sum<=9){
             if(sum<=3){
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*4+i]=M->three->second[0]->first[0]->data[i];
             }
             else if(sum<=6){
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*4+i]=M->three->second[0]->first[0]->data[i];
                sum-=3;
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*5+i]=M->three->second[0]->first[1]->data[i];
             }
             else{
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*4+i]=M->three->second[0]->first[0]->data[i];
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*5+i]=M->three->second[0]->first[1]->data[i];
                sum-=6;
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*6+i]=M->three->second[0]->first[2]->data[i];
             }
          }
          else if(sum<=18){
             for(int i=0;i<LENGTH;i++){
                for(int j=0;j<LENGTH;j++){
                    temp[beginsize+LENGTH*4+i*LENGTH+j]=M->three->second[0]->first[i]->data[j];
                }
             }
             sum-=9;
             if(sum<=3){
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*7+i]=M->three->second[1]->first[0]->data[i];
             }
             else if(sum<=6){
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*7+i]=M->three->second[1]->first[0]->data[i];
                sum-=3;
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*8+i]=M->three->second[1]->first[1]->data[i];
             }
             else{
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*7+i]=M->three->second[1]->first[0]->data[i];
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*8+i]=M->three->second[1]->first[1]->data[i];
                sum-=6;
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*9+i]=M->three->second[1]->first[2]->data[i];
             }
          }
          else{
             for(int i=0;i<LENGTH;i++){
                for(int j=0;j<LENGTH;j++){
                    temp[beginsize+LENGTH*4+i*LENGTH+j]=M->three->second[0]->first[i]->data[j];
                }
             }
             for(int i=0;i<LENGTH;i++){
                for(int j=0;j<LENGTH;j++){
                    temp[beginsize+LENGTH*7+i*LENGTH+j]=M->three->second[1]->first[i]->data[j];
                }
             }
             sum-=18;
             if(sum<=3){
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*10+i]=M->three->second[2]->first[0]->data[i];
             }
             else if(sum<=6){
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*10+i]=M->three->second[2]->first[0]->data[i];
                sum-=3;
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*11+i]=M->three->second[2]->first[1]->data[i];
             }
             else{
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*10+i]=M->three->second[2]->first[0]->data[i];
                for(int i=0;i<3;i++) temp[beginsize+LENGTH*11+i]=M->three->second[2]->first[1]->data[i];
                sum-=6;
                for(int i=0;i<sum;i++) temp[beginsize+LENGTH*12+i]=M->three->second[2]->first[2]->data[i];
             }
          }
    }
    for(int i=0;i<f->blocksize;i++) recover_block(temp[i]);
}
*/
void recover_f(FCB *f)
{
    for(int i=0;i<f->blocksize;i++){
        recover_block(f->index_list[i]);
    }
}

void delete_file(string name)
{
    FCB *t=present->child,*p=t;
    while(!(t->name==name&&t->type==1)) { p=t;t=t->brother;}
    if(present->child==t){
        present->child=t->brother;
        recover_f(t);
        delete t;
    }
    else{
        p->brother=t->brother;
        recover_f(t);
        delete t;
    }
}

void Del(string name)
{
    if(Check(name)){
        if(!check(name,1)){
           cout<<"ϵͳ�Ҳ���ָ����·����"<<endl;
           return;
        }
        else{
           delete_file(name);
        }
    }
    else if(check1(name)){
      if(name[0]!='*'){
          string pre="";
          for(int i=0;name[i]!='*';i++) pre+=name[i];
          FCB *f=present->child;
          //cout<<"pre="<<pre<<endl;
          while(f){
            if(check2(f->name,pre)&&f->type==1) delete_file(f->name);
            f=f->brother;
          }
      }
      else{
          string postfix="";
          for(int i=name.size()-1;name[i]!='*';i--) postfix+=name[i];
          FCB *f=present->child;
          while(f){
            if(check3(f->name,postfix)&&f->type==1) delete_file(f->name);
            f=f->brother;
          }
      }
    }
    else{

    }
}

void show_help()
{
    //cout<<"�й�ĳ���������ϸ��Ϣ������� HELP ������"<<endl;
    cout<<"MD           ����һ��Ŀ¼��"<<endl;
    cout<<"MK           ����һ���ļ���"<<endl;
    cout<<"DIR          ��ʾһ��Ŀ¼���ļ�����Ŀ¼��"<<endl;
    cout<<"CD           ��ʾ��ǰĿ¼�����ƻ�����ġ�"<<endl;
    cout<<"RD           ɾ��Ŀ¼��"<<endl;
    cout<<"DEL          ɾ������һ���ļ���"<<endl;
    cout<<"TREE         ��ͼ�η�ʽ��ʾ���������·����Ŀ¼�ṹ��"<<endl;
    cout<<"FORMAT       ��ʾ�ļ���ռ�д��̵Ŀ�š�"<<endl;
    cout<<"CLS          �����Ļ��"<<endl;
    cout<<"TIME         ��ʾϵͳʱ�䡣"<<endl;
    cout<<"HELP         �ṩWindows�����кͰ�����Ϣ��"<<endl;
    cout<<"EXIT         �˳������г���"<<endl;
}

int main()
{
    showsystem();
    all_init();
    while(1){
      cout<<stringname<<">";
      string str,cmd="",c1,c2;
      getline(cin,str);
      int i=0;
      for(;i<str.size()&&str[i]!=' ';i++) cmd+=str[i];
      if(i==str.size()-1) c1="";
      else{
        while(str[i]==' ') i++;
        for(;i<str.size()&&str[i]!=' ';i++) c1+=str[i];
        if(i==str.size()-1) c2="";
        else{
            while(str[i]==' ') i++;
            for(;i<str.size();i++) c2+=str[i];
        }
      }
      if(cmd=="md"){
           Md(c1);
      }
      else if(cmd=="mk"){
           if(c1==""||c2==""){
              cout<<"'"<<str<<"' �����ڲ����ⲿ���Ҳ���ǿ����еĳ�����������ļ���"<<endl<<endl;
              continue;
           }
           Mk(c1,c2);
      }
      else if(cmd=="dir"){
           Dir(c1);
      }
      else if(cmd=="cd"){
           Cd(c1);
      }
      else if(cmd=="rd"){
           Rd(c1);
      }
      else if(cmd=="del"){
           Del(c1);
      }
      else if(cmd=="tree"){
          Treeview(present->child);
      }
      else if(cmd=="format"){
          Format(c1);
      }
      else if(cmd=="cls"){
         system("cls");
         //showsystem();
      }
      else if(cmd=="help"){
         show_help();
      }
      else if(cmd=="time"){
         cout<<"��ǰʱ��:"<<get_time()<<endl;
      }
      else if(cmd=="exit"){
         return 0;
      }
      else{
         cout<<"'"<<cmd<<"' �����ڲ����ⲿ���Ҳ���ǿ����еĳ�����������ļ���"<<endl<<endl;
      }
      cout<<endl;
    }
}
