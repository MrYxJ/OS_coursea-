#include <iostream>
#include <cstdio>
#include <set>
#include <vector>
using namespace std;

const int MAX_Z = 3;        // 资源种类
const int processNum = 5;   // 进程个数
const int maxn = 11;        // 数组长度

/********************************************
The init Array

********************************************/
int MAX[maxn][maxn];        // Define MAX
int SUM[maxn];              // Define SUM_Z
int ALLOCATION[maxn][maxn]; // Define Allocation
int NEED[maxn][maxn];       // Define Need
int AVAILABLE[maxn];        // Define Available
set<int>hasFinish;         // Define the number of process has finished
vector<int>safePath;       // Storage the safe Path
int WORK[maxn][maxn];        // Storage the set of Work
int posWork;                // Define the crow of Work

/********************************************
The copy Array

********************************************/
int copy_MAX[maxn][maxn];        // Define MAX
int copy_SUM[maxn];              // Define SUM_Z
int copy_ALLOCATION[maxn][maxn]; // Define Allocation
int copy_NEED[maxn][maxn];       // Define Need
int copy_AVAILABLE[maxn];        // Define Available
set<int> copy_hasFinish;         // Define the number of process has finished
vector<int> copy_safePath;       // Storage the safe Path
int copy_WORK[maxn][maxn];        // Storage the set of Work
int copy_posWork;                // Define the crow of Work





void initBank()
{
     //cout << "Input the sum of Z in every process : " << endl;
     for(int i = 0 ; i < MAX_Z ; i ++)     // Get the SUM
        cin >> SUM[i];

    // cout << "Input the MAX : " << endl;
     for(int i = 0 ; i < processNum ; i ++)     // Get the MAX
         for(int j = 0 ; j < MAX_Z ; j ++)
             cin >> MAX[i][j];

     //cout << "Input the Allocation : " << endl;       // Get the Allocation
     for(int i = 0 ; i < processNum ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)
            cin >> ALLOCATION[i][j];

     for(int i = 0 ; i < processNum ; i ++)     // Calculate the NEED
        for(int j = 0 ; j < MAX_Z ; j ++)
            NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];

     for(int j = 0 ; j < MAX_Z ; j ++)      // Get the Available
     {
         int sum = 0;
         for(int i = 0 ; i < processNum ; i ++)
            sum += ALLOCATION[i][j];
         AVAILABLE[j] = SUM[j] - sum;
     }

     int posWork = 0;
     for(int j = 0 ; j < MAX_Z ; j ++)      // Get the Work
        WORK[posWork][j] = AVAILABLE[j];

}


void printSafePath()
{
     cout << "Safe Path : " << endl;
     posWork = 1;
     for(vector<int>::iterator it = safePath.begin() ; it != safePath.end() ; it ++)
     {
        cout << *it << "\t";
        for(int i = 0 ; i < MAX_Z ; i ++)               // Display the MAX
            cout << WORK[posWork - 1][i] << " ";
        cout << "\t";

        for(int i = 0 ; i < MAX_Z ; i ++)               // Display the NEED
            cout << NEED[*it][i] << " ";
        cout << "\t";

        for(int i = 0 ; i < MAX_Z ; i ++)               // Display the ALLOCATION
            cout << ALLOCATION[*it][i] << " ";
        cout << "\t";

        for(int i = 0 ; i < MAX_Z ; i++)                // Display Work + Allocation
            cout << WORK[posWork][i] << " ";
        cout << "\t";
        cout << "true" << endl;                         // Display the stau
        posWork ++;
     }
     cout << endl;
}





/***********************************************************
Request must used copy Array to Caculate , if this request
can response , then update the origin Array . Else don't
change origin Array.    Work should special deal!!!
***********************************************************/

void updateArray()
{
    for(int i = 0 ; i < MAX_Z ; i ++)   SUM[i] = copy_SUM[i];

    for(int i = 0 ; i < processNum ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)   MAX[i][j] = copy_MAX[i][j];

    for(int i = 0 ; i  < processNum ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)   ALLOCATION[i][j] = copy_ALLOCATION[i][j];

    for(int i = 0 ; i < processNum ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)   NEED[i][j] = copy_NEED[i][j];

    for(int i = 0 ; i < MAX_Z ; i ++)    AVAILABLE[i] = copy_AVAILABLE[i];

    for(int i = 0 ; i < posWork ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)
            WORK[i][j] = copy_WORK[i][j];

    while(!safePath.empty())    safePath.pop_back();
    for(vector<int>::iterator it = copy_safePath.begin() ; it != copy_safePath.end() ; it ++)
        safePath.push_back(*it);

}

void copyArray()
{
    for(int i = 0 ; i < MAX_Z ; i ++)   copy_SUM[i] = SUM[i];

    for(int i = 0 ; i < processNum ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)   copy_MAX[i][j] = MAX[i][j];

    for(int i = 0 ; i  < processNum ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)   copy_ALLOCATION[i][j] = ALLOCATION[i][j];

    for(int i = 0 ; i < processNum ; i ++)
        for(int j = 0 ; j < MAX_Z ; j ++)   copy_NEED[i][j] = NEED[i][j];

    for(int i = 0 ; i < MAX_Z ; i ++)   copy_AVAILABLE[i] = AVAILABLE[i];

    for(int i = 0  ; i < MAX_Z ; i ++)  copy_WORK[0][i] = WORK[0][i];

    while(!copy_safePath.empty())   copy_safePath.pop_back();

    copy_hasFinish.clear();
}



