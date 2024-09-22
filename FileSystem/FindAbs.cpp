#include <bits/stdc++.h>
using namespace std;
namespace fs = filesystem;
#define ll long long
class FindAbs{
    public:

        map<string,string> m;

        FindAbs(ll paper, string year){
            publication(paper,year);
        }
        void convert(string abstract){

        }
        void publication(ll paper, string year){
            string filePath = "./Dataset/cit-HepTh-abstracts/" + year + "/" + to_string(paper) + ".abs";
            ifstream file(filePath);

            string content="";
            if (file.is_open()){
                std::string line;
                while (getline(file, line)){
                    content += line + "\n";
                }
                file.close();
                convert(content);
            }
            else{
                cout<<"Not Found\n";
                
            }
        }
};
