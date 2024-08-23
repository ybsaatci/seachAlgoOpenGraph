#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include<array>
#include <unordered_map>
#include<tuple>
#include <math.h>
#include <iterator>
using namespace std;

int N = 42;
int GROUPS = 16;
template <typename S>
ostream& operator<<(ostream& os,
                    const vector<S>& vector)
{
    // Printing all the elements
    // using <<
    for (auto element : vector) {
        os << element << " ";
    }
    return os;
}

class adjMatrix{
    public:
        int adjM[16][16] = { NULL };
        unordered_map<int, int> groups;
        adjMatrix(const string& name) {
            string line;
            int cnt = 0;
            ifstream infile;
            infile.open(name);
            while(infile.good()) {
                while(getline(infile, line)) {
                    cout << cnt << endl;
                    stringstream ss(line);
                    vector<int> r(istream_iterator<int>(ss),
                        {});
                    
                    for(auto n: r){
                        groups.insert({n, cnt});
                    }
                    cnt++;
                    

            }
            }


        }
        adjMatrix(adjMatrix& adj) {
            for(int i = 0; i < GROUPS; i++) {
                for(int j = 0; j< GROUPS; j++) {
                    adjM[i][j] = adj.adjM[i][j];
                }
            }
            groups = adj.groups;
        }
        void prettyPrint() {
            for(int i = 0; i < GROUPS; i++) {
                for(int j = 0; j < GROUPS; j++) {
                    cout << adjM[i][j] << ' ';
                }
                cout << endl;
            }
        }

        void prettyPrintFile(ofstream& file) {
            for(int i = 0; i < GROUPS; i++) {
                for(int j = 0; j < GROUPS; j++) {
                    file << adjM[i][j] << ' ';
                }
                file << endl;
            }
        }

        int succ() {
            int N = GROUPS;
            return succFunc() == N*(N - 1) / 2;
        }

        int succFunc() {
            int score = 0;
            int N = GROUPS;
            for(int i = 0; i < N; i++) {
                for(int j = i+1; j < N; j++){
                    if(adjM[i][j] > 0) {
                        score++;
                    }
                }
            }
            return score;
        }
};

class graphC {
    public:
        int matrix[42][42][2];
        
