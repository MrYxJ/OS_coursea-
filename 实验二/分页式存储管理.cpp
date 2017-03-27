#include<bits/stdc++.h>
using namespace std;
struct Page
{
  int id;
  int block;
  bool stau;
};
Page *fifo_page,*lru_page,*opt_page;
int pro_size,pagelist_size,page_size,block_size;//进程的大小,页表的大小，单个页的大小，内存块的数量。
int Map1[64][64],Map2[64][64],Map3[64][64],n;//位示图的表示和其大小。
int cnt,ft,lt,ot;
int fifo_list[128][128],lru_list[128][128],opt_list[128][128];//fifo_sum,lru_sum;
int data[110];
void creat_map()
{
    int cnt=0;
    n=pagelist_size;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            int x=rand()%2;
            Map1[i][j]=Map2[i][j]=Map3[i][j]=x;
            if(x) cnt++;
        }
    }
    if(cnt<pagelist_size) creat_map();
}

void show_map(int M[64][64])
{
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<<M[i][j]<<"  ";
        }
        cout<<endl;
    }
}

void init()
{
    cnt=-1;ft=lt=0;
    memset(fifo_list,-1,sizeof(fifo_list));
    memset(lru_list,-1,sizeof(lru_list));
    cout<<"初始化设置"<<endl;
    cout<<"请输入进程的大小(单位:KB)"<<endl;while(pro_size<=0) cin>>pro_size;pro_size*=1024;
    cout<<"请输入单个页的大小(单位:KB)"<<endl;while(page_size<=0) cin>>page_size;page_size*=1024;
    cout<<"请输入内存块个数(单位:整数)"<<endl;while(block_size<=0) cin>>block_size;
    pagelist_size=pro_size/page_size;if(pro_size%page_size) pagelist_size++;
    creat_map();
    fifo_page=new Page[pagelist_size+1];
    lru_page=new Page[pagelist_size+1];
    opt_page=new Page[pagelist_size+1];
    for(int i=0;i<pagelist_size;i++) fifo_page[i].id=i,fifo_page[i].block=-1,fifo_page[i].stau=false;
    for(int i=0;i<pagelist_size;i++) lru_page[i].id=i,lru_page[i].block=-1,lru_page[i].stau=false;
    for(int i=0;i<pagelist_size;i++) opt_page[i].id=i,opt_page[i].block=-1,opt_page[i].stau=false;
    system("PAUSE");
    system("cls");
}

void show_menu()
{
    system("cls");
    cout<<endl<<endl;
    cout << "\t\t\t********************************" << endl;
    cout << "\t\t\t欢迎登陆MrYX的内存分页式管理系统" << endl;
    cout << "\t\t\t********************************" << "\n\n";
    cout << "\t\t\t1.输入地址" << endl;
    cout << "\t\t\t2.查看FIFO位视图变化" << endl;
    cout << "\t\t\t3.查看LRU位示图变化" << endl;
    cout << "\t\t\t4.查看FIFO页表" << endl;
    cout << "\t\t\t5.查看LRU页表" << endl;
    cout << "\t\t\t6.查看OPT页表" << endl;
    cout << "\t\t\t7.查看FIFO表及缺页率" << endl;
    cout << "\t\t\t8.查看LRU表及缺页率" << endl;
    cout << "\t\t\t9.查看OPT表及缺页率" << endl;
    cout << "\t\t\t0.退出" << endl;
}

void find_map(int &num,int M[][64])
{
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(M[i][j]==0) {num=i*n+j;M[i][j]=1;return;}
        }
    }
}

int fifo_count(int t)
{
    if(t==0) return 0;
    int sum=0;
    for(int i=0;i<block_size;i++){
        if(fifo_list[i][t-1]>-1) sum++;
    }
    return sum;
}
int lru_count(int t)
{
    if(t==0) return 0;
    int sum=0;
    for(int i=0;i<block_size;i++){
        if(lru_list[i][t-1]>-1) sum++;
    }
    return sum;
}

void add_fifo(int p)
{
    if(cnt==0){
        fifo_list[block_size-1][cnt]=p;
        return;
    }
   int t=0;
   for(int i=0;i<block_size;i++){
      fifo_list[i][cnt]=fifo_list[i][cnt-1];
      if(fifo_list[i][cnt-1]>-1) t++;
   }
   if(t==block_size){
     for(int i=block_size-1;i>0;i--) fifo_list[i][cnt]=fifo_list[i-1][cnt];
     fifo_list[0][cnt]=p;
   }
   else  fifo_list[block_size-t-1][cnt]=p;
}

