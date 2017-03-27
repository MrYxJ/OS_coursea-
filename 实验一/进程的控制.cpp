#include<bits/stdc++.h>
using namespace std;
typedef struct Empt
{
  int start,len;
}empt;
list<empt>emptable;
int Start,Length,Size;
typedef struct node
{
    string name;
    int start,len;
    struct node *next;

}PCB;

PCB *Ready,*Run,*Block;
bool cmp1(empt a,empt b)
{
   if(a.len==b.len) return a.start<b.start;
   else return a.len<b.len;
}

bool cmp2(empt a,empt b)
{
    return a.start<b.start;
}
void set_empt()
{
    cout<<"              Welcome to Mr's YX Process's OS"<<endl<<endl<<endl;;;
    cout<<"Please set OS's Memory Start :";
    cin>>Start;
    cout<<endl;
    cout<<"Please set Memory's Length :";
    cin>>Length;
    cout<<endl;
    cout<<"Please set Memory's Segment:";
    cin>>Size;
    empt temp;
    temp.start=Start;
    temp.len = Length;
    emptable.push_back(temp);
}
void all_init()
{
    set_empt();
    Ready=new PCB;
    Ready->next=NULL;
    Run=new PCB;
    Run->next=NULL;
    Block=new PCB;
    Block->next=NULL;
}

bool check_memory(PCB *&head)
{
    emptable.sort(cmp1);
    for(list<empt>::iterator it=emptable.begin();it!=emptable.end();it++){
       if((*it).len>=head->len){
            head->start=(*it).start;
            if((*it).len-head->len<=Size){
                head->len=(*it).len;
                emptable.erase(it);
            }
            else{
                (*it).start+=head->len;
                (*it).len-=head->len;
            }
            return true;
       }
    }
    return false;
}

void add(PCB *head,PCB *process)
{
    PCB *tmp=head;
    while(tmp->next!=NULL){
        tmp=tmp->next;
    }
    tmp->next=process;
    process->next=NULL;
}

int count_pro(PCB *head)
{
    int n=0;
    while(head->next){
        n++;
        head=head->next;
    }
    return n;
}

void show_pro(PCB *head)
{
    int tr=count_pro(head);
    if(tr){
       PCB *p=head->next;
       cout<<"All Process Count : "<<tr<<endl;
       for(int i=0;i<tr;i++){
          cout<<"Process :"<<i+1<<" name: "<<p->name<<" start: "<<p->start<<" length: "<<p->len<<endl;
          p=p->next;
       }
    }
    else{
       cout<<"Cureent Process Count: 0"<<endl;
    }
}

void show_all()
{
   cout<<"***************************************************"<<endl;
   cout<<"The Readying Process : "<<endl;
   show_pro(Ready);
   cout<<"-------------------------------------------"<<endl;
   cout<<"The Runing Process : "<<endl;
   show_pro(Run);
   cout<<"-------------------------------------------"<<endl;
   cout<<"The Blocking Process : "<<endl;
   show_pro(Block);
   cout<<"-------------------------------------------"<<endl;
   cout<<"Now Memory usage situation:"<<endl;
   int t=1;
   emptable.sort(cmp2);
   for(list<empt>::iterator it=emptable.begin();it!=emptable.end();it++){
     cout<<"The "<<t++<<" free Block Memory"<<" start: "<<(*it).start<<".  length: "<<(*it).len<<endl;
   }
   cout<<"***************************************************"<<endl;
}

PCB* pop(PCB *&head)
{
    if(head->next!=NULL){
        PCB *p=head->next;
        head->next=head->next->next;
        return p;
    }
    return NULL;
}

void check_run()
{
    if(Run->next==NULL&&Ready->next!=NULL){
        PCB *t=pop(Ready);
        add(Run,t);
    }
}

bool check(PCB* head,string s)
{
    head=head->next;
    while(head!=NULL){
        if(head->name==s) return true;
        head=head->next;
    }
    return false;
}

void creat_PCB(string s,int len)
{
    PCB *p=new PCB;
    p->next=NULL;
    while(check(Ready,s)||check(Run,s)||check(Block,s)){
        cout<<"The Process have been created,Please create a new one!"<<endl;
        cin>>s;
    }
    p->name=s;p->len=len;
    if(check_memory(p)){
       add(Ready,p);
    }
    else{
        cout<<"Not enough Memory to create a Process!"<<endl;
        getchar();
        getchar();
    }
}
void stop()
{
    if(Run->next!=NULL){
        PCB *p=pop(Run);
        add(Ready,p);
    }
    else{
        cout<<"Error!Is not currently running process"<<endl;
        getchar();
        getchar();
    }
}

