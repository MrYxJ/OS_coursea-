#include<bits/stdc++.h>
using namespace std;
string SPACE = "(\\s*)";
string RELATION = "(\\S+)";
string RELATIONS = "(" + RELATION + SPACE + "(," + SPACE + RELATION + ")*)";
string WORD = "(\\S+)";
string WORDS = "(" + WORD + SPACE  + "(," + SPACE + WORD + ")*)";
string VALUE = "((\'\\S*\')|(\\d+))";
string VALUES = "(" + VALUE + SPACE + "(," + SPACE + VALUE + ")*)";
string INDEX = "(\\S+)";
string CHAR = "(char\\(\\d+\\))";
string ORDER = "(create|drop|alter|insert|delete|update|select|root|grant|revoke)";
string ORDERS = "("    + ORDER + "(," + ORDER + ")*)";
string DATA_TYPE = "(int|" + CHAR + ")";
string COLNAME_DATATYPE = WORD + " " + DATA_TYPE;
string COLNAME_DATATYPES = "(" + SPACE + COLNAME_DATATYPE + SPACE + "(," + SPACE + COLNAME_DATATYPE + SPACE + ")*)";
string EXPRESSION = WORD + SPACE + "(>=|<=|<|=|>|!=)" + SPACE + VALUE;
string CONNECTION = WORD + SPACE + "=" + SPACE + WORD;
string EXPRESSIONS = "((" + EXPRESSION + "|" + CONNECTION + ")( (and|or) (" + EXPRESSION + "|" + CONNECTION + "))*)";
string FZ = WORD + SPACE + "=" + SPACE + VALUE;
string FZS = "(" + FZ + "(," + SPACE + FZ + ")*)";
string CREATE_TABLE = "create table " + WORD + SPACE + "\\(" + SPACE + COLNAME_DATATYPES + "\\)";
string DROP_TABLE = "drop table " + RELATION;
string ALTER_ADD = "alter table " + RELATION + " add " + COLNAME_DATATYPE;
string ALTER_DROP = "alter table " + RELATION + " drop " + WORDS;
string INSERT = "insert into " + RELATION + "(" + SPACE + "\\(" + WORDS + "\\)" + SPACE + ")?" + " values" + SPACE + "\\(" + VALUES + "\\)";
string DELETE = "delete from " + RELATION + "(" + " where " + EXPRESSIONS + ")?";
string UPDATE = "update " + RELATION + " set " + FZS + "(" + " where " + EXPRESSIONS + ")?";
string SELECT = "select (\\*|" + WORDS + ") from " + RELATIONS + "(" +" where " + EXPRESSIONS + ")?";
string CREATE_INDEX = "create (unique )?index " + INDEX + " on " + RELATION + " \\(" + WORDS + "\\)( cluster)?";
string DROP_INDEX = "drop index " + INDEX;
string CREATE_USER = "create user " + WORD + " identified by " + WORD;
string GRANT_ON_TO = "grant " + ORDERS + " on " + RELATION + " to " + WORDS;
string REVOKE_ON_TO = "revoke " + ORDERS + " on " + RELATION + " from " + WORDS;
string SHOW_TABLES = "show tables";
string SHOW_INDEXS = "show indexs";
string SHOW_INDEX = "show index " + WORD;
string CHANGE_USER="change user "+WORD;

string USER;
bool wrong;

const int BLOCK_SIZE = 6;

vector<string> ALL_ORDER = {
    CREATE_TABLE, DROP_TABLE, ALTER_ADD, ALTER_DROP, INSERT, DELETE,
    UPDATE, SELECT, CREATE_INDEX, DROP_INDEX, CREATE_USER, GRANT_ON_TO,
    REVOKE_ON_TO, SHOW_TABLES, SHOW_INDEXS,SHOW_INDEX,CHANGE_USER
};

struct UserPermission{
    string user, table, order;
    bool operator==(const UserPermission a){
        return a.user == user && a.table == table && a.order == order;
    };
};

bool yacc(char sql[]){
    if(sql[strlen(sql)-1] != ';') return false;
    while(sql[strlen(sql)-1]==';') sql[strlen(sql)-1]='\0';
    int n = ALL_ORDER.size();
    bool tg = false;
    for(int i=0; i<n; i++){
        std::regex patten(ALL_ORDER[i]);
        if(std::regex_match(sql, patten)){
            return true;
        }
    }
    return false;
}

vector<string> lex(string sql){//将每个命令里的单个字符切割出来
    //lex
    //cout<<"sql=="<<sql<<endl;
    vector<string> word;
    int len = sql.size();
    string s = "";
    for(int i=0; i<=len; i++){
        int val=sql[i]-' ';
        if(i == len){
            if(s != "") word.push_back(s);
            break;
        }
        if(sql[i] == ' '){
            if(s != ""){
                word.push_back(s);
                s = "";
            }
            continue;
        }
        if(sql[i] == '(' || sql[i] == ')' || sql[i] == ',' || sql[i] == '*'){
            if(s != ""){
                word.push_back(s);
                s = "";
            }
            s += sql[i];
            word.push_back(s);
            s = "";
        }
        else if(isalpha(sql[i]) || sql[i] == '\'' || sql[i] == '.' || sql[i] == '_'||val<0){
            s += sql[i];
        }
        else if(isdigit(sql[i])){       //digit
            s += sql[i];
        }
        else if(sql[i]=='<'||sql[i]=='>'||sql[i]=='!'){
            if(s!="") { word.push_back(s);s="";}
            s+=sql[i];
            if(i<len-1&&sql[i+1]!='='){
                word.push_back(s);
                s="";
            }
        }
        else if(sql[i]=='='){
            if(s=="<"||s==">"||s=="!"){
                s+=sql[i];
                word.push_back(s);
            }
            else{
                if(s!="") word.push_back(s);
                word.push_back("=");
            }
            s="";
        }
        else{
            if(s != "" && s[s.size()-1] != '>' && s[s.size()-1] != '<' && s[s.size()-1] != '!'){
                word.push_back(s);
                s = "";
            }
            s += sql[i];
        }
    }
    /*
    for(int i=0; i<word.size(); i++)
        cout<<word[i]<<endl;
    */
   return word;
}



typedef struct DataDict{
    string table,property, type;
    int length;
}DD;

typedef struct Condition{
   string property,op,value;
}CD;

struct Index{
    string index_name, table_name, property;
};

bool login(string user,string password){
   ifstream ifstr;
   string us,ps;
   ifstr.open("user.info",ios::in);
   while(ifstr>>us>>ps){
      if(us==user&&password==ps){
            ifstr.close();
            USER=user;
            return true;
      }
   }
   return false;
}

bool is_condition_hold(string s, string t, string op){
    if(op == "=" && s == t) return true;
    if(op == "!=" && s != t) return true;
    if(!((isdigit(s[0]) && isdigit(t[0])) || (isalpha(s[0]) && isalpha(t[0]))))
        return false;
    if(isdigit(s[0])){
        int ss = atoi(s.c_str());
        int tt = atoi(t.c_str());
        if(op == ">" && ss > tt) return true;
        if(op == "<" && ss < tt) return true;
        if(op == ">=" && ss >= tt) return true;
        if(op == "<=" && ss <= tt) return true;
        return false;
    }
    return false;
}