void copy_fifo()
{
    for(int i=0;i<block_size;i++) fifo_list[i][cnt]=fifo_list[i][cnt-1];
}

void show_fifo()
{
   cout<<"                            FIFO　Block Process list:"<<endl;
   for(int i=0;i<=cnt;i++) cout<<data[i]<<" ";
   cout<<endl;
   cout<<"-------------------------------------------------------------------------"<<endl;
   for(int i=0;i<block_size;i++){
    for(int j=0;j<=cnt;j++){
        if(fifo_list[i][j]==-1) cout<<"  ";
        else cout<<fifo_list[i][j]<<" ";
    }
    cout<<endl;
   }
  cout<<"-------------------------------------------------------------------------"<<endl;
  cout<<"寻址次数："<<cnt+1<<endl;
  cout<<"命中次数："<<ft<<endl;
  cout<<"缺页次数："<<cnt+1-ft<<endl;
  cout<<"缺页率:"<<(double)(cnt+1-ft)*1.0/((cnt+1)*1.0)*100.0<<"%."<<endl;
}

void show_fifo_list()
{
   cout<<"                            FIFO　Page list:"<<endl;
   cout<<"-------------------------------------------------------------------------"<<endl;
   cout<<"                      ID:  Block: Status:"<<endl;
   for(int i=0;i<pagelist_size;i++){
     cout<<"                      "<<fifo_page[i].id<<"      "<<fifo_page[i].block<<"      "<<fifo_page[i].stau<<endl;
   }
   cout<<"--------------------------------------------------------------------------"<<endl;
}

void add_lru(int p)
{
   if(cnt==0){
      lru_list[block_size-1][cnt]=p;
      return;
   }
   int t=0;
   for(int i=0;i<block_size;i++){
      lru_list[i][cnt]=lru_list[i][cnt-1];
      if(lru_list[i][cnt-1]>-1) t++;
   }
   if(t==block_size){
     for(int i=block_size-1;i>0;i--) lru_list[i][cnt]=lru_list[i-1][cnt];
     lru_list[0][cnt]=p;
   }
   else  lru_list[block_size-t-1][cnt]=p;
}

void trans_lru(int p)
{
   int pos=0,fir=0;
   for(int i=0;i<block_size;i++) if(lru_list[i][cnt-1]==p)  {pos=i;break;}
   for(int i=0;i<block_size;i++) if(lru_list[i][cnt-1]!=-1) {fir=i;break;}
   for(int i=pos;i>fir;i--) lru_list[i][cnt]=lru_list[i-1][cnt-1];
   lru_list[fir][cnt]=p;
   for(int i=pos+1;i<block_size;i++) lru_list[i][cnt]=lru_list[i][cnt-1];
}

void show_lru()
{
   cout<<"                            LRU　Block Process list:"<<endl;
   for(int i=0;i<=cnt;i++) cout<<data[i]<<" ";
   cout<<endl;
   cout<<"-------------------------------------------------------------------------"<<endl;
   for(int i=0;i<block_size;i++){
    for(int j=0;j<=cnt;j++){
        if(lru_list[i][j]==-1) cout<<"  ";
        else cout<<lru_list[i][j]<<" ";
    }
    cout<<endl;
   }
  cout<<"-------------------------------------------------------------------------"<<endl;
  cout<<"寻址次数："<<cnt+1<<endl;
  cout<<"命中次数："<<lt<<endl;
  cout<<"缺页次数："<<cnt+1-lt<<endl;
  cout<<"缺页率:"<<(double)(cnt+1-lt)*1.0/((cnt+1)*1.0)*100.0<<"%."<<endl;
}

void show_lru_list()
{
   cout<<"                            LRU　Page list:"<<endl;
   cout<<"-------------------------------------------------------------------------"<<endl;
   cout<<"                      ID:  Block: Status:"<<endl;
   for(int i=0;i<pagelist_size;i++){
     cout<<"                      "<<lru_page[i].id<<"      "<<lru_page[i].block<<"      "<<lru_page[i].stau<<endl;
   }
   cout<<"--------------------------------------------------------------------------"<<endl;
}

