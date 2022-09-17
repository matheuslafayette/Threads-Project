#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

struct comp{
    bool operator()(const pair<int, float>& a, const int& b){
        return (a.first < b);
    }
};

int LIN1, COL1, LIN2, COL2;

vector<vector<float>> matrix1, matrix2;
vector<vector<pair<int, float>>> sparseMatrix1, sparseMatrix2;
vector<vector<float>> result;

void makeMatrix1();
void makeMatrix2();
void print(vector<vector<float>>);

void *sparseXnormal(void *id_t){

    int id = *(int *)id_t;

    for(int i = 0; i < COL2; i++){

        float ans = 0;
        for(auto j:sparseMatrix1[id])
            ans += j.second * matrix2[j.first][i];
        
        result[id][i] = ans;
    }

    pthread_exit(NULL);
}

void *sparseXsparse(void *id_t){

    int id = *(int *)id_t;

    for(int i = 0; i < COL2; i++){

        float ans = 0;
        for(auto j:sparseMatrix1[id]){

            auto it = lower_bound(sparseMatrix2[j.first].begin(), sparseMatrix2[j.first].end(), i, comp());

            if (it != sparseMatrix2[j.first].end() && it->first == i)
                ans += j.second * it->second;
        }
        
        result[id][i] = ans;
    }

    pthread_exit(NULL);
}

int main(){

    ios_base::sync_with_stdio(false);

    makeMatrix1();
    makeMatrix2();
    result.resize(LIN1);

    cout << "MATRIX 1" << endl;
    print(matrix1);
    cout << "MATRIX 2" << endl;
    print(matrix2);

    pthread_t threads[LIN1];
    int id[LIN1];

    //sparse x normal   
    cout << "sparse x normal" << endl;
    result.assign(LIN1, vector<float>(COL2, 0.0));

    for(int i = 0; i < LIN1; i++){

        id[i] = i;
        pthread_create(&threads[i], NULL, sparseXnormal, (void *)&id[i]);
    }

    for(int i = 0; i < LIN1; i++)
        pthread_join(threads[i], NULL);
    
    print(result);
    
    //sparse x sparse
    cout << "sparse x sparse" << endl;
    result.assign(LIN1, vector<float>(COL2, 0.0));

    for(int i = 0; i < LIN1; i++){

        id[i] = i;
        pthread_create(&threads[i], NULL, sparseXsparse, (void *)&id[i]);
    }

    for(int i = 0; i < LIN1; i++)
        pthread_join(threads[i], NULL);
    
    print(result);
    
    pthread_exit(NULL);
}

void makeMatrix1(){

    cin >> LIN1 >> COL1;
    matrix1.resize(LIN1);
    sparseMatrix1.resize(LIN1);

    float value;
    for(int i = 0; i < LIN1; i++)
        for(int j = 0; j < COL1; j++){

            cin >> value;
            matrix1[i].push_back(value);

            if(value != 0.0)
                sparseMatrix1[i].push_back({j, value});
        }
}

void makeMatrix2(){

    cin >> LIN2 >> COL2;
    matrix2.resize(LIN2);
    sparseMatrix2.resize(LIN2);

    float value;
    for(int i = 0; i < LIN2; i++)
        for(int j = 0; j < COL2; j++){

            cin >> value;
            matrix2[i].push_back(value);

            if(value != 0.0)
                sparseMatrix2[i].push_back({j, value});
        }
}

void print(vector<vector<float>> matrix){

    for(auto i:matrix){
        for(auto j:i)
            cout << setw(5) << setprecision(2) << fixed << j << " ";
        cout << endl;
    }
    cout << endl;
}