bool is_permission(string user, string table, string order){
    string usr, tble, odr;
    ifstream ifstr("user.permission", ios::in);
    while(ifstr>>usr>>tble>>odr){
        if(usr == user && (tble == table || tble == "*") && odr == order){
            ifstr.close();
            return true;
        }
    }
    ifstr.close();
    return false;
}

bool is_exist_table(string table){//判断table名是否重复。
    string table_name, property, type;
    int length;
    ifstream ifstr;
    ifstr.open("property.dict", ios::in);
    while(ifstr>>table_name>>property>>type>>length){
        if(table_name == table){
            ifstr.close();
            return true;
        }
    }
    ifstr.close();
    return false;
}

bool is_exist_property(string table, string property){//判断table里某个property 是否存在
    struct DataDict t;
    ifstream ifstr;
    ifstr.open("property.dict", ios::in);
    while(ifstr>>t.table>>t.property>>t.type>>t.length){
        if(t.table == table && t.property == property){
            ifstr.close();
            return true;
        }
    }
    ifstr.close();
    return false;
}

bool is_exist_user(string user){
    string user_name, passwd;
    ifstream ifstr;
    ifstr.open("user.info", ios::in);
    while(ifstr>>user_name>>passwd){
        if(user_name == user){
            ifstr.close();
            return true;
        }
    }
    ifstr.close();
    return false;
}

bool add_property(string table, string property, string type, string len){//往数据字典里写该属性
    //check exist
    if(is_exist_property(table, property) == true){
        cout<<"property "<<property<<" already exist."<<endl;
        return false;
    }
    ofstream ofstr;
    ofstr.open("property.dict", ios::app);
    ofstr<<table<<" "<<property<<" "<<type<<" "<<len<<endl;
    ofstr.close();
    return true;
}

void create_table(vector<string> word){
    int len = word.size();
    string table = word[2];
    //judge permission
    if(!is_permission(USER, table, "create")){
        puts("permission denied.");
        return ;
    }
    //judge table is exist or not
    if(is_exist_table(word[2])){
        cout<<"table "<<word[2]<<" already exists."<<endl;
        return ;
    }
    //output table property to data-dictionary
    for(int i=5; i<len; i++){
        if(word[i] == "int")
            add_property(table, word[i-1], word[i], "0");
        else if(word[i] == "char"){
            add_property(table, word[i-1], word[i], word[i+2]);
            i += 3;
        }
        else if(word[i] == ",") i++;
        else if(word[i] == ")") break;
    }
    //create file for table
    ofstream ofstr;
    ofstr.open(table, ios::out);
    ofstr.close();
    cout<<"table "<<table<<" created successfully."<<endl;
}

void drop_table(vector<string> word){
    string table = word[2];
    //judge permission
    if(!is_permission(USER, table, "drop")){
        puts("permission denied.");
        return ;
    }
    //table is exist or not
    if(!is_exist_table(table)){
        cout<<"table "<<table<<" is not exist."<<endl;
        return ;
    }
    //delete table(file)
    if(remove(table.c_str())){
        puts("delete table failed!");
        return ;
    }

    //solve data dictionary
    vector<DataDict> T;
    struct DataDict tp;
    ifstream ifstr;
    ifstr.open("property.dict", ios::in);
    while(ifstr>>tp.table>>tp.property>>tp.type>>tp.length){
        if(tp.table == word[2]) continue;
        else T.push_back(tp);
    }
    ifstr.close();

    ofstream ofstr;
    ofstr.open("property.dict", ios::out);
    int n = T.size();
    for(int i=0; i<n; i++){
        ofstr<<T[i].table<<" "<<T[i].property<<" "<<T[i].type<<" "<<T[i].length<<endl;
    }
    ofstr.close();
    puts("drop table successfully.");
}

void alter_add(vector<string> word){
    string table = word[2];
    //judge permission
    if(!is_permission(USER, table, "alter")){
        puts("permission denied.");
        return ;
    }
    if(!is_exist_table(table)){
        cout<<"table "<<table<<" is not exist."<<endl;
        return ;
    }
    if(is_exist_property(table, word[4])){
        cout<<"property "<<word[4]<<" already exist in table "<<table<<"."<<endl;
        return ;
    }
    ifstream ifstr;
    ifstr.open("property.dict",ios::in);
    vector<DD>p;
    DD t;
    while(ifstr>>t.table>>t.property>>t.type>>t.length){
        if(t.table==table) p.push_back(t);
    }
    ifstr.close();
    ifstr.open(table,ios::in);
    string tem;
    vector<vector<string> > record;
    vector<string>tp;
    while(ifstr>>tem){
        tp.push_back(tem);
        if(tp.size()==p.size()){
            tp.push_back("null");
            record.push_back(tp);
            tp.clear();
        }
    }
    ifstr.close();
    ofstream ofstr;
    ofstr.open(table,ios::out);
    int n=record.size();
    for(int i=0;i<n;i++){
        int m=record[i].size();
        for(int j=0;j<m;j++){
            ofstr<<record[i][j]<<(j<m-1? " " : "\n");
        }
    }
    ofstr.close();
    bool result;
    if(word[5]=="int"){
         result=add_property(table,word[4],word[5],"0");
    }
    else{
        result=add_property(table,word[4],word[5],word[7]);
    }
    if(result) puts("alter table property successfully.");
}