void testOut()
{
    for(int i = 0 ; i < MAX_Z ; i ++)   cout << copy_SUM[i] << " ";
    cout << endl;

    for(int i = 0 ; i < processNum ; i ++)
    {
        for(int j = 0 ; j < MAX_Z ; j ++)   cout << copy_MAX[i][j] << " ";
        cout << endl;
    }
    cout << endl;

    for(int i = 0 ; i  < processNum ; i ++)
    {
        for(int j = 0 ; j < MAX_Z ; j ++)   cout << copy_ALLOCATION[i][j] << " " ;
        cout << endl;
    }
    cout << endl;

    for(int i = 0 ; i < processNum ; i ++)
    {
        for(int j = 0 ; j < MAX_Z ; j ++)   cout << copy_NEED[i][j] << " " ;
        cout << endl;
    }
    cout << endl;

    for(int i = 0 ; i < MAX_Z ; i ++)   cout << copy_AVAILABLE[i] << " ";
    cout << endl;

    for(int i = 0  ; i < MAX_Z ; i ++)  cout << copy_WORK[0][i] << " ";
    cout << endl;
}




void request()
{

    /*******************
    Init the request
    *******************/

    copyArray();

    cout << "Input the change of process : " << endl;
    int changeProcess;
    cin >> changeProcess;
    int request_Z;
    //cout << "Input the request Z : " << endl;
    for(int i = 0 ; i < MAX_Z ; i ++)           // Input the changes and update the copy_Allocation & copy_Need
    {
        cin >> request_Z;
        copy_ALLOCATION[changeProcess][i] += request_Z;
        copy_NEED[changeProcess][i] = copy_MAX[changeProcess][i] - copy_ALLOCATION[changeProcess][i];
    }
    for(int j = 0 ; j < MAX_Z ; j ++)           // Update the copy_Available & copy_Work
    {
        int sum = 0;
        for(int i = 0 ; i < processNum ; i ++)
            sum += copy_ALLOCATION[i][j];
        copy_AVAILABLE[j] = copy_SUM[j] - sum;
        copy_WORK[0][j] = copy_AVAILABLE[j];
    }
    copy_posWork = 0;                           // Init the copy_posWork

    /********************
    Judge the request
    ********************/
    int finish = 0 , change;
    while(true)
    {
        change = 0;
        if(copy_hasFinish.size() == processNum) {finish = 1; break;}
        for(int i = 0 ; i < processNum ; i ++)
        {
            if(copy_hasFinish.count(i)) continue;
            int can = 0;
            for(int j = 0 ; j < MAX_Z ; j ++)
                if(copy_NEED[i][j] > copy_WORK[copy_posWork][j])   {can = 1;break;}
            if(can) continue;
            change = 1;
            copy_hasFinish.insert(i);
            copy_safePath.push_back(i);
            copy_posWork ++;
            for(int j = 0 ; j < MAX_Z ; j ++)
                copy_WORK[copy_posWork][j] = copy_WORK[copy_posWork - 1][j] + copy_ALLOCATION[i][j];
        }
        if(change == 0) {break;}                // No change , can't finish it
    }
    if(finish == 0)
    {
        cout << "Create deadlock !!!" << endl;

        for(int i = 0 ; i < processNum ; i ++)              // Output later the update Array
        {
            cout << i << " :" << "\t";
            for(int j = 0 ; j < MAX_Z ; j ++)
                cout << MAX[i][j] << " ";cout << "\t";
            for(int j = 0 ; j < MAX_Z ; j ++)
                cout << ALLOCATION[i][j] << " ";cout << "\t";
            for(int j = 0 ; j < MAX_Z ; j ++)
                cout << NEED[i][j] << " ";
            if(i == 0)
            {
                cout << "\t";
                for(int j = 0 ; j < MAX_Z ; j ++)
                    cout << AVAILABLE[j] << " ";cout << endl;
            }
            else    cout << endl;
        }
    }
    else
    {
        updateArray();
        printSafePath();
        cout << "Request can response." << endl;

        for(int i = 0 ; i < processNum ; i ++)              // Output later the update Array
        {
            cout << i << " :" << "\t";
            for(int j = 0 ; j < MAX_Z ; j ++)
                cout << MAX[i][j] << " ";cout << "\t";
            for(int j = 0 ; j < MAX_Z ; j ++)
                cout << ALLOCATION[i][j] << " ";cout << "\t";
            for(int j = 0 ; j < MAX_Z ; j ++)
                cout << NEED[i][j] << " ";
            if(i == 0)
            {
                cout << "\t";
                for(int j = 0 ; j < MAX_Z ; j ++)
                    cout << AVAILABLE[j] << " ";cout << endl;
            }
            else    cout << endl;
        }
    }
}

void bank()
{
     initBank();
     int pos = 0 , flag = 1;

     while(flag)
     {
         if(hasFinish.size() == processNum) {flag = 1;break;}       // All process has finished
         if(pos == processNum)  pos = 0;                            // Back to 0 , continue scan
         if(hasFinish.count(pos)) {pos++;continue;}                       // Course process has finished , goto next one
         int can = 0;
         for(int i = 0 ; i < MAX_Z; i ++)
             if(NEED[pos][i] > WORK[posWork][i])    {can = 1;break;}    // If have a Need > Available , it can't run
        if(can) {pos ++;continue;}
        hasFinish.insert(pos);
        safePath.push_back(pos);
        posWork ++;
        for(int i = 0 ; i < MAX_Z ; i ++)
            WORK[posWork][i] += WORK[posWork - 1][i] + ALLOCATION[pos][i];                     // Calculate the Work
        pos ++;
     }
     printSafePath();
}


int main()
{
    freopen("in.txt" , "r" , stdin);

    bank();
    request();
    request();
    request();
    return 0;
}
