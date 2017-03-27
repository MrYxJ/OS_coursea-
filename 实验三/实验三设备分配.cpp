#include<bits/stdc++.h>
using namespace std;
int flag;
typedef struct PCB
{
    string name;
    struct PCB*next;
}pcb;

typedef struct CHCT{
   string name;				//ͨ����
   bool stau;
   PCB *process;		//ռ�ø�ͨ���Ľ���
   PCB *waitlist;	//�ȴ���ͨ���Ľ��̶���
   struct CHCT *next;			//��һ��ͨ��
}CHCT;

typedef struct COCT{
   string name;
   bool stau;
   CHCT *chct;			//���������ϼ�ͨ��
   PCB *process;		//ռ�øÿ������Ľ���
   PCB *waitlist;	//�ȴ��ÿ������Ľ��̶���
   struct COCT *next;			//��һ��������
}COCT;

typedef struct DCT{
   string name;
   string type;
   bool stau;
   COCT *coct;
   PCB *process;		//ռ�ø��豸�Ľ���
   PCB *waitlist;
   struct DCT *next;
}DCT;

typedef struct SDT
{
    string name;
    string type;
    DCT *dct;
    struct SDT * next;
}SDT;


CHCT *channels;			//ͨ������ͷ�ڵ�
COCT *controllers;		//����������ͷ�ڵ�
DCT *devices;			//�豸����ͷ�ڵ�
SDT*systemlist;
const string devices_name[4]={"keyboard","mouse","printer","display"};
set<string>dev_name;
set<string>pro_name;
void init()
{
    dev_name.clear();
    pro_name.clear();
    CHCT *ch1=new CHCT,*ch2=new CHCT;
    channels = new CHCT;channels->next=ch1;
    ch1->name="ch1";ch1->stau=false;ch1->process=NULL;ch1->waitlist=NULL;ch1->next=ch2;
    ch2->name="ch2";ch2->stau=false;ch2->process=NULL;ch2->waitlist=NULL;ch2->next=NULL;

    COCT *co1=new COCT,*co2=new COCT,*co3=new COCT;
    controllers = new COCT; controllers->next = co1;
    co1->name="co1",co1->stau=false,co1->chct=ch1,co1->process=NULL,co1->waitlist=NULL;co1->next=co2;
    co2->name="co2",co2->stau=false,co2->chct=ch2,co2->process=NULL,co2->waitlist=NULL;co2->next=co3;
    co3->name="co3",co3->stau=false,co3->chct=ch2,co3->process=NULL,co3->waitlist=NULL;co3->next=NULL;

    DCT *dc1=new DCT,*dc2=new DCT,*dc3=new DCT,*dc4=new DCT;
    devices=new DCT;devices->next=dc1;
    dc1->name="cherry",dc1->type="keyboard",dc1->stau=false,dc1->coct=co1,dc1->process=NULL,dc1->waitlist=NULL,dc1->next=dc2;
    dc2->name="logi",dc2->type="mouse",dc2->stau=false,dc2->coct=co1,dc2->process=NULL,dc2->waitlist=NULL,dc2->next=dc3;
    dc3->name="hp",dc3->type="printer",dc3->stau=false,dc3->coct=co2,dc3->process=NULL,dc3->waitlist=NULL,dc3->next=dc4;
    dc4->name="view",dc4->type="display",dc4->stau=false,dc4->coct=co3,dc4->process=NULL,dc4->waitlist=NULL,dc4->next=NULL;
    dev_name.insert("cherry");dev_name.insert("logi");dev_name.insert("hp");dev_name.insert("aoc");
}

bool check_devices(string s)
{
    for(int i=0;i<4;i++) if(devices_name[i]==s) return true;
    return false;
}