void alter_drop(vector<string>word){
   string table=word[2];
   if(!is_permission(USER,table,"alter")){
       puts("permission denied.");
       return;
   }
   if(!is_exist_table(table)){
        cout<<"table "<<table<<" is not exist."<<endl;
        return ;
   }
   int len=word.size();
   vector<string> proper;
   for(int i=4; i<len; i++){
        if(word[i] == ",") continue;
        if(!is_exist_property(table, word[i])){
            cout<<"property "<<word[i]<<" not exist."<<endl;
            return ;
        }
   }
   DD t;
    vector<string> pp;
    ifstream ifstr;
    ifstr.open("property.dict", ios::in);
    while(ifstr>>t.table>>t.property>>t.type>>t.length){
        if(t.table == table) pp.push_back(t.property);
    }
    ifstr.close();

    ifstr.open(table, ios::in);
    int cnt = pp.size();
    vector<vector<string> > ppp;
    vector<string> tp;
    string foo;
    while(ifstr>>foo){
        tp.push_back(foo);
        if(tp.size() >= cnt){
            ppp.push_back(tp);
            tp.clear();
        }
    }
    ifstr.close();

    int n = ppp.size();
    int m = ppp[0].size();
    vector<string>::iterator it;
    vector<vector<string> > tmp;
    vector<string> tmpp;
    for(int k=4; k<len; k++){
        if(word[k] == ",") continue;
        int position = 0;
        for(it = pp.begin(); it != pp.end(); it++, position++){
            if((*it) == word[k]) break;
        }
        if(it == pp.end()){
            cout<<"proper "<<word[k]<<" is not exist.";
            return;
        }
        //remove column
        for(int i=0; i<n; i++){
            tmpp = ppp[i];
            int cunt = 0;
            for(it = tmpp.begin(); it != tmpp.end(); it++, cunt++){
                if(cunt == position) break;
            }
            if(it != tmpp.end()) tmpp.erase(it);
            ppp[i] = tmpp;
        }
    }
    //write back
    ofstream ofstr;
    m = ppp[0].size();
    ofstr.open(table, ios::out);
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            ofstr<<ppp[i][j]<<(j < m-1 ? " " : "\n");
        }
    }
    ofstr.close();

    //solve data dictionary property
    vector<struct DataDict> p;
    ifstr.open("property.dict", ios::in);
    //get proper
    while(ifstr>>t.table>>t.property>>t.type>>t.length){
        bool flag = false;
        for(int i=4; i<len; i++){
            if(word[i] == ",") continue;
            if(t.table == table && t.property == word[i]){
                flag = true;
                break;
            }
        }
        if(!flag) p.push_back(t);
    }
    ifstr.close();
    //write back
    ofstr.open("property.dict", ios::out);
    int n_p = p.size();
    for(int i=0; i<n_p; i++){
        ofstr<<p[i].table<<" "<<p[i].property<<" "<<p[i].type<<" "<<p[i].length<<endl;
    }
    ofstr.close();

    puts("alter table property successfully!");
}

bool isnum(string s){//判断string是不是全部由数字组成。
    int n = s.size();
    for(int i=0; i<n; i++)
        if(!isdigit(s[i]))
            return false;
    return true;
}

bool check_proper(string table, vector<string> proper, vector<string> value){//判断insert里的类型是否匹配
    //get property
    ifstream ifstr;
    ifstr.open("property.dict", ios::in);
    struct DataDict t;
    vector<DataDict> p;
    while(ifstr>>t.table>>t.property>>t.type>>t.length){
        if(t.table == table) p.push_back(t);
    }
    ifstr.close();
    int m = p.size();
    //check number
    if(proper.empty()){
        if(value.size() != p.size()){
            puts("property number not match.");
            return false;
        }
        string s;
        for(int i=0; i<m; i++){
            s = value[i];
            if(p[i].type == "char"){
                if(s[0] == '\'' && s[s.size()-1] == '\''){
                    if(s.size()-2 > p[i].length){
                        cout<<"property "<<p[i].property<<" is longer than max_size "<<p[i].length<<"."<<endl;
                        return false;
                    }
                    break;
                }
                cout<<"property "<<p[i].property<<" is not match type char."<<endl;
                return false;
            }
            if(p[i].type == "int"){
                if(!isnum(s)){
                    cout<<"property "<<proper[i]<<" is not match type int."<<endl;
                    return false;
                }
                break;
            }
        }
    }
    else{
        if(proper.size() != value.size()){
            cout<<proper.size()<<"  "<<value.size()<<endl;
            puts("property and value number not match.");
            return false;
        }
        //check type and size
        //check
        int n = proper.size();
        string s;
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                s = value[i];
                if(proper[i] == p[j].property){
                    if(p[j].type == "char"){
                        if(s[0] == '\'' && s[s.size()-1] == '\''){
                            if(s.size()-2 > p[j].length){
                                cout<<"property "<<proper[i]<<" is longer than max_size "<<p[j].length<<"."<<endl;
                                return false;
                            }
                            break;
                        }
                        cout<<"property "<<proper[i]<<" is not match type char."<<endl;
                        return false;
                    }
                    if(p[j].type == "int"){
                        if(!isnum(s)){
                            cout<<"property "<<proper[i]<<" is not match type int."<<endl;
                            return false;
                        }
                        break;
                    }
                }
            }
        }
    }
    return true;
}
bool check_proper2(string pro, string value, string table = ""){
    DataDict p, v;
    if(table != ""){
        ifstream ifstr("property.dict", ios::in);
        while(ifstr>>p.table>>p.property>>p.type>>p.length){
            if(p.table == table && p.property == pro){
                if(p.type == "char" && value[0] == '\'' && value[value.size()-1] == '\''){
                    ifstr.close();
                    return true;
                }
                if(p.type == "int" && isnum(value)){
                    ifstr.close();
                    return true;
                }
                ifstr.close();
                cout<<"property "<<value<<" is not match type "<<p.type<<"."<<endl;
                return false;
            }
        }
        ifstr.close();
        cout<<"no such property "<<pro<<" in table "<<table<<"."<<endl;
        return false;
    }
    //get pro
    int p_pos = pro.find_first_of('.');
    string p_table = pro.substr(0, p_pos);
    string p_pro = pro.substr(p_pos+1);
    bool is_exsit = false;
    ifstream ifstr("property.dict", ios::in);
    while(ifstr>>p.table>>p.property>>p.type>>p.length){
        if(p.table == p_table && p.property == p_pro){
            is_exsit = true;
            break;
        }
    }
    ifstr.close();
    if(!is_exsit){
        cout<<"no such property "<<p_pro<<" in table "<<p_table<<"."<<endl;
        return false;
    }
    if(value.find_first_of('.') != string::npos){
        int v_pos = value.find_first_of('.');
        string v_table = value.substr(0, v_pos);
        string v_pro = value.substr(v_pos+1);
        ifstr.open("property.dict", ios::in);
        while(ifstr>>v.table>>v.property>>v.type>>v.length){
            if(v.table == v_table && v.property == v_pro)
                break;
        }
        ifstr.close();
        if(p.type == v.type) return true;
        else{
            cout<<"property "<<p.property<<"'s type is not match property "<<v.property<<"."<<endl;
        }
    }
    else{
        if(p.type == "char" && value[0] == '\'' && value[value.size()-1] == '\'')
            return true;
        if(p.type == "int" && isnum(value))
            return true;
        return false;
    }
}


void add_record(string table, vector<string> property, vector<string> value){//insert 插入内容写进文件里
    vector<string> p;
    struct DataDict t;
    //get property
    ifstream ifstr;
    ifstr.open("property.dict", ios::in);
    while(ifstr>>t.table>>t.property>>t.type>>t.length){
        if(t.table == table) p.push_back(t.property);
    }
    ifstr.close();
    //write back
    ofstream ofstr;
    ofstr.open(table, ios::app);
    int lem = p.size();
    bool tg;
    if(property.empty()){
        for(int i=0; i<lem; i++){
            if(value[i][0] == '\'') value[i] = value[i].substr(1, value[i].size()-2);
            ofstr<<value[i]<<(i < lem-1 ? " " : "\n");
        }
    }
    else{
        int len = property.size();
        for(int i=0; i<lem; i++){
            tg = false;
            for(int j=0; j<len; j++){
                if(p[i] == property[j]){
                    if(value[j][0] == '\'') value[j] = value[j].substr(1, value[j].size()-2);
                    ofstr<<value[j]<<(i < lem-1 ? " " : "\n");
                    tg = true;
                    break;
                }
            }
            if(!tg)
                ofstr<<"null"<<(i < lem-1 ? " " : "\n");
        }
    }
    ofstr.close();
}