void input_address()
{
    cnt++;
    cout<<"请输入一个16进制的地址: ";
    int x;scanf("%x",&x);
    while(x>=pro_size){
       cout<<"页号越界,进程大小是"<<pro_size<<" KB."<<endl;
       cout<<"请重新输入一个正常16位进制的地址: ";
       scanf("%x",&x);cout<<x<<endl;
    }
    int p=x/page_size;
    data[cnt]=p;
    if(fifo_page[p].stau==true){
        ft++;
        int ans=fifo_page[p].block*page_size+x%page_size;
        cout<<"FIFO页表地址命中"<<endl;
        printf("逻辑地址：%x\n",x);
        printf("物理地址: %x\n",ans);
        copy_fifo();
    }
    else{
        //cout<<"fifo_sum="<<fifo_count(cnt)<<endl;
        //system("PAUSE");
        if(fifo_count(cnt)<block_size){
            int num;
            find_map(num,Map1);
            fifo_page[p].block=num;
            fifo_page[p].stau=true;
            add_fifo(p);
            int ans=fifo_page[p].block*page_size+x%page_size;
            cout<<"FIFO缺页置换后："<<endl;
            printf("逻辑地址：%x\n",x);
            printf("物理地址: %x\n",ans);
        }
        else{
            int temp=fifo_list[block_size-1][cnt-1];
            fifo_page[temp].stau=false;
            int xx=temp/n,yy=temp%n;
            Map1[xx][yy]=0;
           // cout<<"p="<<p<<endl;
            //system("PAUSE");
            int num;
            find_map(num,Map1);
            fifo_page[p].block=num;
            fifo_page[p].stau=true;
            add_fifo(p);
            int ans=fifo_page[p].block*page_size+x%page_size;
            cout<<"FIFO缺页置换后："<<endl;
            printf("逻辑地址：%x\n",x);
            printf("物理地址: %x\n",ans);
        }
    }
    if(lru_page[p].stau==true){
        lt++;
        int ans=lru_page[p].block*page_size+x%page_size;
        trans_lru(p);
        cout<<"LRU页表地址命中"<<endl;
        printf("逻辑地址：%x\n",x);
        printf("物理地址: %x\n",ans);
    }
    else{
        if(lru_count(cnt)<block_size){
            int num;
            find_map(num,Map2);
            lru_page[p].block=num;
            lru_page[p].stau=true;
            add_lru(p);
            int ans=lru_page[p].block*page_size+x%page_size;
            cout<<"FIFO缺页置换后："<<endl;
            printf("逻辑地址：%x\n",x);
            printf("物理地址: %x\n",ans);
            //if(!check_lru(p)) add_lru(p);
        }
        else{
            int temp=lru_list[block_size-1][cnt-1];
            lru_page[temp].stau=false;
            int xx=temp/n,yy=temp%n;
            Map2[xx][yy]=0;
           // cout<<"p="<<p<<endl;
            //system("PAUSE");
            int num;
            find_map(num,Map2);
            lru_page[p].block=num;
            lru_page[p].stau=true;
            add_lru(p);
            int ans=lru_page[p].block*page_size+x%page_size;
            cout<<"FIFO缺页置换后："<<endl;
            printf("逻辑地址：%x\n",x);
            printf("物理地址: %x\n",ans);
        }
    }
}
int opt_count(int t)
{
    if(t==0) return 0;
    int sum=0;
    for(int i=0;i<block_size;i++){
        if(opt_list[i][t-1]>-1) sum++;
    }
    return sum;
}
void add_opt(int p,int t)
{
   if(t==0) { opt_list[block_size-1][t]=p;return;}
   int fir=0;
   for(int i=0;i<block_size;i++) if(opt_list[i][t-1]!=-1) {fir=i;break;}
   opt_list[fir-1][t]=p;
   for(int i=fir;i<block_size;i++) opt_list[i][t]=opt_list[i][t-1];
}
void copy_opt(int t)
{
    for(int i=0;i<block_size;i++) opt_list[i][t]=opt_list[i][t-1];
}

