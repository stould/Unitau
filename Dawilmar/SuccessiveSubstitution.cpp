#include <bits/stdc++.h>
#define MAXN_RULES 1000
using namespace std;

/*
    Will be accepted for substitution, the following steps:
    'X -> Y'
    More informally without quotes: X->Y, these symbols should not have spaces
    Where X is some string and Y is some another string.
    The program will not handle cases where X = Y.
    Is defined 'S' as the initial state and '$' as end state.
    E.g.:

*/

int N, M;

//from rule, to rule

vector<vector<int> > graph(MAXN_RULES);
vector<string> rule_ref;
map<string, int> indexer;
int INDEX;

pair<string, string> splitRule(string &s){
    string from = "", to = "";
    int i = 0;
    while(i < (int) s.size()){
        if(s[i] == '-' && s[i+1] == '>'){
            i += 2;
            break;
        }
        from += string(1, s[i]);
        i++;
    }
    for(; i < (int) s.size() && s[i] != '\n' && s[i] != '\r'; i++){
        to += string(1, s[i]);
    }
    return make_pair(from, to);
}

/*
    Reading from standard input
    first line: N -> amount of rules to read
    next N lines, the X->Y rules
    next line an int M indicating the amount of matches to check
    next line a string T$.

    "
        Note:
        If from a random state X falls on state X1, X2,...,Xn,
        The algorithm will handle the input as:
        X->X1
        X->X2
        ...
        X->Xn

        Will not be accepted 'X->X1|X2|...|Xn'.
    "

    Example of acceptable format of input:
    7
    S->E
    E->E+T
    E->T
    T->T*F
    T->F
    F->(E)
    F->id
    2
    (id+id)$
    ((((idsid))))$
*/
void readRules_andBuildGraph(){
    string rule;
    cin >> N;
    cin.ignore();
    for(int i = 0; i < N; i++){
        getline(cin, rule);
        pair<string, string> r = splitRule(rule);
        int from, to;
        if(indexer.find(r.first) == indexer.end()){
            from = INDEX++;
            indexer[r.first] = from;
            rule_ref.push_back(r.first);
        }else{
            from = indexer[r.first];
        }
        if(indexer.find(r.second) == indexer.end()){
            to = INDEX++;
            indexer[r.second] = to;
            rule_ref.push_back(r.second);
        }else{
            to = indexer[r.second];
        }
        //cout << "#" <<rule_ref[from] << "#" << " to "  << "#" << rule_ref[to] << "#"<< endl;
        graph[from].push_back(to);
    }
}

string T;
bool flag;

//Max of 1e6 steps to calculate.
const int MAX_DEPTH = 9;

//The successive substituition algorithm
void process(int pos, string Z, int dep, stack<char> st){
    if(flag || dep > MAX_DEPTH){
        //If found or the depth is too deep, stop
        return;
    }
    if(Z == "$" && st.size() == 0){
        //if a final state is reached and the stack is empty, stop
        flag = true;
        return;
    }

    //copying the stack and changing the copied one
    stack<char> tmp(st);
    if(pos != 0){
        for(int i = rule_ref[pos].size() - 1; i >= 0; i--){
            tmp.push(rule_ref[pos][i]);
        }
    }else{
        return;
    }

    //recursive calls for each element in the stack
    //take one element and change to another or, take more than one element and go on.
    while(!tmp.empty()){
        char topo = tmp.top(); tmp.pop();
        int next = indexer[string(1, topo)];
        if(topo == Z[0]){
            process(pos, Z.substr(1), dep, tmp);
        }else{
            for(int i = 0; i < graph[next].size(); i++){
                if(topo == Z[0]){
                    process(graph[next][i], Z.substr(1), dep+1, tmp);
                }else{
                    process(graph[next][i], Z, dep+1, tmp);
                }
            }
        }
    }
}



int main(){
    INDEX = 1;
    rule_ref.push_back("TRASH");
    readRules_andBuildGraph();
    cin >> M;
    cin.ignore();

    //reading texts T
    while(M--){
        flag = 0;
        getline(cin, T);
        stack<char> v;
        process(indexer["S"], T, 0, v);
        if(flag){
            cout << "The text matches the grammar." << endl;
        }else{
            cout << "Text could not be recognized by grammar." << endl;
        }
    }
    return 0;
}