void insert_into(vector<string> word){
    vector<string> property, value;
    /*
    for(int i=0;i<word.size();i++){
         cout<<word[i]<<endl;
    }
    */
    int len = word.size();
    string table = word[2];
    //judge permissiom45
    if(!is_permission(USER, table, "insert")){
        puts("permission denied.");
        return ;
    }
    //judge table is exist or not
    if(!is_exist_table(table)){
        cout<<"table "<<table<<" is not exist."<<endl;
        return ;
    }
    //insert record to table(output to file)
    bool tg = false;
    for(int i=3; i<len; i++){
        if(word[i] == "(" || word[i] == ")" || word[i] == ",") continue;
        if(word[i] == "values"){
            tg = true;
            continue;
        }
        if(tg) value.push_back(word[i]);
        else property.push_back(word[i]);
    }
    if(!check_proper(table, property, value)) return ;
    add_record(table, property, value);
    cout<<"record insert into table "<<word[2]<<" successfully."<<endl;
}

bool is_check_where_BigorSmallorEqual(string s,string op,string t){
    if(op == "=" && s == t) return true;
    if(op == "!=" && s != t) return true;
    if(isnum(s)&&isnum(t)){
        int ss = atoi(s.c_str());
        int tt = atoi(t.c_str());
        if(op == ">" && ss > tt) return true;
        if(op == "<" && ss < tt) return true;
        if(op == ">=" && ss >= tt) return true;
        if(op == "<=" && ss <= tt) return true;
        return false;
    }
    if(op == ">" && s > t) return true;
    if(op == "<" && s < t) return true;
    if(op == ">=" && s >= t) return true;
    if(op == "<=" && s <= t) return true;
    return false;
}

bool is_exist_index(string index_name, string table_name="", string property=""){
    ifstream ifstr("index", ios::in);
    string index, table, pro;
    while(ifstr>>index>>table>>pro){
        if(index == index_name && ((table == table_name && pro == property) || (table_name == "" && property == ""))){
            ifstr.close();
            return true;
        }
    }
    ifstr.close();
    return false;
}

bool condition_all(vector<CD> con){
   for(int i=0;i<con.size();i++){
      if(!is_check_where_BigorSmallorEqual(con[i].property,con[i].op,con[i].value)) return false;
   }
   return true;
}

vector<vector<string> > get_all(string table_name,int len){
    //get record
     string s;
     ifstream ifstr(table_name,ios::in);
     vector<vector<string> > record;
     vector<string> tp;
     while(ifstr>>s){
         tp.push_back(s);
         if(tp.size() >=len){
             record.push_back(tp);
             tp.clear();
         }
     }
     ifstr.close();
     return record;
}


void delete_from(vector<string>word){
    string table_name=word[2];
    int len=word.size();
    if(!is_permission(USER,table_name,"delete")){
        puts("permission denied.");
        return ;
    }
    if(!is_exist_table(table_name)){
        cout<<"table "<<table_name<<" is not exist."<<endl;
        return ;
    }
    if(len==3){
        ofstream ofstr;
        ofstr.open(table_name,ios::out);
        ofstr.close();
        puts("record delete successfully.");
        return;
    }
    if(len==7){
       string proper=word[4],op=word[5],value=word[6];
       vector<string> p, tp;
       //get property
       ifstream ifstr("property.dict", ios::in);
       DD t;
       string s,type;
       if(value[0] == '\'') value = value.substr(1, value.size()-2);
       while(ifstr>>t.table>>t.property>>t.type>>t.length){
          if(t.table == table_name){
              p.push_back(t.property);
              if(t.property==proper) type=t.type;
          }
       }
       ifstr.close();
       vector<vector<string> > record=get_all(table_name,p.size());
       int  k=-1;
       for(int i=0;i<p.size();i++){
         if(p[i]==proper){
              k=i;break;
         }
       }
       if(k==-1){
           cout<<"property "<<proper<<" is not exist in table "<<table_name<<"."<<endl;
           return ;
       }
       if(word[6][0]=='\''&&type=="int"||word[6][0]!='\''&&type=="char"){
           cout<<word[6]<<" is not match type "<<type<<endl;
           return;
       }
       vector<vector<string> > final_record;
       vector<vector<string> >::iterator it;
       vector<CD>con;
       for(it=record.begin();it!=record.end();it++){
          for(int j=0;j<(*it).size();j++){
            cout<<(*it)[j]<<" ";
          }
          cout<<endl;
          con.clear();
          con.push_back(CD{(*it)[k],op,value});
          if(condition_all(con)) continue;
          final_record.push_back(*it);
       }
       ofstream ofstr;
       ofstr.open(table_name, ios::out);
       int len1 = final_record.size();
       int len2 = p.size();
       for(int i=0; i<len1; i++)
          for(int j=0; j<len2; j++)
             ofstr<<final_record[i][j]<<(j < len2-1 ? " " : "\n");
       ofstr.close();
       puts("record delete successfully.");
  }
  else if(len==11){
     if(word[7]=="and"){

     }
     else if(word[7]=="or"){

     }
  }
}

void update(vector<string> word){
    string table = word[1], proper_1 = word[3], value_1 = word[5];
    vector<vector<string> > record;
    vector<string> p, tp;
    //judge permission
    if(!is_permission(USER, table, "update")){
        puts("permission denied.");
        return ;
    }
    //get property
    ifstream ifstr;
    ifstr.open("property.dict", ios::in);
    string ta, per, ty, le;
    while(ifstr>>ta>>per>>ty>>le){
        if(ta == table) p.push_back(per);
    }
    ifstr.close();

    //get record
    ifstr.open(table, ios::in);
    while(ifstr>>ta){
        tp.push_back(ta);
        if(tp.size() >= p.size()){
            record.push_back(tp);
            tp.clear();
        }
    }
    ifstr.close();
    //update some record
    int k1 = -1;
    for(int i=0; i<p.size(); i++){
        if(p[i] == proper_1) k1 = i;
    }
    if(k1 == -1){
        cout<<"property is not exist in table "<<table<<"."<<endl;
        return ;
    }
    vector<string> check_property, check_value;
    check_property.push_back(proper_1);
    check_value.push_back(value_1);
    if(word.size() <= 6){
        if(!check_proper(table, check_property, check_value))
            return ;
        if(value_1[0] == '\'') value_1 = value_1.substr(1, value_1.size()-2);
        vector<vector<string> >::iterator it;
        for(it = record.begin(); it != record.end(); it++){
            (*it)[k1] = value_1;
        }
    }
    else{
        string proper_2 = word[7], value_2 = word[9];
        check_property.push_back(proper_2);
        check_value.push_back(value_2);
        if(!check_proper(table, check_property, check_value))
            return ;
        if(value_2[0] == '\'') value_2 = value_2.substr(1, value_2.size()-2);
        int k2 = -1;
        for(int i=0; i<p.size(); i++){
            if(p[i] == proper_2) k2 = i;
        }
        if(k2 == -1){
            cout<<"property is not exist in table "<<table<<"."<<endl;
            return ;
        }
        vector<vector<string> >::iterator it;
        for(it = record.begin(); it != record.end(); it++){
            if((*it)[k2] == value_2) (*it)[k1] = value_1;
        }
    }
    //write back
    ofstream ofstr;
    ofstr.open(table, ios::out);
    int len1 = record.size();
    int len2 = p.size();
    for(int i=0; i<len1; i++)
        for(int j=0; j<len2; j++)
            ofstr<<record[i][j]<<(j < len2-1 ? " " : "\n");
    ofstr.close();
    puts("record update successfully.");

}

