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

        pair<int,vvll> calcMat(vvll a1,vvll ak1){
            int flag=1;
            vvll res(n,vll(n));
            for(int i=1;i<n;i++){
                for(int j=1;j<n;j++){
                    if(ak1[i][j]==1){
                        for(int p=1;p<n;p++){
                            if(a1[j][p]==1){
                                flag=0;
                                res[i][p]=1;
                            }
                        }
                    }
                }
            }
            return {flag,res};
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
            for(int i=0;i<n;i++){
               for(int j=0;j<n;j++){
                  for(int k=0;k<n;k++){
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
                debug(normalizedValue);
                debug(ans);


            }
            return ans;

        }
/*
        vlld betweenessCentrality(){

        }
        vvlld katzCentrality(){ 
        

        }

        vvlld pageRank(){
            vvlld R(1),E(1),A;
            lld d=0.85;
            for(int i=0;i<n;i++){
            R[0].pb((lld)1/n);
            E[0].pb((lld)1/n);
            }
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    A[i][j]=this->adjMat[i+1][j+1];
                }
            }
            for(int i=0;i<3;i++){
                R=matOps.mul(R,A);
                R=matOps.scalarMul(d,R);
                R=matOps.add(E,R);       
            }

            return R;   

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

