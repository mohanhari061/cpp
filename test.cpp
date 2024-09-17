#include <bits/stdc++.h>
// #include "./algo/debug.cpp" 
#include "./Graph/graph.cpp" 
using namespace std ;


void solve(ll testcase){
   ll n,m,directed,temp=0; cin >> n >> m >>  directed;
   vvll edges(m);
   map<ll,ll> mp,rmp;
   
   for(int i=0;i<m;i++){
      ll u,v;cin>>u>>v;
      // if(mp.count(u)==0){
      //    mp[u]=temp;
      //    rmp[temp]=u;
      //    temp++;
      // }
      // if(mp.count(v)==0){
      //    mp[v]=temp;
      //    rmp[temp]=v;
      //    temp++;
      // }
      // edges[i]={mp[u],mp[v]};
      edges[i]={u,v};
   }
   
   graph g(n,edges,directed);
   g.display();
   // auto t=g.katzCentrality();
   debug(t);
   

   
   

  
}  

inline void testcases(){
   int test = 1, testcase = 1 ;
   // cin >> test ;
   
   cout << setprecision(12) << fixed ;
   cerr << setprecision(4) << fixed ;
   while(test --){
      // cout << "Case #" << testcase << ": ";
      solve (testcase ++) ;
   } 
}
 

 
int main (){
   ios_base::sync_with_stdio(false);cin.tie(NULL);

   #ifndef ONLINE_JUDGE
      freopen("output.txt", "w", stdout);
      freopen("input.txt", "r", stdin);
      freopen("error.txt", "w", stderr);
   #endif
   
   testcases();
}