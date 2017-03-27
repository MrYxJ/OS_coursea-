#include<bits/stdc++.h>
using namespace std;
const int maxn=10;
int n,m,t;
int Max[maxn][maxn],copy_Max[maxn][maxn];
int Available[maxn],copy_Available[maxn];
int Allocation[maxn][maxn],copy_Allocation[maxn][maxn];
int Need[maxn][maxn],copy_Need[maxn][maxn];
int Resquest[maxn][maxn];
int Work[maxn],copy_Work[maxn][maxn];
set<int>vis;set<int>copy_vis;
queue<int>safePath;

void read()
{
   ifstream read("F:\\操作系统实验\\实验五\\银行家算法\\in.txt",ios_base::in|ios_base::binary);
   read>>n>>m;
   for(int i=0;i<m;i++) read>>Available[i];
   for(int i=0;i<n;i++){
     for(int j=0;j<m;j++){
       read>>Max[i][j];
     }
   }
   for(int i=0;i<n;i++){
    int sum=0;
    for(int j=0;j<m;j++){
         read>>Allocation[i][j];
         Available[j]-=Allocation[i][j];
    }
   }
   for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
        Need[i][j]=Max[i][j]-Allocation[i][j];
    }
   }
   read>>t;
   for(int i=0;i<t;i++){
     for(int j=0;j<m;j++){
        read>>Resquest[i][j];
     }
   }
}

void copy1()
{
  for(int i=0;i<n;i++){
     for(int i=0;j<m;j++){
         copy_Max[i][j]=Max[i][j];
         copy_Allocation[i][j]=Allocation[i][j];
         copy_Need[i][j]=Need[i][j];
     }
  }
  for(int i=0;i<n;i++) copy_Avaliable[i]=Avaliable[i],copy_vis[i]=vis[i];
}

void copy2()
{
   for(int i=0;i<n;i++){
     for(int i=0;j<m;j++){
         Max[i][j]= copy_Max[i][j];
         Allocation[i][j]=copy_Allocation[i][j];
         Need[i][j]=copy_Need[i][j];
     }
  }
  for(int i=0;i<n;i++) Avaliable[i]=copy_Avaliable[i],vis[i]=copy_vis[i];
}


void init()
{

}

bool bank()
{
    int value[maxn];
    for(int i=0;i<n;i++) value[i]=Avaliable[i];
    while(!safePath.empty()) safePath.pop();
    while(1){
       if(flag==-1||safePath.size()==n) break;
    }
    if(flag)
}

void printf_safePath()
{

}

void solve()
{
    if(bank()){
        printf_safePath();
    }
    else{
        cout<<"Create deadlock!"<<endl;
        return 0;
    }
    for(int i=0;i<n;i++){

    }
}

int main()
{
   read();
   solve();
}