int find_last(int t)
{
    int a[108],v[108];
    memset(v,0,sizeof(v));
    for(int i=0;i<block_size;i++) a[i]=opt_list[i][t-1];
   // for(int i=0;i<block_size;i++) cout<<a[i]<<" ";
    //cout<<endl;
    //system("PAUSE");
    int maxx=t,index=a[0];
    for(int i=0;i<block_size;i++){
        for(int j=t;j<=cnt;j++){
            if(a[i]==data[j]){
                v[i]=1;
                if(maxx<j){
                    maxx=j;
                    index=a[i];
                }
                break;
            }
        }
    }
    int sum=0;
    for(int i=0;i<block_size;i++) sum+=v[i];
    if(sum!=block_size){
        for(int i=0;i<block_size;i++){
            if(!v[i]) index=a[i];
        }
    }
    //cout<<"index="<<index<<endl;
    //system("PAUSE");
    return index;
}

void change(int ans,int p,int t)
{
    for(int i=0;i<block_size;i++){
        if(opt_list[i][t-1]!=ans) opt_list[i][t]=opt_list[i][t-1];
        else opt_list[i][t]=p;
    }
}
void show_opt_list()
{
   cout<<"                            OPT　Page list:"<<endl;
   cout<<"-------------------------------------------------------------------------"<<endl;
   cout<<"                      ID:  Block: Status:"<<endl;
   for(int i=0;i<pagelist_size;i++){
     cout<<"                      "<<opt_page[i].id<<"      "<<opt_page[i].block<<"      "<<opt_page[i].stau<<endl;
   }
   cout<<"--------------------------------------------------------------------------"<<endl;
}

void show_opt()
{
   cout<<"                            OPT　Block Process list:"<<endl;
   for(int i=0;i<=cnt;i++) cout<<data[i]<<" ";
   cout<<endl;
   cout<<"-------------------------------------------------------------------------"<<endl;
   for(int i=0;i<block_size;i++){
    for(int j=0;j<=cnt;j++){
        if(opt_list[i][j]==-1) cout<<"  ";
        else cout<<opt_list[i][j]<<" ";
    }
    cout<<endl;
   }
  cout<<"-------------------------------------------------------------------------"<<endl;
  cout<<"寻址次数："<<cnt+1<<endl;
  cout<<"命中次数："<<ot<<endl;
  cout<<"缺页次数："<<cnt+1-ot<<endl;
  cout<<"缺页率:"<<(double)(cnt+1-ot)*1.0/((cnt+1)*1.0)*100.0<<"%."<<endl;
}

void OPT()
{
   ot=0;
   memset(opt_list,-1,sizeof(opt_list));
   for(int i=0;i<=cnt;i++) opt_page[i].stau=false;
   for(int i=0;i<=cnt;i++){
      if(opt_page[data[i]].stau==true){
         ot++;
         copy_opt(i);
      }
      else{
        // cout<<"count="<<opt_count(i)<<endl;
         //system("PAUSE");
         if(opt_count(i)<block_size){
            int num;
            find_map(num,Map3);
            opt_page[data[i]].block=num;
            opt_page[data[i]].stau=true;
            add_opt(data[i],i);
         }
         else{
            int ans=find_last(i);
            //cout<<"ans=??????"<<ans<<endl;
            //system("PAUSE");
            opt_page[ans].stau=false;
            int xx=opt_page[ans].block/n,yy=opt_page[ans].block%n;
            Map3[xx][yy]=0;
            int num;
            find_map(num,Map3);
            opt_page[data[i]].block=num;
            opt_page[data[i]].stau=true;
            change(ans,data[i],i);
         }
      }
   }
}

int main()
{
    init();
    int choice;
    while(1){
        show_menu();
        cin>>choice;
        switch(choice){
           case 1:system("cls");input_address();system("PAUSE");break;
           case 2:system("cls");show_map(Map1);system("PAUSE");break;
           case 3:system("cls");show_map(Map2);system("PAUSE");break;
           case 4:system("cls");show_fifo_list();system("PAUSE");break;
           case 5:system("cls");show_lru_list();system("PAUSE");break;
           case 6:system("cls");OPT();show_opt_list();system("PAUSE");break;
           case 7:system("cls");show_fifo();system("PAUSE");break;
           case 8:system("cls");show_lru();system("PAUSE");break;
           case 9:system("cls");OPT();show_opt();system("PAUSE");break;
           case 0:exit(1);
        }
    }
}
