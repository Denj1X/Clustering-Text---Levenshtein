/// Pentru a calcula distanta Levenshtein dintre doua cuvinte
/// vom avea o functie care va utiliza programare dinamica:
/// vom construi o matrice unde dp[i][j] = distanta dintre
/// primele i litere ale primului cuvant si primele j litere ale celui de-al doilea;

///Vom avea N cuvinte, MAX lungimea celui mai lung cuvant
///Apoi facem graful complet, sub forma unei liste de muchii
///Facem sortarea muchiilor
///Apoi parcurgem lista si facem un Kruskal pe cuvinte
///Unde vom face n-k reuniuni
///Apoi cu un for fortat, cu worst case O(n^2), dar best case O(1)
///Vom gasi distanta minima intre 2 componente din cele k
///Complexitate Levenshtein pentru N cuvinte: O((N * MAX)^2)
///Complexitate sortare lista de muchii: O(N^2 * log N^2)
///Complexitate Kruskal cu n-k reuniuni: O(N^2 * log N^2)
///Nefacand cu euristica, nu o voi mai mentiona
///Gasire simple linkeage + afisare k componente: O(N^2)
///Complexitate finala: O(max(((N * MAX)^2), (N^2 * log N^2)))


#include <bits/stdc++.h>

using namespace std;
ifstream f("cuvinte.in");

///functia de minim pt 3 numere
int min(int x, int y, int z) {
    return min(min(x, y), z);
}

///distanta Levenshtein cu PD
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

vector<string> vec;
vector<pair<int, pair<int, int> > >gr;
vector<int> parent, viz;

///gasirea stramosului
int disfind(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

///reuniunea componentelor
void disreuniune(int x, int y) {
    int i = disfind(x);
    int j = disfind(y);
    parent[i] = parent[j];
}

int main() {
    int k;
    cin >> k;
    string s;
    while(f >> s) {
        vec.push_back(s);
    }
    ///citirea cuvintelor

    int n = vec.size();
    ///crearea grafului complet ponderat
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            int cost = Levenshtein(vec[i], vec[j], (int) vec[i].length(), (int) vec[j].length());
            gr.push_back(make_pair(cost, make_pair(i, j)));
        }
    }

    parent.resize(n+1);
    ///sortam lista de muchii dupa cost
    sort(gr.begin(), gr.end());

    ///initializarea vectorului de stramos
    for(int i = 0; i < n; i++) {
        parent[i] = i;
    }

    ///parcurgerea listei de muchii pana la n-k reuniuni
    int linkeage = 1e9, val = 0;
    for(auto el : gr) {
        int x = el.second.first;
        int y = el.second.second;
        if(val >= n - k)
            break;
        if(disfind(x) != disfind(y)) {
            disreuniune(x, y);
            val++;
        }
    }

    ///un for fortat pentru gasirea simple linkeage-ului pt Cluster
    for(auto el : gr) {
        if(el.first < linkeage && disfind(el.second.first) != disfind(el.second.second)) {
            cout << el.first << '\n';
            break;
        }
    }

    viz.resize(n + 1, 0);
    ///afisarea celor k componente date
    for(int i = 0; i < n; i++) {
        int ok = 0;
        for(int j = 0; j < n; j++) {
            if((parent[j] == i) && !viz[j]) {
                cout << vec[j] << " ";
                viz[j] = 1;
                ok = 1;
            }
        }
        ///acest ok este pentru afisare corecta, ordonata
        if(ok)
            cout << '\n';
    }
    return 0;
}
