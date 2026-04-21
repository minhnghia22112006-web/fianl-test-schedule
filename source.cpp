#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

int ns, nm;
vector<vector<int>> a, c;

// ===== ĐỌC INPUT =====
void rd()
{
    cin >> ns >> nm;
    a.resize(ns);
    for (int i = 0; i < ns; i++)
    {
        int k;
        cin >> k;
        a[i].resize(k);
        for (int j = 0; j < k; j++)
            cin >> a[i][j];
    }
}

// ===== BUILD =====
void bd()
{
    c.assign(nm, vector<int>(nm, 0));

    for (auto &s : a)
    {
        for (int i = 0; i < s.size(); i++)
        {
            for (int j = i + 1; j < s.size(); j++)
            {
                int x = s[i] - 1;
                int y = s[j] - 1;
                c[x][y] = c[y][x] = 1;
            }
        }
    }
}

// ===== CHECK =====
bool ok(int u, int col, const vector<int> &s)
{
    for (int i = 0; i < nm; i++)
        if (c[u][i] && s[i] == col)
            return false;
    return true;
}

// ===== BACKTRACK =====
int best;
vector<int> ans;

void bt(int u, vector<int> &s, int used)
{
    if (u == nm)
    {
        if (used < best)
        {
            best = used;
            ans = s;
        }
        return;
    }

    for (int col = 1; col <= best; col++)
    {
        if (ok(u, col, s))
        {
            s[u] = col;
            bt(u + 1, s, max(used, col));
            s[u] = 0;
        }
    }
}

vector<int> solveBT()
{
    vector<int> s(nm, 0);
    best = nm;
    bt(0, s, 0);
    return ans;
}

// ===== DP =====
vector<bool> memo;

void findIndSet(int u, vector<bool> &curr, vector<bool> &assigned)
{
    if (u == nm)
    {
        if (count(curr.begin(), curr.end(), true) >
            count(memo.begin(), memo.end(), true))
            memo = curr;
        return;
    }

    if (assigned[u])
    {
        findIndSet(u + 1, curr, assigned);
        return;
    }

    bool canAdd = true;
    for (int i = 0; i < nm; i++)
        if (curr[i] && c[u][i])
        {
            canAdd = false;
            break;
        }

    if (canAdd)
    {
        curr[u] = true;
        findIndSet(u + 1, curr, assigned);
        curr[u] = false;
    }

    findIndSet(u + 1, curr, assigned);
}

vector<int> solveDP()
{
    vector<int> result(nm, 0);
    vector<bool> assigned(nm, false);
    int color = 1;

    while (true)
    {
        memo.assign(nm, false);
        vector<bool> curr(nm, false);
        findIndSet(0, curr, assigned);

        if (count(memo.begin(), memo.end(), true) == 0)
            break;

        for (int i = 0; i < nm; i++)
            if (memo[i])
            {
                result[i] = color;
                assigned[i] = true;
            }

        color++;
    }

    return result;
}

// ===== GREEDY =====
vector<int> solveGreedy()
{
    vector<int> result(nm, 0), degree(nm, 0);

    for (int i = 0; i < nm; i++)
        for (int j = 0; j < nm; j++)
            if (c[i][j])
                degree[i]++;

    vector<pair<int, int>> sorted;
    for (int i = 0; i < nm; i++)
        sorted.push_back({degree[i], i});

    sort(sorted.rbegin(), sorted.rend());

    for (auto &p : sorted)
    {
        int u = p.second;
        int col = 1;
        while (!ok(u, col, result))
            col++;
        result[u] = col;
    }

    return result;
}

// ===== PRINT =====
void pl(const vector<int> &s)
{
    int mx = *max_element(s.begin(), s.end());

    cout << "\nSo ca thi: " << mx << "\n";

    for (int i = 1; i <= mx; i++)
    {
        cout << "Ca " << i << ": ";
        for (int j = 0; j < nm; j++)
            if (s[j] == i)
                cout << j + 1 << " ";
        cout << "\n";
    }
}

// ===== MAIN =====
int main()
{
    int t;
    cin >> t;

    for (int tc = 1; tc <= t; tc++)
    {
        cout << "==== TEST " << tc << " ====\n";

        a.clear();
        c.clear();
        ans.clear();

        rd();
        bd();

        // ===== BACKTRACKING =====
        auto start = high_resolution_clock::now();
        vector<int> resBT = solveBT();
        auto end = high_resolution_clock::now();
        auto timeBT = duration_cast<milliseconds>(end - start);

        cout << "\n--- Backtracking ---";
        pl(resBT);
        cout << "Time: " << timeBT.count() << " ms\n";

        // ===== DP =====
        start = high_resolution_clock::now();
        vector<int> resDP = solveDP();
        end = high_resolution_clock::now();
        auto timeDP = duration_cast<milliseconds>(end - start);

        cout << "\n--- Dynamic Programming ---";
        pl(resDP);
        cout << "Time: " << timeDP.count() << " ms\n";

        // ===== GREEDY =====
        start = high_resolution_clock::now();
        vector<int> resGreedy = solveGreedy();
        end = high_resolution_clock::now();
        auto timeGreedy = duration_cast<milliseconds>(end - start);

        cout << "\n--- Greedy ---";
        pl(resGreedy);
        cout << "Time: " << timeGreedy.count() << " ms\n";
    }
    return 0;
}
