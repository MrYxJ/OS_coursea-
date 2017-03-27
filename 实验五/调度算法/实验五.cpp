#include<bits/stdc++.h>
#include<fstream>
using namespace std;
int Q;
typedef struct node
{
    string name;
    double level;//优先级别
    int arrival_time;//到达时间
    int burst_time;//服务时间
    int finished_time;//结束运行时间
    int runned_time;//已运行周期
    int round_time;//周转周期
    double value_arround;//带权周转周期
}PCB;
PCB p[100];
set<int>ready,blocked,running,finished;
int n;
void init()
{
    ready.clear();blocked.clear();running.clear();finished.clear();
}
bool cmp1(node a,node b)
{
   return a.arrival_time<b.arrival_time;
}

bool cmp2(node a,node b)
{
  return a.burst_time==b.burst_time ? a.arrival_time<b.arrival_time : a.burst_time<b.burst_time;
}

void write()
{
     ofstream out("F:\\操作系统实验\\实验五\\调度算法\\out.txt",ios_base::app);
     out<<"进程名:      "; for(int i=0;i<n;i++) out<<setw(6)<<p[i].name;out<<endl;
     out<<"到达时间:    "; for(int i=0;i<n;i++) out<<setw(6)<<p[i].arrival_time;out<<endl;
     out<<"服务时间:    "; for(int i=0;i<n;i++) out<<setw(6)<<p[i].burst_time;out<<endl;
     out<<"结束运行时间:"; for(int i=0;i<n;i++) out<<setw(6)<<p[i].finished_time;out<<endl;
     out<<"周转周期:    "; for(int i=0;i<n;i++) out<<setw(6)<<p[i].round_time;out<<endl;
     out<<"带权周转周期:"; for(int i=0;i<n;i++) out<<setw(6)<<setprecision(3)<<p[i].value_arround;out<<endl;
     double aver_t=0,aver_tt=0;
     for(int i=0;i<n;i++) aver_t+=p[i].round_time; aver_t=aver_t/(n*1.0);
     for(int i=0;i<n;i++) aver_tt+=p[i].value_arround; aver_tt=aver_tt/(n*1.0);
     out<<"平均周转周期:     "<<aver_t<<endl;
     out<<"平均带权周期:     "<<aver_tt<<endl;
     out<<endl;
     out.close();
}
void FCFS()
{
    sort(p,p+n,cmp1);
    int t=0;
    for(int i=0;i<n;i++){
      if(t<p[i].arrival_time){
        p[i].finished_time=p[i].arrival_time+p[i].burst_time;
        p[i].round_time=p[i].finished_time-p[i].arrival_time;
        p[i].runned_time=p[i].burst_time;
        p[i].value_arround=(p[i].round_time*1.0)/(p[i].burst_time*1.0);
      }
      else{
        p[i].finished_time=t+p[i].burst_time;
        p[i].round_time=p[i].finished_time-p[i].arrival_time;
        p[i].runned_time=p[i].burst_time;
        p[i].value_arround=(p[i].round_time*1.0)/(p[i].burst_time*1.0);
      }
      t=p[i].finished_time;
    }
    ofstream out("F:\\操作系统实验\\实验五\\调度算法\\out.txt",ios_base::app);
    out<<"FCFS:"<<endl;
    out.close();
    write();
}

void SJF()
{
    finished.clear();
    sort(p,p+n,cmp2);
    int t=0;
    for(int i=0;i<n;){
        int index=-1;
        for(int j=0;j<n;j++){
            if(!finished.count(j)&&t>=p[j].arrival_time){
                finished.insert(j);
                index=j;
                break;
            }
        }
        if(index==-1) t++;
        else{
            i++;
            p[index].finished_time=t+p[index].burst_time;
            p[index].round_time=p[index].finished_time-p[index].arrival_time;
            p[index].runned_time=p[index].burst_time;
            p[index].value_arround=(p[index].round_time*1.0)/(p[index].burst_time*1.0);
            t=p[index].finished_time;
        }
    }
    ofstream out("F:\\操作系统实验\\实验五\\调度算法\\out.txt",ios_base::app);
    out<<"SJF:"<<endl;
    out.close();
    write();
}

void PSA()
{
    int t=0;
    finished.clear();
    for(int i=0;i<n;i++){
      double maxx=0;int index;
      for(int j=0;j<n;j++){
        if(!finished.count(j)&&p[j].level>maxx){
            maxx=p[j].level;
            index=j;
        }
      }
      finished.insert(index);
      if(t<p[index].arrival_time){
         p[index].finished_time=p[index].arrival_time+p[index].burst_time;
         p[index].round_time=p[index].finished_time-p[index].arrival_time;
         p[index].runned_time=p[index].burst_time;
         p[index].value_arround=(p[index].round_time*1.0)/(p[index].burst_time*1.0);
         t=p[index].finished_time;
      }
      else{
        p[index].finished_time=t+p[index].burst_time;
        p[index].round_time=p[index].finished_time-p[index].arrival_time;
        p[index].runned_time=p[index].burst_time;
        p[index].value_arround=(p[index].round_time*1.0)/(p[index].burst_time*1.0);
      }
      t=p[index].finished_time;
    }
    ofstream out("F:\\操作系统实验\\实验五\\调度算法\\out.txt",ios_base::app);
    out<<"PSA:"<<endl;
    out.close();
    write();
}

