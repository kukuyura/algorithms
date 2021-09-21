// Dynamic connectivity problem
// Watch https://codeforces.com/gym/100551/submission/85558778 for more info about this code

#include<iostream>
#include<vector>
#include<set>
#include<stack>
#include<fstream>
 
using namespace std;
 
struct query
{
	char t;
	int u, v, ans;
	query() : t('#'), u(-1), v(-1), ans(-1) {}
};
 
struct node
{
	int l, r;
	vector<pair<int, int>> e;
	node* lch, * rch;
	node() : l(-1), r(-1), lch(0), rch(0) {}
};
 
vector<query> q;
vector<int> dsupref, dsurang;
stack<pair<int*, int>> rollback;
int num, a, b;
 
node* build(int l, int r)
{
	node* ret = new node;
	ret->l = l;
	ret->r = r;
	if (l != r)
	{
		ret->lch = build(l, (l + r) / 2);
		ret->rch = build((l + r) / 2 + 1, r);
	}
	else
	{
		ret->lch = 0;
		ret->rch = 0;
	}
	return ret;
}
 
void add(int l, int r, pair<int, int> uv, node* curr)
{
	if (l <= curr->l && curr->r <= r)
	{
		curr->e.push_back(uv);
	}
	else if (l <= curr->r && curr->l <= r)
	{
		add(l, r, uv, curr->lch);
		add(l, r, uv, curr->rch);
	}
}
 
int root(int v)
{
	if (dsupref[v] == -1)
	{
		return v;
	}
	else
	{
		return root(dsupref[v]);
	}
}
 
void merge(int u, int v)
{
	a = root(u);
	b = root(v);
	if (a == b)
	{
		return;
	}
	rollback.push({ &num, num });
	--num;
	if (dsurang[a] > dsurang[b])
	{
		rollback.push({ &dsupref[b], dsupref[b] });
		rollback.push({ &dsurang[a], dsurang[a] });
		dsupref[b] = a;
		dsurang[a] += dsurang[b];
	}
	else
	{
		rollback.push({ &dsupref[a], dsupref[a] });
		rollback.push({ &dsurang[b], dsurang[b] });
		dsupref[a] = b;
		dsurang[b] += dsurang[a];
	}
}
 
void dfs(node* curr)
{
	int mem = rollback.size();
	for (auto i : curr->e)
	{
		merge(i.first, i.second);
	}
	if (curr->l == curr->r)
	{
		q[curr->l].ans = num;
	}
	else
	{
		dfs(curr->lch);
		dfs(curr->rch);
	}
	while ((int)rollback.size() > mem)
	{
		*rollback.top().first = rollback.top().second;
		rollback.pop();
	}
}
 
 
int main()
{
	ifstream in;
	ofstream out;
 
	in.open("connect.in");
	out.open("connect.out");
 
	int n, k;
	in >> n >> k;
	num = n;
	node* root = build(0, k);
	set<pair<pair<int, int>, int>> opened;
	set<pair<pair<int, int>, int>>::iterator iter;
	q.resize(k + 1);
	dsupref.resize(n, -1);
	dsurang.resize(n, 1);
	for (int i = 0; i < k; ++i)
	{
		in >> q[i].t;
		if (q[i].t == '+')
		{
			in >> q[i].u >> q[i].v;
			if (q[i].u > q[i].v)
			{
				swap(q[i].u, q[i].v);
			}
			--q[i].u;
			--q[i].v;
			opened.insert({ { q[i].u, q[i].v }, i });
		}
		if (q[i].t == '-')
		{
			in >> q[i].u >> q[i].v;
			if (q[i].u > q[i].v)
			{
				swap(q[i].u, q[i].v);
			}
			--q[i].u;
			--q[i].v;
			iter = opened.lower_bound({ { q[i].u, q[i].v }, -1 });
			add(iter->second, i, iter->first, root);
			opened.erase(iter);
		}
	}
	for (auto i : opened)
	{
		add(i.second, k, i.first, root);
	}
	dfs(root);
	for (auto i : q)
	{
		if (i.t == '?')
		{
			out << i.ans << '\n';
		}
	}
 
	return 0;
}
