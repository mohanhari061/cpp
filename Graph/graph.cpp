#include <bits/stdc++.h>
#include "./matrixOps.cpp"

using namespace std;

class graph{
    public:
        ll n,m;
        bool directed;
        vvll edges;
        map<ll,vll> adjList;
        vvll adjMat,d;
        matrixOperation matOps;

        graph(ll n,vvll edges,bool directed=false){
            this->n=n;
            this->m=edges.size();
            this->edges=edges;
            this->directed=directed;
            this->adjMat=vvll(n,vll(n,0));
            this->d=vvll(n,vll(n,1e9));
            for(int i=0;i<this->m;i++){
                adjList[edges[i][0]].pb(edges[i][1]);
                adjMat[edges[i][0]][edges[i][1]]=1;
                if(!directed){
                    adjList[edges[i][1]].pb(edges[i][0]);
                    adjMat[edges[i][1]][edges[i][0]]=1;
                }

            }
        }
        void display(){
            cout<<"Adjacency List"<<endl;
            for(auto x:this->adjList){
                cout<<x.first<<" :: ";
                for(auto ele:x.second){
                    cout<<ele<<" ";
                }
                cout<<nline;
                
            }
            cout<<nline<<nline;
            cout<<"Adjacency Matrix"<<endl;
            for(int i=0;i<n;i++){
               for(int j=0;j<n;j++){
                  cout<<this->adjMat[i][j]<<" ";
               }
               cout<<nline;
            }
        }
        
        pair<vlld,vlld> degreeDistribution(){
            map<ll,lld> mp;
            vll in(n),out(n),deg(n);
            for(int i=0;i<n;i++){
               for(int j=0;j<n;j++){
                  if(adjMat[i][j]){
                    out[i]++;
                    in[j]++;
                  }
               }
            }
            for(int i=0;i<n;i++){
               deg[i]=in[i]+out[i];
            }
            ll maxD=*max_element(all(deg));
            vlld degDis(maxD+1),cdegDis(maxD+1);
            for(auto x:deg){
                mp[x]++;
            }

            for(int i=0;i<=maxD;i++){
                if(mp.count(i))
                    degDis[i]=mp[i];
            }
            cdegDis[0]=degDis[0];
            for(int i=1;i<=maxD;i++){
                cdegDis[i]=cdegDis[i-1]+degDis[i];
            }
            return {degDis,cdegDis};
        }
        
        vlld localClusteringCoefficient(){
            vlld lcd(n);
            for(int node=0;node<n;node++){
                auto temp=this->adjList[node];
                ll k=temp.size(),ans=0;
                for(int i=0;i<k;i++){
                   for(int j=i+1;j<k;j++){
                      if(this->adjMat[temp[i]][temp[j]]){
                        ans++;
                      }
                   }
                }    

                lcd[node]=((lld)2*ans)/(k*(k-1));
            }
            return lcd;
        }
        
        lld globalClusteringCoefficient(){
            ll closedTriangles=0,openTriangles=0;
            vvll temp=this->adjMat;
            for(int i=0;i<n;i++){
               for(int j=0;j<n;j++){
                    if(temp[i][j]){
                        for(int k=0;k<n;k++){
                            if(k!=i && k!=j){
                                if((temp[i][k]==0 && temp[j][k]==1) || (temp[i][k]==1 && temp[j][k]==0)){
                                    openTriangles++;
                                }
                                else if(temp[i][k]==1 && temp[j][k]==1){
                                    closedTriangles++;
                                }
                            }
                        }
                    }
                    
               }
            }
            cout<<closedTriangles<<" "<<openTriangles<<endl;
            return ((lld)closedTriangles/(closedTriangles+openTriangles/2));
        }

        void bfs(ll src,vll& vis){
            
           queue<ll> q;q.push(src);
        
           while(!q.empty()){
              ll u=q.front();q.pop();
              for(auto v:this->adjList[u]){
                 if(vis[v]==0){
                    vis[v]=1;
                    q.push(v);
                 }
              }
           }
           return ;
        
        }        
        

