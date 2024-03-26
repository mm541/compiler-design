#include <bits/stdc++.h>
using namespace std;
char startSymbol;
unordered_map<char,unordered_set<char>> first;
unordered_map<char,unordered_set<char>> follow;
map<char,vector<pair<char,int>>> occurence;
vector<string> stringToVector(string& str,char delim) {
    int i = 0;
    vector<string> result;
    while(i < str.length()) {
        int j = i+1;
        while(j < str.length() && str[j] != delim) j++;
        string s = str.substr(i,j-i);
        result.push_back(s);
        i = j+1;
    }
    return result;
}
void rearrangeString(char ch,vector<string>& s) {
    int i=0,j=s.size()-1;
    while(i < j) {
        if(s[i].find(ch) != string::npos) {
            string str = s[i];
            s[i] = s[j];
            s[j] = str;
            j--;
        }else {
            i++;
        }
    }
}
void findProductionWhereGivenVariableExist(char ch,string& str,int index) {
    for(auto& c:str) {
        if(c >= 'A' && c <= 'Z') {
            occurence[c].push_back({ch,index});
        }
    }
}
void findFirst(char var, unordered_map<char,vector<string>>& grammar){
    unordered_set<char> terminals;
    bool isEpsilon = false;
    for(auto& st:grammar[var]) {
        if(st[0] == '#') {
            terminals.insert('#');
            isEpsilon = true;
            continue;
        }
        int i;
        for(i=0;i < st.size(); i++) {
            char ch = st[i];
            if(grammar.find(ch) == grammar.end()) {
                terminals.insert(ch);
                break;
            }
            if(var == ch) {
                if(isEpsilon) continue;
                break;
            }
            if(first.find(ch) == first.end()) {
                findFirst(ch,grammar);
            } 
            terminals.insert(first[ch].begin(),first[ch].end());
            if(!isEpsilon) {
                terminals.erase('#');
            }
            if(first[ch].find('#') == first[ch].end()) {
                break;
            }
        }
        if(i == st.size()) {
            isEpsilon = true;
            terminals.insert('#');
        }
    }
    first[var] = terminals; 
}
void printFirst() {
    for(auto& p:first) {
        auto it = p.second.begin();
        cout<<"FIRST("<<p.first<<")"<<" = {"<<(*it);
        it++;
        while(it != p.second.end()) {
            cout<<", "<<(*it);
            it++;
        }
        cout<<"}"<<endl;
    } 
}
void findFollow(char var, unordered_map<char,vector<string>>& grammar) {
    unordered_set<char> terminals;
    if(var == startSymbol) {
        terminals.insert('$');
    }
    for(auto& p:occurence[var]) {
        string str = grammar[p.first][p.second];
        int index = str.find(var)+1;
        for(;index < str.length(); index++) {
            if(str[index] < 'A' || str[index] > 'Z') {
                terminals.insert(str[index]);
                break;
            }
            terminals.insert(first[str[index]].begin(),first[str[index]].end());
            if(terminals.find('#') == terminals.end()) break;
            terminals.erase('#');
        }
        if(index >= str.length() && p.first != var) {
            if(follow.find(p.first) == follow.end())
                findFollow(p.first,grammar);
            terminals.insert(follow[p.first].begin(),follow[p.first].end());
        }
    }
    follow[var] = terminals;
}
void printFollow() {
    for(auto& p:follow) {
        auto it = p.second.begin();
        cout<<"FOLLOW("<<p.first<<")"<<" = {"<<(*it);
        it++;
        while(it != p.second.end()) {
            cout<<", "<<(*it);
            it++;
        }
        cout<<"}"<<endl;
    } 
}
int main() {
    ifstream f("cfg.txt");
    unordered_map<char,vector<string>> grammar;
    bool start = true;
    while(f.good()) {
        string str;
        getline(f,str);
        vector<string> t = stringToVector(str,' ');
        if(start){
            start = false;
            startSymbol = t[0][0];
        }
        grammar[t[0][0]] = stringToVector(t[1],'|');
        rearrangeString(t[0][0],grammar[t[0][0]]);
        for(int i=0; i < grammar[t[0][0]].size(); i++) {
            findProductionWhereGivenVariableExist(t[0][0],grammar[t[0][0]][i],i);
        }
    }
    
    for(auto& p:grammar) {
        if(first.find(p.first) == first.end()) {
            findFirst(p.first,grammar);
        }
    }
    for(auto& p:grammar) {
        if(follow.find(p.first) == follow.end()) {
            findFollow(p.first,grammar);
        }
    }
    // cout<<follow.size()<<endl;
    printFirst();
    printFollow();
}