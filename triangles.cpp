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
int const N = 17;
int triCount = 0;
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
int unionMatrix[N][N];


class graphC {
    public:
        int matrix[N][N][2];
        
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
        tuple<int, int> flip(int i, int j) {
            int a = matrix[i][j][1];
            int b = matrix[i][j][0];

            if(matrix[a][b][0] != -1) {
                return make_tuple(-1, -1);
            }

            matrix[i][a][0] = b;
            matrix[i][b][1] = a;
            matrix[j][b][0] = a;
            matrix[j][a][1] = b;
            matrix[j][i][0] = -1;
            matrix[j][i][1] = -1;
            matrix[i][j][0] = -1;
            matrix[i][j][1] = -1;
            matrix[a][j][0] = b;
            matrix[a][b][1] = j;
            matrix[a][b][0] = i;
            matrix[a][i][1] = b;
            matrix[b][i][0] = a;
            matrix[b][a][1] = i;
            matrix[b][a][0] = j;
            matrix[b][j][1] = a;

            if(unionMatrix[i][j] == 1) {
                for(int w = 0; w < N; w++) {
                    if(w != i && w != j && unionMatrix[i][w] == 0 && unionMatrix[j][w] == 0) {
                        triCount += 1;
                    }
                }
            }
                
            unionMatrix[i][j] -= 1;
            unionMatrix[j][i] -= 1;

            if(unionMatrix[a][b] == 1) {
                for(int w = 0; w < N; w++) {
                    if(w != a && w != b && unionMatrix[a][w] == 0 && unionMatrix[b][w] == 0) {
                        triCount -= 1;
                    }
                }
            }
            
            unionMatrix[a][b] += 1;
            unionMatrix[a][b] += 1;

            return make_tuple(a, b);
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

        vector< tuple<int, int>>getEveryEdge() {
            vector< tuple<int, int>> flips;
            for(int i = 0; i < N; i++) {
                for(int j = i+1; j < N; j++) {
                    if(matrix[i][j][0] != -1 && flippable(i, j)) {
                        flips.push_back(make_tuple(i, j));
                    }
                }
            }
            return flips;
        }
    };

tuple<graphC, graphC> search(graphC& g, graphC& h, int c) {
    int cnt = 0;
    int curr = triCount;
    int best = curr;
    float temp = 1.12;
    double pAccept;
    ofstream myfile;
    int bestU[N][N];
    graphC bestG = graphC(g);
    graphC bestH = graphC(h);
    copy(&unionMatrix[0][0], &unionMatrix[0][0] + N*N, &bestU[0][0]);
    
    if(curr > 0) {
        int improve = triCount;
        for(tuple<int, int> p : g.getEveryEdge()) {
            tuple<int, int> e = g.flip(get<0>(p), get<1>(p));
            if(triCount < improve) {
                graphC bestG = graphC(g);
                graphC bestH = graphC(h);
                copy(&bestU[0][0], &bestU[0][0] + N*N, &unionMatrix[0][0]);
                return make_tuple(bestG, bestH);
            }
            g.flip(get<0>(e), get<1>(e));
        }
         for(tuple<int, int> p : h.getEveryEdge()) {
            tuple<int, int> e = h.flip(get<0>(p), get<1>(p));
            if(triCount < improve) {
                graphC bestG = graphC(g);
                graphC bestH = graphC(h);
                copy(&bestU[0][0], &bestU[0][0] + N*N, &unionMatrix[0][0]);
                return make_tuple(bestG, bestH);
            }
            h.flip(get<0>(e), get<1>(e));
        }
    }
    while(1) {
        curr = triCount;
        float t = temp / float(cnt + 1);
        try
        {
            pAccept = exp(-(curr - best) / t);
        }
        catch(const std::exception& e)
        {
            pAccept = numeric_limits<double>::infinity();
            std::cerr << e.what() << '\n';
        }
        if(curr < best || ((double)rand() / RAND_MAX < pAccept)){
            bestG = graphC(g);
            bestH = graphC(h);
            copy(&unionMatrix[0][0], &unionMatrix[0][0] + N*N, &bestU[0][0]);
            best = curr;
            if(best == 0) { 
                break;
            }
        }

        float r = (double)rand() / RAND_MAX;
        if(r > 0.5) {
            tuple<int, int> edge = g.getRandomEdge();
            g.flip(get<0>(edge), get<1>(edge));
        } else {
            tuple<int, int> edge = h.getRandomEdge();
            h.flip(get<0>(edge), get<1>(edge));
        }
        cnt++;
        if(cnt == c){
            break;
        }
    }
    triCount = best;
    return make_tuple(bestG, bestH);
}

int main()
{
    srand(time(NULL));
    graphC g("t17-1.txt");
    graphC h("t17-2.txt");

    g.prAll();
    cout << endl;
    h.prAll();
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(g.matrix[i][j][0] != -1 || g.matrix[i][j][1] != -1) {
                unionMatrix[i][j] += 1;
            }
        }
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(h.matrix[i][j][0] != -1 || h.matrix[i][j][1] != -1) {
                unionMatrix[i][j] += 1;
            }
        }
    }

    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {
            for(int k = j + 1; k < N; k++) {
                triCount ++;
            }
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cout << unionMatrix[i][j] << " ";
        }
        cout << endl;
    }
    tuple<graphC, graphC> t = search(g, h, 1000);
    for(int i = 0; i < 1000; i++) {
        t = search(get<0>(t), get<1>(t), 500);
        if(triCount == 0) {
            break;
        }
    }
    cout << triCount << endl;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cout << unionMatrix[i][j] << " ";
        }
        cout << endl;
    }
}