void block()
{
   if(Run->next!=NULL){
      PCB *p=pop(Run);
      add(Block,p);
   }
   else{
      cout<<"Error!Is not currently running process"<<endl;
      getchar();
      getchar();
   }
}

void recover()
{
   if(Block->next!=NULL){
       PCB *p=pop(Block);
       add(Ready,p);
   }
   else{
      cout<<"Error!Is not currently blocking process"<<endl;
      getchar();
      getchar();
   }
}

void finish()
{
    if(Run->next!=NULL){
        PCB*p=pop(Run);
        check_run();
        emptable.sort(cmp2);
        if(emptable.size()==0){
            empt a;
            a.start=p->start;
            a.len=p->len;
            emptable.push_back(a);
            delete p;
        }
        else{
        for(list<empt>::iterator it=emptable.begin();it!=emptable.end();it++){
              list<empt>::iterator it1=it;
              it1++;
              if((*it).start>p->start+p->len){//当前区间相邻的上部分,下部分都被占用
                 empt a;
                 a.len=p->len;
                 a.start=p->start;
                 emptable.push_back(a);
                 delete p;
                 break;
              }
              else if((*it).start+(*it).len==p->start&&p->start+p->len==(*it1).start){
                 (*it).len+=(p->len+(*it1).len);
                 emptable.erase(it1);
                 delete p;
                 break;
              }
              else if(p->start+p->len==(*it).start){
                 (*it).start=p->start;
                 (*it).len+=p->len;
                 delete p;
                 break;
              }
              else if((*it).start+(*it).len==p->start){
                 (*it).len+=p->len;
                 delete p;
                 break;
              }
        }
     }
    }
    else{
      cout<<"Error!Is not currently running process"<<endl;
         getchar();
      getchar();
    }
}

void free_que(PCB* head)
{

    while(head->next){
        PCB *p=head;
        head=head->next;
        delete p;
    }
}
void free_all()
{
   free_que(Ready);
   free_que(Run);
   free_que(Block);
}

void display()
{
    cout << "Welocme to MrYX's OS" << endl;
    cout << "---------------------------------------" << endl;
    cout << "c ---- [Creat a process] :c -a -150 "<<endl;
    cout << "t ---- [Current process's time has gone]" << endl;
    cout << "s ---- [Block a process]" << endl;
    cout << "g ---- [Recover a process]" << endl;
    cout << "f ---- [Finish a process]" << endl;
    cout << "h ---- [View the help]" << endl;
    cout << "m ---- [View the memory]" << endl;
    cout << "e ---- [Exit]" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Please input command : " << endl;
}

void show()
{
    //getchar();getchar();
    system("cls");
    display();
}
void menu()
{
    all_init();
    while(1){
      show();
      string s;
      getchar();
      getline(cin,s);
      switch(s[0])
      {
          case 'c':{
            int i=0;
            while(s[i]!='-') i++;
            i++;
            int len=i;
            while(s[len]!=' ') len++;
            string str=s.substr(i,len-i);
            int n=s.size()-1;
            int num=0;
            for(int i=n;s[i]!='-';i--) len=i;
            for(int i=len;i<=n;i++) num=num*10+s[i]-'0';
            cout<<"str="<<str<<" "<<num<<endl;
            creat_PCB(str,num);
            check_run();
            show();
            break;
          }
          case 't':{
            stop();
            check_run();
            show();
            break;
          }
          case 's':{
            block();
            check_run();
            show();
            break;
          }
          case 'g':{
            recover();
            show();
            check_run();
            break;
          }
          case 'f':{
            finish();
            show();
            check_run();
            break;
          }
          case 'm':{
            show_all();
            getchar();getchar();
            break;
          }
          case 'h':{
            show();
            break;
          }
          case 'e':{
            free_all();
            cout<<"End"<<endl;
            return;
         }
         default : {
           cout<<"Erro"<<endl;
           cout<<"' "<<s<<" 'is ,Enter any key continue!"<<endl;
           getchar();getchar();
           break;
         }
      }
    }
    return;
}

int main()
{
    menu();
}

