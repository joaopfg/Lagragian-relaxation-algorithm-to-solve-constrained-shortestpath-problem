#include <bits/stdc++.h>

#define rep(i,begin,end) for(int i=begin;i<=end;i++)
#define repi(i,end,begin) for(int i=end;i>=begin;i--)
#define fio ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

using namespace std;
using namespace std::chrono;

typedef pair<int,double> id;
typedef pair<double,int> di;

#define MAXN 102

struct wayNode{
	double dway;
	vector<int> way;
};

struct comp{
	bool operator()(const wayNode (&a), const wayNode (&b)){
		return b.dway < a.dway;
	}
}; 

const double INF = 1000000009.0;
int n, m, k, it;
double threshold, f0, g0, fn, gn, fopt, fplus, gplus, fminus, gminus, fu, gu, fk, gk;
double u, L, Lu, Lk, LB, UB;
bool sol;
double d[MAXN];
int par[MAXN], sz[MAXN];
int pos[MAXN][MAXN];
double wc[MAXN][MAXN], wt[MAXN][MAXN];
wayNode X0, Xn, Xopt, Xplus, Xminus, Xu, Xk;
vector<wayNode> A;
priority_queue<wayNode, vector<wayNode>, comp> B;

void Dijkstra(int s, vector<id> (&g)[MAXN]){
	rep(i,1,n){
		if(d[i] != -1.0) d[i] = INF;
		par[i] = -1;
	} 

	d[s] = 0.0;

	set<di> q;
	q.insert({d[s],s});

	int cur, to;
	double weight;

	while(!q.empty()){
		cur = q.begin()->second;
		q.erase(q.begin());

		rep(i,0,(int)g[cur].size() - 1){
			to = g[cur][i].first;
			weight = g[cur][i].second;

			if(weight != -1.0 && d[to] != -1.0 && d[cur] + weight < d[to]){
				q.erase({d[to],to});
				d[to] = d[cur] + weight;
				par[to] = cur;
				q.insert({d[to],to});
			}
		}
	}
}

wayNode getWay(int spur){
	wayNode myWay;
	myWay.dway = 0.0;

	int crawl = n;
	myWay.way.push_back(crawl);

	while(par[crawl] != -1){
		myWay.way.push_back(par[crawl]);
		myWay.dway += wc[par[crawl]][crawl];
		crawl = par[crawl];
	}

	reverse(myWay.way.begin(),myWay.way.end());

	return myWay;
}

wayNode unionWay(wayNode (&way1), wayNode (&way2)){
	wayNode Union;
	Union.dway = way1.dway + way2.dway;

	rep(i,0,(int)way1.way.size() - 1) Union.way.push_back(way1.way[i]);
	rep(i,1,(int)way2.way.size() - 1) Union.way.push_back(way2.way[i]);

	return Union;
}

bool equalWay(wayNode (&way1), wayNode (&way2)){
	if(way1.dway != way2.dway || (int)way1.way.size() != (int)way2.way.size()) return false;

	rep(i,0,(int)way1.way.size() - 1){
		if(way1.way[i] != way2.way[i]) return false;
	}

	return true;
}

int spurBegin(wayNode (&way1), wayNode (&way2)){
	int ind = 0;
	int minSz = min((int)way1.way.size(),(int)way2.way.size());

	while(ind < minSz && way1.way[ind] == way2.way[ind]) ind++;

	return ind-1;
}

wayNode getRoot(wayNode (&myWay), int spurInd){
	wayNode root;
	root.dway = 0.0;

	for(int i=0;i<(int)myWay.way.size() - 1;i++){
		root.way.push_back(myWay.way[i]);

		if(i != 0) root.dway += wc[myWay.way[i-1]][myWay.way[i]];
		if(i == spurInd) break;
	}

	return root;
}