void create_index(vector<string> word){
    string table, index_name, pro;
    index_name = word[2];
    table = word[4];
    pro = word[6];
    //table not exist
    if(!is_exist_table(table)){
        cout<<"table "<<table<<" is not exist."<<endl;
        return ;
    }
    //judge permission
    if(!is_permission(USER, table, "create")){
        puts("permission denied.");
        return ;
    }
    //property not exist
    if(!is_exist_property(table, pro)){
        cout<<"property "<<pro<<" is not exist."<<endl;
        return ;
    }
    //add to index
    if(!is_exist_index(index_name, table, pro)){
        ofstream ofstr("index", ios::app);
        ofstr<<index_name<<" "<<table<<" "<<pro<<endl;
        ofstr.close();
    }
    //get property
    vector<DataDict> P;
    DataDict tp;
    int position = 0;
    bool tg_pos = false;
    ifstream ifstr("property.dict", ios::in);
    while(ifstr>>tp.table>>tp.property>>tp.type>>tp.length){
        if(tp.table == table){
            P.push_back(tp);
            if(!tg_pos && tp.property == pro)
                tg_pos = true;
            if(!tg_pos) position ++;
        }
    }
    ifstr.close();
    //create index
    vector<pair<string, int> > index;
    vector<vector<string> > record;
    vector<string> t;
    string s;
    ifstr.open(table, ios::in);
    int cnt_record = 0;
    if(position!=0){          //assistant index
        while(ifstr>>s){
            t.push_back(s);
            if(t.size() >= P.size()){
                record.push_back(t);
                index.push_back(make_pair(t[position], cnt_record/BLOCK_SIZE));
                cnt_record ++;
                t.clear();
            }
        }
    }
    while(ifstr>>s){            //main index
        t.push_back(s);
        if(t.size() >= P.size()){
            record.push_back(t);
            if(cnt_record % BLOCK_SIZE == 0)
                index.push_back(make_pair(t[position], cnt_record));
            cnt_record ++;
            t.clear();
        }
    }
    ifstr.close();
    sort(index.begin(), index.end());
    //write back
    ofstream ofstr(index_name+".index", ios::out);
    for(vector<pair<string, int> >::iterator it = index.begin(); it != index.end(); it++){
        ofstr<<it->first<<" "<<it->second<<endl;
    }
    ofstr.close();
    puts("Index create successfully.");
}


void drop_index(vector<string> word){
    string index_name = word[2];
    //index not exist
    if(!is_exist_index(index_name)){
        cout<<"index "<<index_name<<" is not exists."<<endl;
        return ;
    }
    vector<Index> index;
    struct Index tp;
    ifstream ifstr("index", ios::in);
    while(ifstr>>tp.index_name>>tp.table_name>>tp.property){
        if(tp.index_name == index_name) continue;
        index.push_back(tp);
    }
    ifstr.close();
    //write back
    ofstream ofstr("index", ios::out);
    int index_size = index.size();
    for(int i=0; i<index_size; i++)
        ofstr<<tp.index_name<<" "<<tp.table_name<<" "<<tp.property<<endl;
    ofstr.close();
    //delete index
    string IndexName = index_name + ".index";
    if(remove(IndexName.c_str())){
        puts("delete index failed!");
        return ;
    }
    puts("Index drop successfully.");
}


