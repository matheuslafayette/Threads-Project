#include <bits/stdc++.h>
#include <chrono>
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

    const int lin = id%LIN1;
    const int col = id%COL2;

    float ans = 0;
    for(auto i:sparseMatrix1[lin])
        ans += i.second * matrix2[i.first][col];
    
    result[lin][col] = ans;

    pthread_exit(NULL);
}

void *sparseXsparse(void *id_t){

    int id = *(int *)id_t;

    const int lin = id%LIN1;
    const int col = id%COL2;

    float ans = 0;
    for(auto i:sparseMatrix1[lin]){
        auto it = lower_bound(sparseMatrix2[i.first].begin(), sparseMatrix2[i.first].end(), col, comp());
        if (it != sparseMatrix2[i.first].end() && it->first == col)
            ans += i.second * it->second;
    }
    
    result[lin][col] = ans;

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

    if(COL1 != LIN2){
        
        cout << "not possible!" << endl;
        pthread_exit(NULL);
    }

    const int TAM = LIN1 * COL2;
    pthread_t threads[TAM];
    int id[TAM];

    //sparse x normal   
    cout << "sparse x normal" << endl;
    result.assign(LIN1, vector<float>(COL2, 0.0));

    for(int i = 0; i < TAM; i++){

        id[i] = i;
        pthread_create(&threads[i], NULL, sparseXnormal, (void *)&id[i]);
    }

    for(int i = 0; i < TAM; i++)
        pthread_join(threads[i], NULL);
    
    print(result);
    
    //sparse x sparse
    cout << "sparse x sparse" << endl;
    result.assign(LIN1, vector<float>(COL2, 0.0));

    for(int i = 0; i < TAM; i++){

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