bool check_dct(DCT *head,string s,string pro,COCT *&ans)
{
    DCT *pt=head;
    while(pt->next){
        if(pt->name==s) break;
        else pt=pt->next;
    }
    if(pt->stau==false){
        pt->stau=true;
        PCB *p=new PCB; p->next=NULL; p->name=pro;
        pt->process=p;
        ans=pt->coct;
        return true;
    }
    else{
        PCB *p=new PCB; p->next=NULL; p->name=pro;
        PCB *t=pt->waitlist;
        if(t==NULL) pt->waitlist=p;
        else {
            while(t->next!=NULL) t=t->next;
            t->next=p;
        }
        cout<<"���̣�"<<pro<<" ������["<<s<<"]�豸��"<<endl;
        return false;
    }
}

bool check_coct(COCT *&head,string pro,CHCT *&ans)
{
    if(head->stau==false){
        head->stau=true;
        PCB *p=new PCB; p->next=NULL; p->name=pro;
        head->process=p;
        ans=head->chct;
        return true;
    }
    else{
        PCB *p=new PCB; p->next=NULL; p->name=pro;
        PCB *t=head->waitlist;
        if(t==NULL) head->waitlist=p;
        else {
            while(t->next!=NULL) t=t->next;
            t->next=p;
        }
        cout<<"���̣�"<<pro<<" ������["<<head->name<<"]�豸��������"<<endl;
        return false;
    }
}

void check_chct(CHCT *&head,string pro)
{
    if(head->stau==false){
        head->stau=true;
        PCB *p=new PCB; p->next=NULL; p->name=pro;
        head->process=p;
    }
    else{
        PCB *p=new PCB; p->next=NULL; p->name=pro;
        PCB *t=head->waitlist;
        if(t==NULL) head->waitlist=p;
        else {
            while(t->next!=NULL) t=t->next;
            t->next=p;
        }
        cout<<"���̣�"<<pro<<" ������["<<head->name<<"]ͨ����"<<endl;
    }
}
void check_devices_independence(string str,string pro,COCT *&ans)
{
   DCT *t=devices->next;
   while(!(t->type==str&&t->stau==false)) t=t->next;
   PCB *p=new PCB;p->name=pro;p->next=NULL;
   t->waitlist=NULL;t->stau=true;t->process=p;
   ans=t->coct;
   cout<<pro<<"���̳ɹ�ռ��"<<str<<"���͵�"<<t->name<<"�豸��"<<endl;
}

void app_pro(string s)
{
    int i=2;while(s[i]!=' ') i++;
    string str=s.substr(2,i-2),pro=s.substr(i+1,s.size()-i-1);
    //cout<<"str="<<str<<"pro="<<pro<<endl;
    if(pro_name.count(pro)){
        cout<<"���󣡽������ظ�"<<endl;
        return;
    }
    pro_name.insert(pro);
    if(check_devices(str)){
       //cout<<"str="<<str<<endl;
       COCT*stc;CHCT*sth;
       check_devices_independence(str,pro,stc);
       //cout<<"coct->name"<<stc->name<<endl;
       if(check_coct(stc,pro,sth)){
         //   cout<<"ch->name"<<sth->name<<endl;
                check_chct(sth,pro);
            }
    }
    else{
        //for(set<string>::iterator it=dev_name.begin();it!=dev_name.end();it++) cout<<*it<<endl;
        if(!dev_name.count(str)){
            cout<<"����ϵͳ�豸���ﲢû�д��豸!"<<endl;
            return;
        }
        COCT*stc;CHCT*sth;
        if(check_dct(devices,str,pro,stc)){
            if(check_coct(stc,pro,sth)){
                check_chct(sth,pro);
            }
        }
    }
}

void rec_chct(CHCT *&t)
{
   PCB*p=t->process;t->process=NULL; delete(p);
   if(t->waitlist==NULL) t->stau=false;
   else{
        t->process=t->waitlist;
        t->waitlist=t->waitlist->next;
   }
}

void pro_ch(CHCT *&head,string pro)
{
    PCB *p=new PCB;
    p->name=pro;p->next=NULL;
    if(head->waitlist==NULL) head->waitlist=p;
    else{
        PCB *t=head->waitlist;
        while(t->next!=NULL) t=t->next;
        t->next=p;
    }
}

