#include <bits/stdc++.h>

using namespace std;

ifstream f("cuvinte.in");

///min function for 3 values
int min(int x, int y, int z) {
    return min(min(x, y), z);
}

///editing distance
int Levenshtein(string str1, string str2, int m, int n) {
    int dp[m + 1][n + 1];
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0)
                dp[i][j] = j;
            else if (j == 0)
                dp[i][j] = i;
            else if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min(dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1]);
        }
    }
    return dp[m][n];
}


char *sir, *p, *elem;
vector<string> vec;
vector<pair<int, pair<int, int> > >gr;
vector<int> parent, viz;

///find the root for a node
int disfind(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

///the reunion of connected components
///which include two given nodes
void disreuniune(int x, int y) {
    int i = disfind(x);
    int j = disfind(y);
    parent[i] = parent[j];
}

///DFS function
void dfs(const int &node, vector<int> *comp) {
    viz[node] = 1;
    for(auto vecin : comp[node]) {
        if(!viz[vecin])
            dfs(vecin, comp);
    }
}

int main() {
    int k;
    cin >> k;
    f.get(sir, 10000);
    p = strtok(sir, " ");

    ///taking each word as a node
    while(p != NULL) {
        strcpy(elem, p);
        vec.push_back(elem);
        p = strtok(NULL, " ");
    }

    ///creating the complete undirected weighted graph
    for(int i = 1; i < vec.size(); i++) {
        for(int j = 0; j < i; j++) {
            int cost = Levenshtein(vec[i], vec[j], (int) vec[i].length(), (int) vec[j].length());
            gr.push_back(make_pair(cost, make_pair(i, j)));
        }
    }

    int n = vec.size();
    parent.resize(n+1);
    vector<int> comp[n+1];
    ///ascending sort of vertices after cost
    sort(gr.begin(), gr.end());

    for(int i = 0; i < n; i++) {
        parent[i] = i;
    }

    int linkeage = 0, val = 0;
    for(auto el : gr) {
        if(val < n - k) {
            int cost = el.first;
            int x = el.second.first;
            int y = el.second.second;
            if (disfind(x) != disfind(y)) {
                disreuniune(x, y);
                comp[x].push_back(y);
                comp[y].push_back(x);
                linkeage = cost;
            }
            val++;
        }
    }

    viz.resize(n + 1, 0);

    for(int i = 0; i < n; i++) {
        if(!viz[i])
            dfs(i, comp);
    }
    ///TODO: find a way to print the words as nodes

    return 0;
}