void select(vector<string> word){
    time_t t_begin = clock();
    vector<string> pro;
    vector<string> table;
    vector<Condition> condition;
    Condition conn;
    int len = word.size();
    bool tg_from = false;
    bool tg_where = false;
    for(int i=1; i<len; i++){
        if(word[i] == ",") continue;
        if(word[i] == "from"){
            tg_from = true;
            continue;
        }
        if(word[i] == "where"){
            tg_where = true;
            continue;
        }
        if(!tg_from) pro.push_back(word[i]);
        else if(tg_from && !tg_where) table.push_back(word[i]);
        if(tg_where==true){
            if(i+2 < len){
                if(word[i+2].find_first_of('.') != string::npos)
                    conn = Condition{word[i], word[i+1], word[i+2]};
                else
                    condition.push_back(Condition{word[i], word[i+1], word[i+2]});
                i += 3;
            }
        }
    }
    //exist table
    for(int i=0; i<table.size(); i++){
        if(!is_exist_table(table[i])){
            cout<<"table "<<table[i]<<" is not exist."<<endl;
            return ;
        }
        //judge permission
        if(!is_permission(USER, table[i], "select")){
            puts("permission denied.");
            return ;
        }
    }
    //exist property
    if(!pro.empty() && pro[0] != "*"){
        for(int i=0; i<pro.size(); i++){
            if(pro[i].find_first_of('.') != string::npos){
                int pos = pro[i].find_first_of('.');
                string s_table = pro[i].substr(0, pos);
                string s_pro = pro[i].substr(pos+1);
                if(!is_exist_property(s_table, s_pro)){
                    cout<<"property "<<pro[i]<<" is not exist."<<endl;
                    return ;
                }
            }
            else{           //single table
                if(!is_exist_property(table[0], pro[i])){
                    cout<<"property "<<pro[i]<<" is not exist."<<endl;
                    return ;
                }
            }
        }
    }
    //condition legal
    if(!condition.empty()){
        if(table.size() == 1){
            for(int i=0; i<condition.size(); i++)
                if(!check_proper2(condition[i].property, condition[i].value, table[0]))
                    return;
        }
        else{
            for(int i=0; i<condition.size(); i++)
                if(!check_proper2(condition[i].property, condition[i].value))
                    return;
        }
    }
    //conn legal
    if(table.size() > 1){
        if(!check_proper2(conn.property, conn.value)){
            return;
        }
    }
    //solve char
    for(int i=0; i<condition.size(); i++){
        if(condition[i].value[0] == '\'')
            condition[i].value = condition[i].value.substr(1, condition[i].value.size()-2);
    }

    //single or multi table
    /*
    for(int i=0;i<pro.size();i++) cout<<pro[i]<<" ";cout<<endl;
    for(int i=0;i<table.size();i++) cout<<table[i]<<" ";cout<<endl;
    for(int i=0;i<condition.size();i++) cout<<condition[i].property<<" "<<condition[i].op<<" "<<condition[i].value<<endl;
    */
    if(table.size() == 1){          //single table
        vector<DataDict> p;
        struct DataDict t;
        string s;
        //get property
        int cnt = pro.size();
        vector<bool> flag;
        ifstream ifstr;
        ifstr.open("property.dict", ios::in);
        bool tgg;
        if(pro[0] == "*"){
            while(ifstr>>t.table>>t.property>>t.type>>t.length){
                if(t.table == table[0]){
                    p.push_back(t);
                    flag.push_back(true);
                }
            }
        }
        else{
            while(ifstr>>t.table>>t.property>>t.type>>t.length){
                if(t.table == table[0]){
                    p.push_back(t);
                    tgg = false;
                    for(int i=0; i<cnt; i++){
                        if(t.property == pro[i]){
                            flag.push_back(true);
                            tgg = true;
                            break;
                        }
                    }
                    if(tgg) continue;
                    flag.push_back(false);
                }
            }
        }
        ifstr.close();
        //get condition property place
        vector<int> condition_place;
        for(int i=0; i<condition.size(); i++){
            for(int j=0; j<p.size(); j++){
                if(p[j].property == condition[i].property){
                    condition_place.push_back(j);
                }
            }
        }
        //get record
        vector<vector<string> > pp;
        vector<string> tp;
        ifstr.open(table[0], ios::in);
        int n = p.size();
        bool tg = false;
        while(ifstr>>s){
            tp.push_back(s);
            if(tp.size() >= n){
                pp.push_back(tp);
                tp.clear();
            }
        }
        ifstr.close();

        vector<vector<string> > ans, foo;
        vector<string> ans_p;
        int m = pp.size();
        //exist index
        bool has_index = false;
        if(!condition.empty()){
            Index index_tp;
            vector<int> index;
            ifstr.open("index", ios::in);
            string index_name = table[0] + "_" + condition[0].property;
            while(ifstr>>index_tp.index_name>>index_tp.table_name>>index_tp.property){
                if(index_tp.table_name+"_"+index_tp.property== index_name){
                    has_index = true;
                    break;
                }
            }
            ifstr.close();
            if(has_index){
                puts("using index!");
                //get index
                string str;
                int addr;
                ifstr.open(index_name + ".index", ios::in);
                while(ifstr>>str>>addr){
                    if(is_check_where_BigorSmallorEqual(str,condition[0].op,condition[0].value))
                        index.push_back(addr);
                }
                ifstr.close();
                //get index property position
                int index_position = -1;
                for(int i=0; i<n; i++){
                    if(p[i].property == condition[0].property){
                        index_position = i;
                        break;
                    }
                }
                int cnt_index = index.size();
                for(int i=0; i<cnt_index; i++){
                    for(int j=0; j<BLOCK_SIZE; j++){
                        if(index[i]*BLOCK_SIZE + j >= m) break;
                        if(is_condition_hold(pp[index[i]*BLOCK_SIZE+j][index_position], condition[0].value, condition[0].op))
                            foo.push_back(pp[index[i]*BLOCK_SIZE+j]);
                    }
                }
            }
        }
        //get ans
        if(!has_index) foo = pp;
        m = foo.size();
        bool tg_;
        for(int i=0; i<m; i++){
            //check contain
            tg_ = false;
            for(int j=0; j<condition_place.size(); j++){
                if(!is_check_where_BigorSmallorEqual(foo[i][condition_place[j]],condition[j].op,condition[j].value)){
                    tg_ = true;
                    break;
                }
            }
            if(tg_) continue;

            ans_p.clear();
            for(int j=0; j<n; j++){
                if(flag[j]) ans_p.push_back(foo[i][j]);
            }
            ans.push_back(ans_p);
        }

        //displays
        int mm = ans.size();
        if(mm == 0){
            cout<<"no such record in table "<<table[0]<<"."<<endl;
            return ;
        }
        if(pro.size() == 1 && pro[0] == "*"){
            for(int i=0; i<n; i++)
                cout<<p[i].property<<(i < n-1 ? "  " : "\n");
            for(int i=0; i<n; i++)
                cout<<"------";
            puts("");
            for(int i=0; i<mm; i++){
                for(int j=0; j<n; j++)
                    cout<<ans[i][j]<<(j < n-1 ? "  " : "\n");
            }
            for(int i=0; i<n; i++)
                cout<<"------";
            puts("");
        }
        else{
            for(int i=0; i<n; i++){
                if(flag[i])
                    cout<<p[i].property<<"  ";
            }
            puts("");
            for(int i=0; i<cnt; i++)
                cout<<"------";
            puts("");
            for(int i=0; i<mm; i++){
                for(int j=0; j<ans[i].size(); j++)
                    cout<<ans[i][j]<<(j < ans[i].size()-1 ? "  " : "\n");

            }
            for(int i=0; i<cnt; i++)
                cout<<"------";
            puts("");
        }
    }
    else{           //multi table
        vector<DataDict> p1, p2;
        DataDict t;
        string s;
        //get property
        int cnt = pro.size();
        vector<bool> flag1, flag2;
        ifstream ifstr;
        ifstr.open("property.dict", ios::in);
        bool tgg1, tgg2;
        if(pro[0] == "*"){
            while(ifstr>>t.table>>t.property>>t.type>>t.length){
                if(t.table == table[0]){
                    p1.push_back(t);
                    flag1.push_back(true);
                }
                if(t.table == table[1]){
                    p2.push_back(t);
                    flag2.push_back(true);
                }
            }
        }
        else{
            while(ifstr>>t.table>>t.property>>t.type>>t.length){
                if(t.table == table[0]){
                    p1.push_back(t);
                    tgg1 = false;
                    for(int i=0; i<cnt; i++){
                        int pos = pro[i].find_first_of('.');
                        string s_table = pro[i].substr(0, pos);
                        string s_pro = pro[i].substr(pos+1);

                        if(s_table == t.table && t.property == s_pro){
                            flag1.push_back(true);
                            tgg1 = true;
                            break;
                        }
                    }
                    if(!tgg1) flag1.push_back(false);
                }
                if(t.table == table[1]){
                    p2.push_back(t);
                    tgg2 = false;
                    for(int i=0; i<cnt; i++){
                        int pos = pro[i].find_first_of('.');
                        string s_table = pro[i].substr(0, pos);
                        string s_pro = pro[i].substr(pos+1);

                        if(s_table == t.table && t.property == s_pro){
                              flag2.push_back(true);
                            tgg2 = true;
                            break;
                        }
                    }
                    if(!tgg2) flag2.push_back(false);
                }
            }
        }
        ifstr.close();
        //get condition property place
        vector<pair<int, int> > condition_place1, condition_place2;
        for(int i=0; i<condition.size(); i++){
            int pos = condition[i].property.find_first_of('.');
            string s_table = condition[i].property.substr(0, pos);
            string s_pro = condition[i].property.substr(pos+1);
            if(s_table == table[0]){
                for(int j=0; j<p1.size(); j++){
                    if(p1[j].property == s_pro && s_table == table[0]){
                        condition_place1.push_back(make_pair(j, i));
                        break;
                    }
                }
            }
            else{
                for(int j=0; j<p2.size(); j++){
                    if(p2[j].property == s_pro && s_table == table[1]){
                        condition_place2.push_back(make_pair(j, i));
                        break;
                    }
                }
            }
        }
        //get conn place
        int conn_place1, conn_place2;
        int pos1 = conn.property.find_first_of('.');
        string s_pro1 = conn.property.substr(pos1+1);
        int pos2 = conn.value.find_first_of('.');
        string s_pro2 = conn.value.substr(pos2+1);
        for(int j=0; j<p1.size(); j++){
            if(p1[j].property == s_pro1){
                conn_place1 = j;
                break;
            }
        }
        for(int j=0; j<p2.size(); j++){
            if(p2[j].property == s_pro2){
                conn_place2 = j;
                break;
            }
        }
        //get record
        vector<vector<string> > pp1, pp2;
        vector<string> tp;
        //table 1
        ifstr.open(table[0], ios::in);
        int n = p1.size();
        while(ifstr>>s){
            tp.push_back(s);
            if(tp.size() >= n){
                pp1.push_back(tp);
                tp.clear();
            }
        }
        ifstr.close();
        //table 2
        ifstr.open(table[1], ios::in);
        int nn = p2.size();
        while(ifstr>>s){
            tp.push_back(s);
            if(tp.size() >= nn){
                pp2.push_back(tp);
                tp.clear();
            }
        }
        ifstr.close();

        vector<vector<string> > ans_choose1, ans_choose2, foo1, foo2;
        int m1 = pp1.size(), m2 = pp2.size();
        //exist index in table 1
        bool has_index = false;
        if(true){
            Index index_tp;
            vector<int> index;
            string index_name = table[0] + "_" + s_pro1;
            ifstr.open("index", ios::in);
            while(ifstr>>index_tp.index_name>>index_tp.table_name>>index_tp.property){
                if(index_tp.index_name == index_name){
                    has_index = true;
                    break;
                }
            }
            ifstr.close();
            if(has_index){
                puts("using index!");
                //get index
                string str;
                int addr;
                ifstr.open(index_name + ".index", ios::in);
                while(ifstr>>str>>addr){
                    index.push_back(addr);
                }
                ifstr.close();
                //get index property position
                int index_position = -1;
                for(int i=0; i<n; i++){
                    if(p1[i].property == s_pro1){
                        index_position = i;
                        break;
                    }
                }
                int cnt_index = index.size();
                for(int i=0; i<cnt_index; i++){
                    for(int j=0; j<BLOCK_SIZE; j++){
                        if(index[i]*BLOCK_SIZE + j >= m1) break;
                        if(find(foo1.begin(), foo1.end(), pp1[index[i]*BLOCK_SIZE+j]) == foo1.end())
                            foo1.push_back(pp1[index[i]*BLOCK_SIZE+j]);
                    }
                }
            }
            if(!has_index) foo1 = pp1;
        }
        //exist index in table 2
        has_index = false;
        if(true){
            Index index_tp;
            vector<int> index;
            string index_name = table[1] + "_" + s_pro2;
            ifstr.open("index", ios::in);
            while(ifstr>>index_tp.index_name>>index_tp.table_name>>index_tp.property){
                if(index_tp.index_name == index_name){
                    has_index = true;
                    break;
                }
            }
            ifstr.close();
            if(has_index){
                puts("using index!");
                //get index
                string str;
                int addr;
                ifstr.open(index_name + ".index", ios::in);
                while(ifstr>>str>>addr){
                    index.push_back(addr);
                }
                ifstr.close();
                //get index property position
                int index_position = -1;
                for(int i=0; i<nn; i++){
                    if(p2[i].property == s_pro2){
                        index_position = i;
                        break;
                    }
                }
                int cnt_index = index.size();
                for(int i=0; i<cnt_index; i++){
                    for(int j=0; j<BLOCK_SIZE; j++){
                        if(index[i]*BLOCK_SIZE + j >= m2) break;
                        if(find(foo2.begin(), foo2.end(), pp2[index[i]*BLOCK_SIZE+j]) == foo2.end())
                            foo2.push_back(pp2[index[i]*BLOCK_SIZE+j]);
                    }
                }
            }
            if(!has_index) foo2 = pp2;
        }
        //table 1 choose
        int cnt_pp1 = foo1.size(), cnt_pp2 = foo2.size();
        int cnt_con1 = condition_place1.size(), cnt_con2 = condition_place2.size();
        bool is_choose;
        for(int i=0; i<cnt_pp1; i++){
            is_choose = true;
            for(int j=0; j<cnt_con1; j++){
                if(!is_condition_hold(foo1[i][condition_place1[j].first], condition[condition_place1[j].second].value, condition[condition_place1[j].second].op)){
                    is_choose = false;
                    break;
                }
            }
            if(is_choose){
                ans_choose1.push_back(foo1[i]);
            }
        }
        //table 2 choose
        for(int i=0; i<cnt_pp2; i++){
            is_choose = true;
            for(int j=0; j<cnt_con2; j++){
                if(!is_condition_hold(foo2[i][condition_place2[j].first], condition[condition_place2[j].second].value, condition[condition_place2[j].second].op)){
                    is_choose = false;
                    break;
                }
            }
            if(is_choose){
                ans_choose2.push_back(foo2[i]);
            }
        }
        //join table 1 and table 2
        vector<vector<string> > ans;
        vector<string> ans_tp1, ans_tp2;
        int cnt_c1 = ans_choose1.size(), cnt_c2 = ans_choose2.size();
        for(int i=0; i<cnt_c1; i++){
            ans_tp1.clear();
            for(int k=0; k<n; k++){
                if(flag1[k]) ans_tp1.push_back(ans_choose1[i][k]);
            }
            for(int j=0; j<cnt_c2; j++){
                if(is_condition_hold(ans_choose1[i][conn_place1], ans_choose2[j][conn_place2], conn.op)){
                    ans_tp2 = ans_tp1;
                    for(int k=0; k<nn; k++){
                        if(flag2[k]) ans_tp2.push_back(ans_choose2[j][k]);
                    }
                    ans.push_back(ans_tp2);
                }
            }
        }
        //displays
        int cnt_ans = ans.size();
        if(cnt_ans == 0){
            cout<<"no such record."<<endl;
            return ;
        }
        if(pro.size() == 1 && pro[0] == "*"){
            for(int i=0; i<n; i++)
                cout<<p1[i].property<<"  ";
            for(int i=0; i<nn; i++)
                cout<<p2[i].property<<(i < nn-1 ? "  " : "\n");
            for(int i=0; i<n+nn; i++)
                cout<<"------";
            puts("");
            for(int i=0; i<cnt_ans; i++){
                for(int j=0; j<n+nn; j++)
                    cout<<ans[i][j]<<(j < n+nn-1 ? "  " : "\n");
            }
            for(int i=0; i<n+nn; i++)
                cout<<"------";
            puts("");
        }
        else{
            for(int i=0; i<n; i++){
                if(flag1[i]) cout<<p1[i].property<<"  ";
            }
            for(int i=0; i<nn; i++){
                if(flag2[i]) cout<<p2[i].property<<"  ";
            }
            puts("");
            for(int i=0; i<cnt; i++)
                cout<<"------";
            puts("");
            for(int i=0; i<cnt_ans; i++){
                int cnt_tp = ans[i].size();
                for(int j=0; j<cnt_tp; j++)
                    cout<<ans[i][j]<<(j < cnt_tp-1 ? "  " : "\n");
            }
            for(int i=0; i<cnt; i++)
                cout<<"------";
            puts("");
        }
    }
    time_t t_end = clock();
    cout<<"Time Cost: "<<difftime(t_end, t_begin)<<" ms."<<endl;
}