void rec_coct(COCT *&t)
{
   PCB*p=t->process;t->process=NULL; delete(p);
   if(t->waitlist==NULL) t->stau=false;
   else{
        t->process=t->waitlist;
        t->waitlist=t->waitlist->next;
        pro_ch(t->chct,t->process->name);
   }
   rec_chct(t->chct);
}

void pro_co(COCT *&head,string pro)
{
    PCB *p=new PCB;
    p->name=pro;p->next=NULL;
    if(head->waitlist==NULL) head->waitlist=p;
    else{
        PCB *t=head->waitlist;
        while(t->next!=NULL) t=t->next;
        t->next=p;
    }
}

bool check_pro(DCT*head)
{
    string pro=head->process->name;
    if(head->coct->process->name!=pro||head->coct->chct->process->name!=pro) return false;
    return true;
}

void rec_pro(string s)
{
    string dev=s.substr(2,s.size()-2);
    if(!dev_name.count(dev)){
        cout<<"dev="<<dev<<endl;
        cout<<"�����豸����û�и��豸"<<endl;
        return;
    }
    DCT *t=devices;
    while(t->name!=dev) t=t->next;
    if(t->stau==false){
         cout<<dev<<"�豸��û��ռ�еĽ���"<<endl;
         return;
    }
    if(!check_pro(t)){
         cout<<"���豸ռ�еĽ��̲�û�еõ�ϵͳ�����豸,������,ͨ��������Դ��"<<endl;
         return;
    }
    else{
       string pro=t->process->name;
       PCB*p=t->process;t->process=NULL; delete(p);
       if(t->waitlist==NULL) t->stau=false;
       else{
         t->process=t->waitlist;
         t->waitlist=t->waitlist->next;
         pro_co(t->coct,t->process->name);
       }
       rec_coct(t->coct);
    }
}


void add_dev(string s)
{
    int i=2;while(s[i]!=' ') i++;
    string type=s.substr(2,i-2),devname=s.substr(i+1,s.size()-i);
   // cout<<type<<" "<<devname<<endl;
    DCT *d=new DCT;
    d->type=type;d->name=devname;d->next=NULL;d->process=NULL;d->waitlist=NULL;d->stau=false;
    dev_name.insert(devname);
    cout<<"������Ҫ�����¼ӵĿ������ϻ������еĿ�����?(��/����)(y/n)";
    char x;cin>>x;
    if(x=='y'){
       COCT *cot=new COCT;cot->process=NULL,cot->waitlist=NULL,cot->next=NULL;cot->stau=false;
       cout<<"�������µĿ���������:"; cin>>cot->name;
       cout<<"�µĿ��ƹ����Ǹ�ͨ����?:";string ch;cin>>ch;
       CHCT*t1=channels->next; while(t1->name!=ch) t1=t1->next;
       if(t1==NULL){
           cout<<"����û�и�ͨ��"<<endl;
           return;
       }
       cot->chct=t1;
       COCT *t2=controllers->next;
       while(t2->next!=NULL) t2=t2->next;
       t2->next=cot;
       d->coct=cot;
       DCT*t3=devices->next; while(t3->next!=NULL) t3=t3->next;
       t3->next=d;
    }
    else{
       cout<<"���������������"<<endl;
       string co;cin>>co;
       COCT *t=controllers->next;
       while(t->name!=co) t=t->next;
       if(t==NULL){
          cout<<"���������ﲢ�޸ÿ�����"<<endl;
          return;
       }
       d->coct=t;
       DCT*pt=devices->next; while(pt->next!=NULL) pt=pt->next;
       pt->next=d;
    }
}

void dele_dev(string s)
{
    string name=s.substr(2,s.size()-2); dev_name.erase(name);
    DCT *t=devices->next;
    while(t->name!=name) t=t->next;
    while(t->stau==true){
        cout<<"���豸���б�ռ�õĽ��̣��޷�ɾ��,���ͷź���ɾ��!"<<endl;
        return;
    }
    if(devices->next==t) devices->next=devices->next->next;
    else{
        DCT *p=devices->next;
        while(p->next!=t) p=p->next;
        p->next=t->next;
    }
}

