#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
using namespace std;

bool minterms(string filename, int& var, vector<int>& minterm, vector<int>& dontcare){
    ifstream r;
    string line, minn, donn, empty1, empty2;
    r.open(filename);
    r >> var;
    r >> minn;
    r >> donn;
    int c = 0;
    istringstream is1(minn);
    istringstream is2(donn);
    while(getline( is1 , empty1 , ','))
    {
        minterm.push_back(stoi(empty1));
    }
    while(getline( is2 , empty2 , ','))
    {
        dontcare.push_back(stoi(empty2));
    }
    for (int i = 0; i<minterm.size(); i++){
        if((minterm[i] >= pow(2,var))){
            cout << "Based on the number of variables in the function, " << minterm[i] << " is not a valid minterm"<<endl;
            c = 1;
        }
    }
   
    for (int i = 0; i<dontcare.size(); i++){
        if((dontcare[i] >= pow(2,var))){
        cout << "Based on the number of variables in the function, " << dontcare[i] << " is not a valid don't care term"<<endl;
            c = 1;
        }
    }
   
    r.close();
    if (c == 1){
        return false;
    }
    else{
        return true;
    }
}

vector<pair<string,string>> dec_to_bin(int var, vector<int>& minterm){
    int s,n;
    vector<pair<string,string>> m(minterm.size());
    string r="";
    for(int i=0;i<minterm.size();i++)
    {
        cout<<"M "<<minterm[i];
        n=minterm[i];
        r="";
        while(n!=0){
            r=(n%2==0 ?"0":"1")+r;
            n/=2;
        }
        s=r.length();
        while(var>s){
            r="0"+r;
            s++;
        }
        m[i].first=r;
        m[i].second=to_string(minterm[i]);
        cout<<"     "<<m[i].first<<endl;
    }
    cout<<"-------------------------------"<<endl;
    return m;
}

int count_ones(int s,string x) {
    int count = 0;
    for (int i=0;i<s;i++){
        if(x[i]=='1')
            count=count+1;
    }
    return count;
}

bool found (int x, string y){
    istringstream i1(y);
    string empty;
    vector <int> yes;
    while(getline( i1 , empty, ','))
    {
        yes.push_back(stoi(empty));
    }
    for (int j = 0; j < yes.size(); j++){
        if (x == yes[j]){
            return true;
        }
    }
    return false;
}


vector<pair<string,string>> comparison (int v, vector<pair<string,string>> a, vector<pair<string,string>> b, vector<bool>& check_a,vector<bool>& check_b){
    vector<pair<string,string>> group,changed;
    string one,two;
    vector<bool> check_bb;
    vector<bool> check_aa;
    int flag=0,at=0,repetition=0;
    check_a.resize(a.size(), false);
    check_b.resize(b.size(), false);
    check_bb.resize(b.size(), true);
    check_aa.resize(a.size(), true);
    for(int x=0;x<a.size();x++){
        flag=0;
        at=0;
        //check_a.push_back(false);
        repetition=0;
        for(int y=0;y<b.size();y++){
            //check_b.push_back(false);
            one=a[x].first;
            two=b[y].first;
            changed=b;
            flag=0;
            at=0;
            repetition=0;
            for(int i=0;i<v;i++){
                if(one[i]==two[i])
                    flag=flag+1;
                else
                    at=i;
                }
            if(flag+1==v){
                check_a[x]=false;
                check_b[y]=false;
                check_aa[x]=false;
                check_bb[y]=false;
                changed[y].first[at]='_';
                changed[y].second = a[x].second+","+b[y].second;
                for(int j=0;j<group.size();j++){
                    if(group[j]==changed[y])
                        repetition=1;
                }
                if(repetition==0)
                    group.push_back(changed[y]);
                flag=0;
                at=0;
            }
            else{
                check_a[x]=true;
                //if (y == b.size()-1)
                check_b[y]=true;
            }
        }
    }
    for(int i=0;i<check_aa.size();i++){
        if(check_aa[i]==false)
            check_a[i]=false;
    }
    for(int i=0;i<check_bb.size();i++){
        if(check_bb[i]==false)
            check_b[i]=false;
    }
    return group;
}

