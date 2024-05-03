#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;


vector<vector<pair<string,int>>> finalPaths;    
void exportPaths() {            //Export paths to draw with networkX
    ofstream outFile("paths.txt");
    if (!outFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo para escribir." << endl;
        return;
    }

    for (const auto& path : finalPaths) {
        for (const auto& node : path) {
            outFile << node.first << " " << node.second << endl;
        }
        outFile << endl; // Separador entre caminos
    }

    outFile.close();
}

void findSubstring(string a, string b, int& count){     //Find the largest sufix and prefix of two substrings
    int size_a=a.size();
    int size_b=b.size();
    int i=0;
    int j=0;
    while(i<size_a){
        if(a[i]==b[j]){
            count++;
            i++;
            j++;
        }
        else{
            j=0;
            count=0;
            while(i<size_a && a[i]!=b[j]){
                i++;
            }
        }
    }

}

void fillMatrix(vector<vector<int>>& adj_matrix, vector<string> seq, int linkage){  //Fill the matrix with the best substring coincidence
    vector<int> temp_row;
    int count;
    for(int i=0; i<seq.size(); i++){
        for(int j=0; j<seq.size(); j++){
            count=0;
            if(i==j) temp_row.push_back(-1);
            else{
                findSubstring(seq[i],seq[j],count);
                if(count<linkage) temp_row.push_back(0);
                else temp_row.push_back(count);
            }
            //cout<<seq[i]<<" "<<seq[j]<<" "<<count<<endl;
        }
        adj_matrix.push_back(temp_row);
        temp_row.clear();
    }
}

int findGreaterEdge(vector<vector<int>> adj_matrix){        //Find the grater value in the matrix
    int max_value=INT_MIN;
    int max_origin;
    for(int i=0; i<adj_matrix.size();i++){
        for(int j=0;j<adj_matrix[i].size();j++){
            if(max_value<adj_matrix[i][j]){
                max_value=adj_matrix[i][j];
                max_origin=i;
            } 
        }
    }
    return max_origin;              //Which nodes the edge with greater value starts or origins
}

bool allVisited(vector<bool> visited){  //Verified if all visited
    for(auto v:visited){
        if(v==false) return false;
    }
    return true;
}


//DFS greedy that can act as hamiltonean
void DFS_greedy(vector<string> sequences, vector<vector<int>> adj_matrix,vector<bool> visited, int cur, int edge_value, string ans, vector<pair<string,int>> path){
    visited[cur]=true;
    ans+=sequences[cur].substr(edge_value);         //Add to answer the assembly
    path.push_back(make_pair(sequences[cur],edge_value));       //Add to path the current node and the edge which came from
    if(allVisited(visited)){        //If hamiltonean path is accomplished
        cout<<ans<<" "<<ans.size()<<endl;
        finalPaths.push_back(path);
        return;
    } 
        

    vector<pair<int,int>> order; //Store the order which nodes will be visited (edge_value, destination_index)
    for(int i=0;i<adj_matrix[cur].size();i++){
        if(adj_matrix[cur][i]>0 && visited[i]==false){
            order.push_back(make_pair(adj_matrix[cur][i], i));
        }
    }
    sort(order.begin(),order.end());  //Order by edge value (first in pair)
    for(auto node:order){
        DFS_greedy(sequences,adj_matrix,visited,node.second, node.first,ans,path);
    }
    return;
}

//Call the greedy algorithm with hamiltonean purpose or not
void exploreGraph(vector<string> sequences, vector<vector<int>> adj_matrix, bool hamiltonean = false){
    vector<bool> visited(adj_matrix.size(),false);
    vector<string> ans;
    int idx_greater_origin = findGreaterEdge(adj_matrix);
    DFS_greedy(sequences,adj_matrix, visited, idx_greater_origin,0,"",vector<pair<string,int>>());
    if(hamiltonean){        //If hamiltonean, try with all the nodes as starting node
        for(int i=0; i<adj_matrix.size();i++){
            if(i!=idx_greater_origin) DFS_greedy(sequences,adj_matrix, visited,i,0,"",vector<pair<string,int>>());
        }
    }
}

void reverseSequences(vector<string>& sequences){  //Reverse sequences
    for (string& seq : sequences) {
        reverse(seq.begin(), seq.end());
    }
}

void printMatrix(vector<vector<int>> adj_matrix){
    for(auto i:adj_matrix){
        for(auto j:i){
            cout<<j;
        }
        cout<<endl;
    }
}

int main(){
    int linkage = INT_MIN;
    vector<string> sequences = {"TACGA","ACCC","GACA"};

    //Set ask sequences
    sequences={
        "ATCCGTTGAAGCCGCGGGC",
        "TTAAGTACTGCCCG",
        "ATCTGTGTCGGG",
        "CGACTCCCGACACA",
        "CACAGATCCGTTGAAGCCGCGGG",
        "CTCGAGTTAAGTA",
        "CGCGGGCAGTACTT"    
    };
    //sequences={
    //    "AGTATTGGCAATC",
    //    "CCTTTTGG",
    //    "AATCGATG",
    //    "TTGGCAATCACT",
    //    "ATGCAAACCT"
    //};

    vector<vector<int>> adj_matrix;
    linkage=1;
    fillMatrix(adj_matrix, sequences,linkage);      //Fill matrix
    //printMatrix(adj_matrix);
    exploreGraph(sequences, adj_matrix,true);        //Call greedy or hamiltonean (last parameter set to true)

    if(!finalPaths.empty()) exportPaths();      //If result

    reverseSequences(sequences);        //Try with reverse

    adj_matrix.clear();         //Clear for reverse
    fillMatrix(adj_matrix, sequences,linkage);              
    exploreGraph(sequences, adj_matrix);             //Call greedy or hamiltonean

    return 0;
}