void show()
{
    cout<<"[SDT]:\t[DCT name]:\t[DCT type]:\t[COCT]:\t[CHCT]:"<<endl;
    DCT *t=devices->next;
    while(t!=NULL){
        cout<<t->name<<"\t"<<t->name<<"\t\t"<<t->type<<"\t\t"<<t->coct->name<<"\t"<<t->coct->chct->name<<endl;
        t=t->next;
    }
    cout<<endl;
}

void show_dev()
{
    DCT *t=devices->next;
    while(t!=NULL){
        cout<<"�豸����"<<t->name<<"\t�豸���ͣ�"<<t->type<<endl;
        if(t->stau){
            cout<<"|Process|:"<<t->process->name;
            cout<<"  |Blocking|:";
            PCB *p=t->waitlist;while(p!=NULL) cout<<p->name<<" ",p=p->next;
        }
        else cout<<t->name<<" �豸û���κν���ռ��"<<endl;
        cout<<endl;
        t=t->next;
    }
}

void show_con()
{
    COCT *t=controllers->next;
    while(t!=NULL){
        cout<<"�豸��������"<<t->name<<endl;
        if(t->stau){
            cout<<"|Process|:"<<t->process->name;
            cout<<"  |Blocking|:";
            PCB *p=t->waitlist;while(p!=NULL) cout<<p->name<<" ",p=p->next;
        }
        else cout<<t->name<<" ������û���κν���ռ��"<<endl;
        cout<<endl;
        t=t->next;
    }
}

void show_ch()
{
    CHCT *t=channels->next;
    while(t!=NULL){
        cout<<"ͨ������"<<t->name<<endl;
        if(t->stau){
            cout<<"|Process|:"<<t->process->name;
            cout<<"  |Blocking|:";
            PCB *p=t->waitlist;while(p!=NULL) cout<<p->name<<" ",p=p->next;
        }
        else cout<<t->name<<" ͨ��û���κν���ռ��"<<endl;
        cout<<endl;
        t=t->next;
    }
}

void show_all()
{
    while(1){
        int ok=0;
        system("cls");
        show();
        cout<<"��Ҫ��ѯ��һ���豸����������ͨ���½���ռ�л����������"<<endl;
        cout<<"1.�豸\n2.������.\n3.ͨ��.\n0.�����ϼ��˵�"<<endl;
        int n;cin>>n;
        switch(n){
           case 0:{ok=1;break;}
           case 1:{show_dev();system("PAUSE");break;}
           case 2:{show_con();system("PAUSE");break;}
           case 3:{show_ch();system("PAUSE");break;}
        }
        if(ok) return;
    }
}

void solve()
{
    cout<<"ѡ�������";
    string s;getline(cin,s);
    if(s.size()==1&&s[0]!='h'&&s[0]!='e'){
        cout<<"���������д���"<<endl;
        return;
    }
    if(s[0]=='s'){
        app_pro(s);
    }
    else if(s[0]=='r'){
        rec_pro(s);
    }
    else if(s[0]=='a'){
        add_dev(s);
    }
    else if(s[0]=='d'){
        dele_dev(s);
    }
    else if(s[0]=='h'){
       show_all();
    }
    else if(s[0]=='e'){
        flag=1;
        return;
    }
}

void menu()
{
    cout<<"\t\t��ӭ����MrYX's IO�豸����ϵͳ\n\n"<<endl;
    cout<< "\t\t[s].�����豸\n" << endl;
    cout<< "\t\t[r].�����豸\n" << endl;
    cout<< "\t\t[a].�����豸\n" << endl;
    cout<< "\t\t[d].ɾ���豸\n" << endl;
    cout<< "\t\t[h].�鿴�豸\n" << endl;
    cout<< "\t\t[e].�˳�ϵͳ\n" << endl;
}

int main()
{
    flag=0;
    init();
    while(1){
        menu();
        solve();
        if(flag==1) return 0;
        system("PAUSE");
        system("cls");
    }
}
