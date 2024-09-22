#include <bits/stdc++.h>
using namespace std;

class dateMap{
    public:
        map<string,string> m;
        dateMap(){
            freopen("Dataset/cit-HepTh-dates.txt/Cit-HepTh-dates.txt", "r", stdin);
            string p,date;
            while(cin>>p>>date){
                m[p]=date.substr(0,4);
            }
        }

};