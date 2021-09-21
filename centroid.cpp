// Centroid decomposition
// Watch https://codeforces.com/contest/342/submission/91165516 for more info about this code

#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
 
using namespace std;
 
const int INF = 1e9;
 
int n, m;
 
vector<vector<int>> g, asktime;
vector<int> painttime, answer, shortestpathatmoment, shortestpathtocentroid, compress, compressquery, c, cq;
vector<bool> isdeleted;
 
void docompress(int curr, int p, int& iteration, set<int>& currentqueries)
{
	compress[curr] = iteration;
	c[iteration] = curr;
	++iteration;
	if (painttime[curr] != INF)
	{
		currentqueries.insert(painttime[curr]);
	}
	for (int i : asktime[curr])
	{
		currentqueries.insert(i);
	}
	for (int i : g[curr])
	{
		if (i != p && !isdeleted[i])
		{
			docompress(i, curr, iteration, currentqueries);
		}
	}
}
 
void getweight(int curr, int p, vector<int>& weight)
{
	for (int i : g[curr])
	{
		if (i != p && !isdeleted[i])
		{
			getweight(i, curr, weight);
			weight[compress[curr]] += weight[compress[i]];
		}
	}
}
 
int findcentroid(int curr, int p, int nlocal, const vector<int>& weight)
{
	for (int i : g[curr])
	{
		if (!isdeleted[i] && i != p && weight[compress[i]] > nlocal / 2)
		{
			return findcentroid(i, curr, nlocal, weight);
		}
	}
	return curr;
}
 
void dfs(int curr, int p, int depth)
{
	shortestpathtocentroid[compress[curr]] = depth;
	if (painttime[curr] != INF)
	{
		shortestpathatmoment[compressquery[painttime[curr]]] = min(shortestpathatmoment[compressquery[painttime[curr]]], depth);
	}
	for (int i : g[curr])
	{
		if (i != p && !isdeleted[i])
		{
			dfs(i, curr, depth + 1);
		}
	}
}
 
void solve(int currvert, int nlocal)
{
	int iteration = 0, iterationquery = 0, mlocal;
	vector<int> weight(nlocal, 1);
	set<int> currentqueries;
	c.clear();
	c.resize(nlocal);
	cq.clear();
	docompress(currvert, -1, iteration, currentqueries);
	for (int i : currentqueries)
	{
		cq.emplace_back(i);
		compressquery[i] = iterationquery;
		++iterationquery;
	}
	mlocal = iterationquery;
	getweight(currvert, -1, weight);
	int centroid = findcentroid(currvert, -1, nlocal, weight);
	shortestpathatmoment.clear();
	shortestpathatmoment.resize(mlocal, INF);
	shortestpathtocentroid.clear();
	shortestpathtocentroid.resize(nlocal, INF);
 
	dfs(centroid, -1, 0);
	for (int i = 1; i < mlocal; ++i)
	{
		shortestpathatmoment[i] = min(shortestpathatmoment[i - 1], shortestpathatmoment[i]);
	}
	for (int i = 0; i < nlocal; ++i)
	{
		for (int j : asktime[c[i]])
		{
			answer[j] = min(answer[j], shortestpathtocentroid[i] + shortestpathatmoment[compressquery[j]]);
		}
	}
 
	isdeleted[centroid] = true;
	for (int i : g[centroid])
	{
		if (!isdeleted[i])
		{
			solve(i, weight[compress[i]]);
		}
	}
}
 
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
 
	int a, b, t, v;
	cin >> n >> m;
	g.resize(n);
	for (int i = 0; i < n - 1; ++i)
	{
		cin >> a >> b;
		--a;
		--b;
		g[a].emplace_back(b);
		g[b].emplace_back(a);
	}
 
	asktime.resize(n);
	painttime.resize(n, INF);
	answer.resize(m, INF);
	painttime[0] = 0;
	for (int i = 0; i < m; ++i)
	{
		cin >> t >> v;
		--v;
		if (t == 1 && painttime[v] == INF)
		{
			painttime[v] = i;
		}
		if (t == 2)
		{
			asktime[v].emplace_back(i);
		}
	}
 
	isdeleted.resize(n, false);
	compress.resize(n);
	compressquery.resize(m);
	solve(0, n);
 
	for (int i : answer)
	{
		if (i != INF)
		{
			cout << i << '\n';
		}
	}
}