int main(){
fio

vector<id> gc[MAXN], gt[MAXN], g[MAXN];

cin >> n >> m >> k;

int aux;

cin >> aux >> threshold;

rep(i,1,n) cin >> aux;

int u, v;
double weight, times;

rep(i,1,m){
	cin >> u >> v >> weight >> times;
	gc[u].push_back({v,weight});
	gt[u].push_back({v,times});
	g[u].push_back({v,-1});
	sz[u]++;
	pos[u][v] = sz[u] - 1;
	wc[u][v] = weight;
	wt[u][v] = times;
}

auto start = high_resolution_clock::now();

Dijkstra(1,gc);

if(d[n] != INF){
	X0 = getWay(1);
	f0 = d[n];
	g0 = 0;

	rep(i,1,(int)X0.way.size() - 1) g0 += wt[X0.way[i-1]][X0.way[i]];
	g0 -= threshold;
}

if(g0 <= 0){
	Xopt = X0;
	fopt = f0;
	sol = true;
	goto fin;
}
else{
	Xplus = X0;
	fplus = f0;
	gplus = g0;

	Dijkstra(1,gt);

	if(d[n] != INF){
		Xn = getWay(1);
		gn = d[n] - threshold;
		fn = 0;

		rep(i,1,(int)Xn.way.size() - 1) fn += wc[Xn.way[i-1]][Xn.way[i]];
	}

	if(gn > 0) goto fin;
	else{
		Xminus = Xn;
		fminus = fn;
		gminus = gn;

		step1:

		u = (fminus - fplus)/(gplus - gminus);
		L = fplus + u*gplus;
		UB = INF;

		rep(i,1,n){
			rep(j,0,(int)g[i].size() - 1) g[i][j].second = gc[i][j].second + u*gt[i][j].second;
		}

		Dijkstra(1,g);

		Xu = getWay(1);

		fu = 0;
		gu = 0;

		rep(i,1,(int)Xu.way.size() - 1){
			fu += wc[Xu.way[i-1]][Xu.way[i]];
			gu += wt[Xu.way[i-1]][Xu.way[i]];
		}

		gu -= threshold;
		Lu = fu + u*gu;

		if(gu == 0){
			sol = true;
			Xopt = Xu;
			fopt = fu;
			goto fin;
		}
		else if(Lu == L && gu < 0){
			LB = Lu;
			UB = min(UB,fu);
			Xminus = Xu;
			it = 1;
			goto step2;
		}
		else if(Lu == L && gu > 0){
			LB = Lu;
			UB = fminus;
			it = 1;
			goto step2;
		}
		else if(Lu < L && gu > 0){
			Xplus = Xu;
			fplus = fu;
			gplus = gu;
			goto step1;
		}
		else if(Lu < L && gu < 0){
			Xminus = Xu;
			fminus = fu;
			gminus = gu;
			UB = min(UB,fu);
			goto step1;
		}

		step2:

		A.push_back(Xplus);
		A.push_back(Xminus);

		while(true){
			it++;

			if(LB >= UB){
				sol = true;
				Xopt = Xminus;
				fopt = UB;
				goto fin;
			}

			int spurB = 0, spurNode;

			///Lawler's modification
			if(it != 1) spurB = spurBegin(A[it-2],A[it-1]);
			///For each spur node
			rep(i,spurB,(int)A[it-1].way.size() - 2){
				spurNode = A[it-1].way[i];
				wayNode rootPath = getRoot(A[it-1],i);

				rep(j,0,it-1){
					wayNode pPath = getRoot(A[j],i);

					///Turn off the possibility of picking a way already computed
					if(equalWay(rootPath,pPath)) g[A[j].way[i]][pos[A[j].way[i]][A[j].way[i+1]]].second = -1.0;
				}

				///Turn off the vertex in the spur way
				rep(j,0,(int)rootPath.way.size() - 2) d[rootPath.way[j]] = -1.0;

				///Gets the smallest way from the spurNode
				Dijkstra(spurNode,g);

				///Puts the candidate in the heap
				if(d[n] != INF){
					wayNode spurPath = getWay(spurNode);
		    		wayNode totalPath = unionWay(rootPath,spurPath);

		   		 	B.push(totalPath);  
				}

				///Restoring edges and vertex
				rep(j,0,it-1){
					wayNode pPath = getRoot(A[j],i);

					if(equalWay(rootPath,pPath)) g[A[j].way[i]][pos[A[j].way[i]][A[j].way[i+1]]].second = wc[A[j].way[i]][A[j].way[i+1]];
				}

				rep(j,0,(int)rootPath.way.size() - 2) d[rootPath.way[j]] = INF;
			}

			if(B.empty()){
				Lk = INF;
				fk = INF;
				LB = Lk;
				break;
			} 

			wayNode temp = B.top();

			bool notEqual = true;
			int crawl = it-1;

			while(crawl > 0 && A[crawl].dway == temp.dway){
				if(equalWay(A[crawl],temp)) notEqual = false;
				crawl--;
			}

			if(notEqual){
				A.push_back(B.top()); 
				Xk = B.top();
				fk = 0;
				gk = 0;

				rep(i,1,(int)Xk.way.size() - 1){
					fk += wc[Xk.way[i-1]][Xk.way[i]];
					gk += wt[Xk.way[i-1]][Xk.way[i]];
				}

				gk -= threshold;
				Lk = fk + u*gk;
				LB = Lk;

				if(gk <= 0 && fk < UB){
					UB = fk;
					Xminus = Xk;
				}
			} 
			else it--;
 
			B.pop();

			if(it == 100){
				Lk = INF;
				fk = INF;
				LB = Lk;
				break;
			} 
		} 
	}
}

fin:

auto stop = high_resolution_clock::now();
auto duration = duration_cast<microseconds>(stop - start);

cout << "Time taken by function: "
         << duration.count()/1000000.0 << "  seconds" << endl;

if(sol){
	cout << "Path weight: " << fopt << endl;
	cout << "Path: ";
	rep(i,0,(int)Xopt.way.size() - 1) cout << Xopt.way[i] << " ";
	cout << endl;
}
else cout << "Optimal solution not found" << endl;
return 0;
}