        ll connectedComponents(){
            vll vis(n);
            ll count=0;
            for(int i=0;i<n;i++){
                if(vis[i]==0){
                    count++;
                    bfs(i,vis);
                }               
            }
            return count;
        }
        vlld degreeCentrality(){
            vlld ans(n);
            vll in(n),out(n),deg(n);
            for(int i=0;i<n;i++){
               for(int j=0;j<n;j++){
                  if(adjMat[i][j]){
                    out[i]++;
                    in[j]++;
                  }
               }
            }
            for(int i=0;i<n;i++){
               deg[i]=in[i]+out[i];
            }
            ll maxD=*max_element(all(deg));
            for(int i=0;i<n;i++){
               ans[i]=(lld)deg[i]/maxD;
            }

            return ans;
            
        }

        
        vlld closenessCentrality(){ //floydWarshall

            vlld ans(n);
            
            for(int i=0;i<n;i++){
               d[i][i]=0;
            }
            for(int k=0;k<n;k++){
                for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    if(d[i][j]>d[i][k]+d[k][j]){
                        d[i][j]=d[i][k]+d[k][j];
                    }
                  }
               }
            }

            for(int i=0;i<n;i++){
                auto temp=accumulate(all(d[i]),0ll);
                ans[i]=(lld)(n-1)/temp;
            }
            for(auto x:ans){
                cout<<x<<" ";
            }
            cout<<endl;
            return ans;
        }
        
        vvlld eigenVectorCentrality(){ //power iteration method
            lld normalizedValue=-1,pNVal=-2;
            vvlld adjm(n,vlld(n,0)),ans(n,vlld(1,1));
            for(int i=0;i<n;i++){
               for(int j=0;j<n;j++){
                  adjm[i][j]=(lld)adjMat[i][j];
               }
            }
            ll itr=7;
            while(itr-- && pNVal!=normalizedValue){
                lld temp=0;
                ans=matOps.mul(adjm,ans);
                for(int i=0;i<n;i++){
                   temp+=ans[i][0]*ans[i][0];
                }
                pNVal=normalizedValue;
                normalizedValue=sqrt(temp);
                for(int i=0;i<n;i++){
                   ans[i][0]/=normalizedValue;
                }
                

            }
            return ans;

        }


        // pending************************
        // vlld betweenessCentrality(){

        // }

        ll diameter(){
            closenessCentrality();
            ll mx=LLONG_MIN;
            for(auto v:d){
                for(auto x:v){
                  mx=max(mx,x);
                }
            }
            return mx;
        }

        vvll bfsDis(ll src){
            vll vis(n),temp;
            vvll res;
            queue<int> q;q.push(src);q.push(-1);
            res.pb({});

            while(!q.empty()){
                ll u=q.front();
                for(auto v:adjList[u]){
                    if(vis[v]==0){
                        temp.pb(v);
                        vis[v]=1;
                        q.push(v);
                    }
                }
                if(q.front()==-1){
                    q.pop();
                    if(!q.empty()){
                        res.pb(temp);
                        temp={};
                        q.push(-1);
                    }
                    
                }
            }
            return res;

        }
        vlld katzCentrality(){ 
            ll dia=diameter();
            vvvlld A(dia+1,vvlld(n,vlld(n,0)));
            vlld ans(n);
            for(int i=0;i<n;i++){
                vvll res=bfsDis(i);
                for(int a=0;a<res.size();a++){
                    for(int b=0;b<res[a].size();b++){
                        A[a][i][res[a][b]]=1;
                    }
                }
            }

            for(int i=0;i<n;i++){
                for(int k=0;k<A.size();k++){
                    ll temp=0;
                    lld alpha=0.2;
                    for(int j=0;j<n;j++){
                        temp+=A[k][j][i];
                    }
                    ans[i]+=temp*pow(alpha,k+1);
                }
            }
            return ans;

        }

        vvlld pageRank(ll iterations){
            vvlld R(1,vlld(n,(lld)1/n)),E(1,vlld(n,(lld)1/n)),A(n,vlld(n,0));
            lld d=0.85;
           
            E=matOps.scalarMul(1-d,E);
            
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    A[i][j]=adjMat[i][j];
                }
            }
             
            for(int i=0;i<iterations;i++){
                R=matOps.mul(R,A);
                R=matOps.scalarMul(d,R);
                R=matOps.add(E,R);       
            }
            
            return R;   

        }

        lld calculateDensity() {
            if (n < 2) return 0.0; 
            return (2.0 * m) / (n * (n - 1));
        }

        void calculateHubAuth(int maxIterations = 100, double tol = 1e-5) {
            vlld hub(n, 1.0);      
            vlld auth(n, 1.0);     
            vlld newHub(n, 0.0);   
            vlld newAuth(n, 0.0);   

            for (int iter = 0; iter < maxIterations; iter++) {
                for (int i = 0; i < n; i++) {
                    newAuth[i] = 0.0;
                    for (int j = 0; j < n; j++) {
                        if (adjMat[j][i] == 1) {
                            newAuth[i] += hub[j];  // Authority is the sum of hubs of incoming nodes
                        }
                    }
                }

                for (int i = 0; i < n; i++) {
                    newHub[i] = 0.0;
                    for (int j = 0; j < n; j++) {
                        if (adjMat[i][j] == 1) {
                            newHub[i] += auth[j];  // Hub is the sum of authorities of outgoing nodes
                        }
                    }
                }

                double maxAuth = *max_element(all(newAuth));
                double maxHub = *max_element(all(newHub));

                for (int i = 0; i < n; i++) {
                    auth[i] = newAuth[i] / maxAuth;
                    hub[i] = newHub[i] / maxHub;
                }

                lld hubDiff = 0.0, authDiff = 0.0;
                for (int i = 0; i < n; i++) {
                    hubDiff += fabs(hub[i] - newHub[i]);
                    authDiff += fabs(auth[i] - newAuth[i]);
                }

                if (hubDiff < tol && authDiff < tol) {
                    break; 
                }
            }

            // Output the final hub and authority scores
            cout << "Hub scores: ";
            for (double h : hub) {
                cout << h << " ";
            }
            cout << endl;

            cout << "Authority scores: ";
            for (double a : auth) {
                cout << a << " ";
            }
            cout << endl;
        }
     
        int countBidirectionalEdges() {
            int count = 0;
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (adjMat[i][j] == 1 && adjMat[j][i] == 1) {
                        count++;
                    }
                }
            }
            return count;
        }

        lld calculateReciprocity() {
            ll totalDirectedEdges = m;
            if (totalDirectedEdges == 0) return 0.0; 

            ll bidirectionalEdges = countBidirectionalEdges();
            return (2.0 * bidirectionalEdges) / totalDirectedEdges; 
        }


        lld linkPredClusteringCoeff(ll A,ll B){
            lld count=0;
            set<ll> a;
            for(auto x:this->adjList[A]){
                a.insert(x);
            }
            for(auto x:this->adjList[B]){
                if(a.count(x))count++;
            }
            for(auto x:this->adjList[B]){
                a.insert(x);
            }

            return count/(lld)(a.size());
            
        }
        /*
        vlld linkPredStatusTheory(ll A,ll B){
            
            vlld ans;
            vll temp;
            set<ll> a,common;

            for(auto x:this->adjList[A]){
                a.insert(x);
            }
            for(auto x:this->adjList[B]){
                if(a.count(x))common.insert(x);
            }

            for(auto c:common){
                //A<-c B<-c
                if(this->adjMat[c][A] && this->adjMat[c][B] ){
                    ll x=this->adjMat[c][A],y=this->adjMat[c][B];
                    if(x==-1 && y==-1){

                    }
                    else if(x==-1 && y==1){

                    }
                    else if(x==1 && y==-1){
                        
                    }
                }
                //A<-c B->c
                //A->c B<-c
                //A->c B->c
            }
            

        }
        ll linkPredBalanceTheory(){

        }/**/

};

