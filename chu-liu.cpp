// Chu-Liu/Edmonds' algorithm for max spanning tree

#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#include<stack>

using namespace std;

using ll = long long;

int dfs(int curr, const vector<vector<int>>& g, vector<bool>& used)
{
	used[curr] = true;
	int ret = 1;
	for (int i : g[curr])
	{
		if (!used[i])
		{
			ret += dfs(i, g, used);
		}
	}
	return ret;
}

void merge(int fi, int se, vector<set<pair<ll, int>>>& g2, vector<ll>& d, vector<int>& dsu)
{
	if ((int)g2[se].size() < (int)g2[fi].size())
	{
		swap(fi, se);
	}
	for (auto i : g2[fi])
	{
		g2[se].insert({ i.first + d[fi] - d[se], i.second });
	}
	g2[fi].clear();
	dsu[fi] = se;
}

int get_head(int x, vector<int>& dsu)
{
	if (dsu[x] == -1)
	{
		return x;
	}
	return dsu[x] = get_head(dsu[x], dsu);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n >> m;
	vector<vector<int>> g(n);
	vector<set<pair<ll, int>>> g2(n);
	for (int i = 0; i < m; ++i)
	{
		int u, v;
		ll w;
		cin >> u >> v >> w;
		--u;
		--v;
		g[u].emplace_back(v);
		g2[v].insert({ w, u });
	}
	vector<bool> used(n, false);
	if (dfs(0, g, used) < n)
	{
		cout << "NO" << '\n';
		exit(0);
	}
	ll ans = 0;
	vector<ll> d(n, 0);
	vector<int> used2(n, -1), is_connected_to_root(n, false);
	is_connected_to_root[0] = true;
	vector<int> dsu(n, -1);
	for (int i = 1; i < n; ++i)
	{
		if (is_connected_to_root[i])
		{
			continue;
		}
		stack<int> p;
		p.push(i);
		while (!is_connected_to_root[get_head(p.top(), dsu)])
		{
			int v = get_head(p.top(), dsu);
			used2[v] = i;
			while (get_head((*g2[v].begin()).second, dsu) == get_head(v, dsu))
			{
				g2[v].erase(g2[v].begin());
			}
			ans += (*g2[v].begin()).first + d[v];
			d[v] -= (*g2[v].begin()).first + d[v];
			int u = get_head((*g2[v].begin()).second, dsu);
			if (used2[u] != i)
			{
				p.push(u);
				continue;
			}
			while (get_head(p.top(), dsu) != get_head(u, dsu))
			{
				merge(get_head(p.top(), dsu), get_head(u, dsu), g2, d, dsu);
				p.pop();
			}
		}
		while (!p.empty())
		{
			is_connected_to_root[p.top()] = true;
			p.pop();
		}
	}
	cout << "YES" << '\n' << ans << '\n';
}