int main(){
    int v,s;
    string name;
    vector<int> m;
    vector<int> d;
    vector<pair<string,string>> mm;
    vector<pair<string,string>> dd;
    vector<vector<pair<string,string>>> groups;
    vector<vector<vector<pair<string,string>>>> itterations;
    vector<pair<string,string>> PI;
    bool b;
    b = minterms("/Users/andrewsinout/Desktop/Project QM/Project QM/DigitalData.txt", v, m, d);
    if (b == true){
        cout<<"The minterms:"<<endl;
        mm = dec_to_bin(v,m);
        cout<<"The don't care terms:"<<endl;
        dd = dec_to_bin(v,d);
        //cout<<"hereee  "<<count_ones(v,mm.at(3).first)<<endl;
        
        /*for(int i=0;i<=v;i++){
            vector<string> groupi(0);
            groups.push_back(groupi);
        }*/
        for(int i=0;i<=v;i++){
            vector<pair<string,string>> groupi;
            for(int j=0;j<mm.size();j++){
                s=count_ones(v,mm[j].first);
                if(s==i)
                    groupi.push_back(mm[j]);
            }
            
            for(int j=0;j<dd.size();j++){
                s=count_ones(v,dd[j].first);
                if(s==i)
                    groupi.push_back(dd[j]);
            }
            cout<<"group"<<i<<" :"<<endl;
            for (int x = 0;x<groupi.size();x++) {
                cout<<groupi[x].first<<"("<<groupi[x].second<<")"<<endl;
            }
            cout<<"------"<<endl;
            groups.push_back(groupi);
        }
        
        bool empty=false;
        vector<bool> check_a;
        vector<bool> check_b;
        vector<bool> check_c;
        itterations.push_back(groups);
        int n=0,var=v;
        while(itterations[n].size()!=0){
            vector<vector<pair<string,string>>> combined_groups;
            for(int i=0;i<var;i++){
                vector<pair<string,string>> combined_groupi;
                combined_groups.push_back(comparison(v,itterations[n][i],itterations[n][i+1],check_a,check_b));
                combined_groupi=combined_groups[i];
                cout<<"combined group"<<i<<" :"<<endl;
                for (int x = 0;x<combined_groupi.size();x++) {
                    cout<<combined_groupi[x].first<<"("<<combined_groupi[x].second<<")"<<endl;
                }
                cout<<"------"<<endl;
                if(i==0){
                    check_c=check_a;
                }
                else{
                    for(int y=0;y<check_a.size();y++){
                        if(check_a[y]==false){
                            check_c[y]=check_a[y];
                        }
                    }
                }
                for(int y=0;y<check_c.size();y++){
                    if((check_c[y]==true)){
                        PI.push_back(itterations[n][i][y]);
                    }
                }
                check_c.clear();
                check_c=check_b;
                check_b.clear();
                check_a.clear();
            }
            for(int y=0;y<check_c.size();y++){
                if((check_c[y]==true)){
                    PI.push_back(itterations[n][var][y]);
                }
            }
            itterations.push_back(combined_groups);
            n++;
            var--;
            check_c.clear();
            for(int i=0;i<itterations[n-1].size();i++){
                if(itterations[n-1][i].size()!=0)
                    empty=true;
            }
            if(empty==false)
                break;
            empty=false;
        }
        for(int i=0;i<itterations[n-2].size();i++){
            for(int y=0;y<itterations[n-2][i].size();y++){
                    PI.push_back(itterations[n-2][i][y]);
                }
        }
        
        empty=false;
        vector<pair<string,string>> PII;
        for(int i=0;i<PI.size();i++){
            for(int j=i+1;j<PI.size();j++){
                if(PI[i].first==PI[j].first){
                    empty=true;
                }
            }
            if(empty==false)
                PII.push_back(PI[i]);
            empty=false;
        }
        cout<<endl<<"The prime implicatns :"<<endl;
        for (int i = 0;i<PII.size();i++) {
            cout<<PII[i].first<<"("<<PII[i].second<<")"<<endl;
        }
        
        cout<<"----------------------------"<<endl;
        cout<<endl<<"The boolean equation for the prime implicatns :"<<endl;
        string L="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for(int j=0;j<PII.size();j++){
            for(int i=0;i<v;i++){
                if(PII[j].first[i]=='0')
                    cout<<L[i]<<"'";
                if(PII[j].first[i]=='1')
                    cout<<L[i];
            }
            if(j!=PII.size()-1)
                cout<<"+";
        }

        
        cout<<endl<<endl<<"----------------------------"<<endl;
        cout<<endl<<"Essential prime implicants :"<<endl<<endl;
                vector<pair<string, string>> EPI;
                vector <int> index;

                for (int i=0;i<m.size();i++){
                    int counter=0;
                    for (int j=0;j<PII.size();j++){
                        if (found(m[i],PII[j].second)==true){
                            counter++;
                            index.push_back(j);
                        }
                    }
                    if (counter==1){
                        
                        EPI.push_back(PII[index[0]]);
                    }
                    index.clear();
                }
                for (int i=0;i<EPI.size();i++) {
                    cout<<EPI[i].first<<"("<<EPI[i].second<<")"<<endl;
                }

        cout<<"----------------------------"<<endl;
        cout<<endl<<"The boolean equation for the Essential prime implicatns :"<<endl;
        for(int j=0;j<EPI.size();j++){
            for(int i=0;i<v;i++){
                if(EPI[j].first[i]=='0')
                    cout<<L[i]<<"'";
                if(EPI[j].first[i]=='1')
                    cout<<L[i];
            }
            if(j!=EPI.size()-1)
                cout<<"+";
        }
        cout<<endl;
        
        string f;
        vector<string> r;
        for(int i=0;i<EPI.size();i++){
            istringstream g(EPI[i].second);
            while(getline(g,f,',')){
                r.push_back(f);
            }
        }
        
        for(int i=0;i<m.size();i++)
            for(int j=0;j<r.size();j++){
                if(m[i]==stoi(r[j])){
                    m.erase(m.begin()+i);
                    i--;
                }
            }
        
        cout<<"----------------"<<endl;
        cout<<"----------------"<<endl;

        for(int i=0;i<m.size();i++)
            cout<<m[i]<<endl;
        
        cout<<"----------------"<<endl;
        cout<<"----------------"<<endl;
        
        
        empty=false;
        vector<pair<string,string>> PI_C;
        for(int i=0;i<PII.size();i++){
            for(int j=0;j<EPI.size();j++){
                if(PII[i].first==EPI[j].first){
                    empty=true;
                }
            }
            if(empty==false)
                PI_C.push_back(PII[i]);
            empty=false;
        }
        cout<<endl<<"The prime implicatns after removing the essential prime implicants :"<<endl;
        for (int i = 0;i<PI_C.size();i++) {
            cout<<PI_C[i].first<<"("<<PI_C[i].second<<")"<<endl;
        }
        
        cout<<"----------------"<<endl;
        cout<<"----------------"<<endl;
        
        for (int j=0;j<m.size();j++){
            int counter=0;
            for (int i=0;i<PI_C.size();i++){
                if (found(m[j],PI_C[i].second)==true){
                    counter++;
                }
            }
            if (counter==PI_C.size()){
                    m.erase(m.begin()+j);
            }
        }
        cout<<endl<<"Essential prime implicatns After the dominationg colomns table :"<<endl;
        for (int i=0;i<EPI.size();i++) {
            cout<<EPI[i].first<<"("<<EPI[i].second<<")"<<endl;
        }
        
        cout<<"----------------------------"<<endl;
        cout<<endl<<"The boolean equation for the Essential prime implicatns After the dominationg colomns :"<<endl;
        for(int j=0;j<EPI.size();j++){
            for(int i=0;i<v;i++){
                if(EPI[j].first[i]=='0')
                    cout<<L[i]<<"'";
                if(EPI[j].first[i]=='1')
                    cout<<L[i];
            }
            if(j!=EPI.size()-1)
                cout<<"+";
        }
        cout<<endl;
        
        cout<<"----------------"<<endl;
        cout<<"----------------"<<endl;
        
        bool check=false;
        for (int i=0;i<PI_C.size();i++){
            int counter=0;
            for (int j=0;j<m.size();j++){
                if (found(m[j],PI_C[i].second)==true){
                    counter++;
                }
            }
            if (counter==m.size()){
                EPI.push_back(PI_C[i]);
                check=true;
            }
        }
        
        if (check==false){
            for (int j=0;j<m.size();j++){
                for (int i=0;i<PI_C.size();i++){
                    if (found(m[j],PI_C[i].second)==true){
                        EPI.push_back(PI_C[i]);
                        break;
                    }
                }
            }
        }
        
        cout<<endl<<"Essential prime implicatns After the coverage table :"<<endl;
        for (int i=0;i<EPI.size();i++) {
            cout<<EPI[i].first<<"("<<EPI[i].second<<")"<<endl;
        }

        cout<<"----------------------------"<<endl;
        cout<<endl<<"The boolean equation for the Essential prime implicatns After the coverage table :"<<endl;
        for(int j=0;j<EPI.size();j++){
            for(int i=0;i<v;i++){
                if(EPI[j].first[i]=='0')
                    cout<<L[i]<<"'";
                if(EPI[j].first[i]=='1')
                    cout<<L[i];
            }
            if(j!=EPI.size()-1)
                cout<<"+";
        }
        cout<<endl;
        
    }
    else{
    cout << "Data in file is not correct according to the number of variables present please change the data in the file and try again"<< endl;
    }
    return 0;
}