void HRRN()
{
    finished.clear();
    int t=0;
    sort(p,p+n,cmp1);
    for(int i=0;i<n;i++) p[i].level=1;
    for(int i=0;i<n;i++){
      double maxx=0;int index;
      for(int j=0;j<n;j++){
        if(!finished.count(j)&&p[j].level>maxx){
            maxx=p[j].level;
            index=j;
        }
      }
      finished.insert(index);
      if(t<p[index].arrival_time){
         p[index].finished_time=p[index].arrival_time+p[index].burst_time;
         p[index].round_time=p[index].finished_time-p[index].arrival_time;
         p[index].runned_time=p[index].burst_time;
         p[index].value_arround=(p[index].round_time*1.0)/(p[index].burst_time*1.0);
         t=p[index].finished_time;
      }
      else{
        p[index].finished_time=t+p[index].burst_time;
        p[index].round_time=p[index].finished_time-p[index].arrival_time;
        p[index].runned_time=p[index].burst_time;
        p[index].value_arround=(p[index].round_time*1.0)/(p[index].burst_time*1.0);
      }
      t=p[index].finished_time;
      for(int j=0;j<n;j++){
         if(!finished.count(j)){
            if(t>p[j].arrival_time) p[j].level=((t-p[j].arrival_time)+p[j].burst_time)*1.0/(p[j].burst_time*1.0);
         }
      }
    }
    ofstream out("F:\\操作系统实验\\实验五\\调度算法\\out.txt",ios_base::app);
    out<<"HRRN:"<<endl;
    out.close();
    write();
}

void RRT()
{
    int t=0;
    finished.clear();
    for(int i=0;i<n;i++) p[i].runned_time=0;
    sort(p,p+n,cmp1);
    while(finished.size()<n){
      for(int i=0;i<n;i++){
         if(!finished.count(i)){
            if(t>=p[i].arrival_time){
                if(p[i].burst_time-p[i].runned_time>=Q){
                   p[i].runned_time+=Q;
                   t+=Q;
                }
                else{
                   t+=p[i].burst_time-p[i].runned_time;
                   p[i].runned_time=p[i].burst_time;
                }
                if(p[i].runned_time==p[i].burst_time){
                    finished.insert(i);
                    p[i].finished_time=t;
                    p[i].round_time=p[i].finished_time-p[i].arrival_time;
                    p[i].value_arround=(p[i].round_time*1.0)/(p[i].burst_time*1.0);
                }
            }
         }
      }
    }
    ofstream out("F:\\操作系统实验\\实验五\\调度算法\\out.txt",ios_base::app);
    out<<"RRT:       时间片大小："<<Q<<endl;
    out.close();
    write();
}

void MFQ()
{
    sort(p,p+n,cmp1);

}

void menu()
{
    cout<<"Dispatch Algorithm" << endl;
    cout<<"1.FCFS" << endl;
    cout<<"2.SJF" << endl;
    cout<<"3.PSA" << endl;
    cout<<"4.HRRN"<<endl;
    cout<<"5.RRT"<<endl;
    //cout<<"6.MFQ"<<endl;
    cout <<"0.Exit" << endl;
}

void read()
{
     ifstream read("F:\\操作系统实验\\实验五\\调度算法\\in.txt",ios_base::in|ios_base::binary);
     read>>n;
     for(int i=0;i<n;i++){
        read>>p[i].name>>p[i].arrival_time>>p[i].burst_time>>p[i].level;
     }
     read.close();
}

void function()
{
    menu();
    while(1)
    {
        int sel;
        cin>>sel;
        switch(sel)
        {
          case 1: {read();FCFS();break;}
          case 2: {read();SJF(); break;}
          case 3: {read();PSA();break;}
          case 4: {read();HRRN();break;}
          case 5: {read();cout<<"设置时间片大小:";cin>>Q;RRT();break;}
         // case 6: {read(); MFQ();break;}
          case 0: {exit(0);}
          default : break;
        }
        //menu();
    }
}



void show_project()
{
    for(int i=0;i<n;i++) cout<<p[i].name<<" "<<p[i].arrival_time<<" "<<p[i].burst_time<<" "<<p[i].level<<endl;
}

int main()
{
    //show_project();
    function();
}