void create_user(vector<string> word){
    string user = word[2], password = word[5];
    if(is_exist_user(user)){
        cout<<"user "<<user<<" already exists."<<endl;
        return ;
    }
    ofstream ofstr("user.info", ios::app);
    ofstr<<user<<" "<<password<<endl;
    ofstr.close();
    puts("User create successfully.");
}

void grant(vector<string> word){
    vector<string> order_list;
    vector<string> user_list;
    string table;
    int word_size = word.size();
    bool tg_on = false, tg_to = false;
    for(int i=1; i<word_size; i++){
        if(word[i] == ",") continue;
        if(word[i] == "on") tg_on = true;
        else if(word[i] == "to") tg_to = true;
        else{
            if(tg_on == false)
                order_list.push_back(word[i]);
            else{
                if(tg_to == false) table = word[i];
                else{
                    user_list.push_back(word[i]);
                    //user not exist
                    if(!is_exist_user(word[i])){
                        cout<<"user "<<word[i]<<" is not exist."<<endl;
                        return ;
                    }
                    if(word[i] == USER){
                        cout<<"permission denied."<<endl;
                        return ;
                    }
                }
            }
        }
    }
    //table not exist
    if(!is_exist_table(table)){
        cout<<"table "<<table<<" is not exist."<<endl;
        return ;
    }

    //write back
    ofstream ofstr("user.permission", ios::app);
    int user_size = user_list.size();
    int order_len;
    for(int i=0; i<user_size; i++){
        order_len = order_list.size();
        for(int j=0; j<order_len; j++){
            if(is_permission(user_list[i], table, order_list[j]))
                continue;
            ofstr<<user_list[i]<<" "<<table<<" "<<order_list[j]<<endl;
        }
    }
    ofstr.close();
    puts("Grant successfully.");
}

