#include <bits/stdc++.h>
using namespace std;
vector<string> stringToVector(string str,char delim) {
    int i = 0;
    vector<string> result;
    while(i < str.length()) {
        int j = i+1;
        while(j < str.length() && str[j] != delim) j++;
        string s = str.substr(i,j-i);
        i = j+1;
        result.push_back(s);
    }
    return result;
}
string concatStates(unordered_set<string>& s) {
    string res = "";
    for(auto& str:s) {
        res += str+",";
    }
    res.pop_back();
    return res;
}
int main() {
    ifstream f("input.txt");
    unordered_set<string> finalStates;
    string initialState;
    vector<vector<string>> table;
    string str;
    getline(f,str);
    initialState = str;
    getline(f,str);
    vector<string> t = stringToVector(str,',');
    finalStates.insert(t.begin(),t.end());
    
    while(f.good()) {
        getline(f,str);
        t = stringToVector(str,' ');
        table.push_back(t);
    }
    f.clear();
    f.seekg(0);
    
    //conversion from nfa to dfa
    unordered_map<string,string> changedStates;
    int i = 0;
    int j = table.size();
    int num = table[0].size();  //number of alphabets
    while(i != j) {
        for(;i < j; i++) {
            for(auto& s: table[i]) {
                string temp = s;
                sort(temp.begin(),temp.end());
                if(s[0] != '-' && s.find(',') != string::npos) {
                    if(changedStates.find(temp) == changedStates.end()) {
                        vector<string> t = stringToVector(s,',');
                        changedStates[temp] = to_string(table.size());
                        bool isFinal = false;
                        table.push_back({});
                        for(int k=0;k < num; k++) {
                            unordered_set<string> states;
                            for(string& st:t) {
                                int index = stoi(st);   
                                if(finalStates.find(st) != finalStates.end()) isFinal = true;
                                if(table[index][k][0] != '-') {
                                    for(string temp:stringToVector(table[index][k],',')) {        
                                        states.insert(temp);
                                }
                            }
                        }              
                        if(!states.empty()) {
                                string state = concatStates(states);
                                table.back().push_back(state);    
                            }
                        else {
                                table.back().push_back("-1");
                            }
                        }
                        if(isFinal){
                             finalStates.insert(s);
                        }
                    }    
                }
            }     
        }
        i = j;
        j = table.size();
    } 
    for(auto& v:table) {
        for(string& stri:v) {
            string t = stri;
            sort(t.begin(),t.end());
            if(changedStates.find(t) != changedStates.end()) {
                stri = changedStates[t];
            }
        }
     }
     cout<<initialState<<endl;
     for(auto& stri: finalStates) {
            string t = stri;
            sort(t.begin(),t.end());
            if(changedStates.find(t) != changedStates.end()) {
                cout<<changedStates[t]<<" ";
            }else {
                cout<<stri<<" ";
            }
            
     }
     cout<<endl;
     for(auto& v:table) {
        for(string& stri:v) {
            cout<<stri<<" ";
        }
        cout<<endl;
     }
}