        graphC(const string& name) {
            string line;
            int v = 0;
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
                    for(int k = 0; k < 2; k++) {
                    matrix[i][j][k] = -1;
                    }
                }
            }
            ifstream infile;
            infile.open(name);
            while(infile.good()) {
                while(getline(infile, line)) {
                    v++;
                    stringstream ss(line);
                    stringstream ins(line);
                    vector<int> r(istream_iterator<int>(ss),
                        {});
                    string i;
                    int index;
                    r.erase(r.begin());

                    getline(ins, i, ' ');
                    index = stoi(i);
                    parse(index, r);
                }
                
            }

        }
        graphC(graphC& g) {
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
                    for(int k = 0; k < 2; k++) {
                    matrix[i][j][k] = g.matrix[i][j][k];
                    }
                }
            }
        }
        void parse(int i, vector<int> v) {

            for(int j = 0; j < v.size(); j++) {
                matrix[i][v[j]][0] = v[(j + 1) % v.size()];
                matrix[i][v[(j + 1) % v.size()]][1] = v[j];

            }
        }

        string pr(int i) {
            string st;
            int* start;
            for(int j = 0; j < N; j++) {
                if(matrix[i][j][0] != -1 || matrix[i][j][1] != -1) {
                    start = &matrix[i][j][0];
                    st += to_string(matrix[i][j][0]);
                    break;
                }
            }
            int* curr = &matrix[i][*start][0];
            while(*curr != *start) {
                st += " " + to_string(*curr);
                curr = &matrix[i][*curr][0];
            }
            return st;
        }

        void prAll() {
            for(int i = 0; i < N; i++) {
                cout << to_string(i) + ". " + pr(i) << endl;
            }
        }
        void prAllFile(ofstream& file) {
            for(int i = 0; i < N; i++) {
                file << to_string(i) + ". " + pr(i) << endl;
            }
        }
        void flip(int i, int j, adjMatrix& adjM) {
            int a = matrix[i][j][1];
            int b = matrix[i][j][0];

            if(matrix[a][b][0] != -1) {
                return;
            }

            matrix[i][a][0] = b;
            matrix[i][b][1] = a;
            matrix[j][b][0] = a;
            matrix[j][a][1] = b;
            matrix[j][i][0] = -1;
            matrix[j][i][1] = -1;
            matrix[i][j][0] = -1;
            matrix[i][j][1] = -1;
            adjM.adjM[adjM.groups.at(i)][adjM.groups.at(j)] -= 1;
            adjM.adjM[adjM.groups.at(j)][adjM.groups.at(i)] -= 1;
            adjM.adjM[adjM.groups.at(a)][adjM.groups.at(b)] += 1;
            adjM.adjM[adjM.groups.at(b)][adjM.groups.at(a)] += 1;
            matrix[a][j][0] = b;
            matrix[a][b][1] = j;
            matrix[a][b][0] = i;
            matrix[a][i][1] = b;
            matrix[b][i][0] = a;
            matrix[b][a][1] = i;
            matrix[b][a][0] = j;
            matrix[b][j][1] = a;
        }

        int cost(int i, int j, adjMatrix& adjM) {
            int a = matrix[i][j][1];
            int b = matrix[i][j][0];
            int cost = 0;
            if(matrix[i][a][1] == b || matrix[j][a][0] == b) {
                return -1;
            }

            if((adjM.adjM[adjM.groups.at(i)][adjM.groups.at(j)] == 1) && (adjM.groups.at(i) != adjM.groups.at(j))) {
                cost--;
            }
            if((adjM.adjM[adjM.groups.at(a)][adjM.groups.at(b)] == 0) && (adjM.groups.at(a) != adjM.groups.at(b))) {
                cost++;
            }

            return cost;  
        }

        bool flippable(int a, int b){
            int c = matrix[a][b][0];
            int d = matrix[a][b][1];
            return matrix[c][d][0] == -1;
        }

        tuple<int, int> getRandomEdge() {
            int i = rand() % N;
            int j = rand() % N;
            while(matrix[i][j][0] == -1 || !flippable(i, j)) {
                i = rand() % N;
                j = rand() % N;
            }

            return make_tuple(i, j);
        }
    };

tuple<graphC, adjMatrix> search(graphC& g,adjMatrix& b, int count) {
    int cnt = 0;
    int best = 1;
    int curr = 1;
    float c = 0.05;
    double pAccept;
    ofstream myfile;
    adjMatrix bMatrix = adjMatrix(b);
    graphC bTri = graphC(g);
    while(!b.succ()) {
        try
        {
            pAccept = exp((-cnt / c) * (best - curr) / best);
        }
        catch(const std::exception& e)
        {
            pAccept = numeric_limits<double>::infinity();
            std::cerr << e.what() << '\n';
        }
        if(curr > best || ((double)rand() / RAND_MAX < pAccept)){
            if(curr == 0) {
                curr++;
            }
            best = curr;
            bMatrix = adjMatrix(b);
            bTri = graphC(g);
        }
        tuple<int, int> edge = g.getRandomEdge();
        curr += g.cost(get<0>(edge), get<1>(edge), b);
        g.flip(get<0>(edge), get<1>(edge), b);
        cnt++;
        if(b.succ()){
            myfile.open("solution.txt");
            g.prAllFile(myfile);
            myfile << endl;
            b.prettyPrintFile(myfile);
            bMatrix = adjMatrix(b);
            bTri = graphC(g);
            myfile.close();
            break;
        }
        if(cnt == count){
            break;
        }
    }
    return {bTri, bMatrix};
}
int main()
{
    srand(time(NULL));
    graphC mygraph("tri42.txt");
    adjMatrix b("groups42.txt");
    for(int i = 0; i <  N; i++) {
        for(int j = 0; j < N; j++) {
                if(mygraph.matrix[i][j][0] != -1) {
                    b.adjM[b.groups.at(i)][b.groups.at(mygraph.matrix[i][j][0])] += 1;
                }
            }
        }
    tuple<graphC, adjMatrix> t = search(mygraph, b, 2500);
    while(!get<1>(t).succ()){
        t = search(get<0>(t), get<1>(t), 250);
        //cout << get<1>(t).succFunc() << ' ' << to_string(GROUPS) << endl;
    }
    
}