void revoke(vector<string> word){
    vector<string> order_list;
    vector<string> user_list;
    string table;
    int word_size = word.size();
    bool tg_on = false, tg_from = false;
    for(int i=1; i<word_size; i++){
        if(word[i] == ",") continue;
        if(word[i] == "on") tg_on = true;
        else if(word[i] == "from") tg_from = true;
        else{
            if(tg_on == false)
                order_list.push_back(word[i]);
            else{
                if(tg_from == false) table = word[i];
                else{
                    user_list.push_back(word[i]);
                    //table not exist
                    if(!is_exist_user(word[i])){
                        cout<<"user "<<word[i]<<" is not exist."<<endl;
                        return ;
                    }
                }
            }
        }
    }
    //table not exist
    if(!is_exist_table(table)){
        cout<<"table "<<table<<" is not exist."<<endl;
        return ;
    }

    //get user order
    vector<UserPermission> user;
    string usr, tab, odr;
    ifstream ifstr("user.permission", ios::in);
    while(ifstr>>usr>>tab>>odr){
        user.push_back(UserPermission{usr, tab, odr});
    }
    ifstr.close();
    //revoke
    int user_size = user_list.size();
    int user_len = user.size();
    int order_len;
    vector<UserPermission>::iterator it;
    for(int i=0; i<user_size; i++){
        order_len = user_list[i].size();
        for(int j=0; j<order_len; j++){
            it = find(user.begin(), user.end(), UserPermission{user_list[i], table, order_list[j]});
            if(it != user.end()){
                user.erase(it);
                break;
            }
            else{
                cout<<"Error: user "<<user_list[i]<<" has no permission "<<order_list[j]<<" on table "<<table<<"."<<endl;
                return ;
            }
        }
    }

    //write back
    int ans_size = user.size();
    ofstream ofstr("user.permission", ios::out);
    for(int i=0; i<ans_size; i++){
        ofstr<<user[i].user<<" "<<user[i].table<<" "<<user[i].order<<endl;
    }
    ofstr.close();
    puts("Revoke successfully.");
}

bool change_user(vector<string>word){
   string user=word[2];
   if(!is_exist_user(user)){
       cout<<user<<"is not exit"<<endl;
       return false;
   }
   if(USER=="root"){
       USER=user;
       return true;
   }
   cout<<"please input "<<user<<" password:";
   string ps;cin>>ps;
   if(login(user,ps)){
        USER=user;
        return true;
   }
   else return false;
}
void excute(vector<string> word){
   if(word[0]=="create"){
     if(word[1]=="table"){
         create_table(word);
     }
     else if(word[1]=="index"){
         create_index(word);
     }
     else if(word[1]=="user"){
         create_user(word);
     }
   }
   else if(word[0]=="drop"){
     if(word[1]=="table"){
         drop_table(word);
     }
     else if(word[1]=="index"){
        drop_index(word);
     }
   }
   else if(word[0]=="insert"){
         insert_into(word);
   }
   else if(word[0]=="alter"){
       if(word[3]=="add"){
          alter_add(word);
       }
       else{
          alter_drop(word);
       }
   }
   else if(word[0]=="delete"){
        delete_from(word);
   }
   else if(word[0]=="update"){
        update(word);
   }
   else if(word[0]=="select"){
       select(word);
   }
   else if(word[0]=="grant"){
       grant(word);
   }
   else if(word[0]=="change"){
       if(change_user(word)) cout<<"changing successful!"<<endl;
   }
   else if(word[0]=="revoke"){
        revoke(word);
   }
}

int main(){
    char sql[200];
    string user,password;
    while(true){
        printf("Enter user name: "); cin>>user;
        printf("Enter password: "); cin>>password;
        if(login(user,password)) break;
        puts("user or password incorrect!\n");
    }
    getchar();
    while(true){
        cout<<"\n"<<USER<<"@> ";
        gets(sql);
        if(strcmp(sql,"exit;")==0){
            printf("~Bye!");
            break;
        }
        if(!yacc(sql)){
            puts("illegal SQL statement!");
            continue;
        }
        vector<string> word=lex(sql);
        excute(word);
    }
    return 0;
}
