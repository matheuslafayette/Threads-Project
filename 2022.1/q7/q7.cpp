#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

int TAM;
vector<vector<string>> matrix;

void *checkRow(void *row_p){

    int row = *(int *)row_p;
    set<string> s;
    for(int i = 0; i < TAM; i++)
        s.insert(matrix[row][i]);
    
    pthread_exit((void *)(s.size() == TAM));
}

void *checkCol(void *col_p){

    int col = *(int *)col_p;
    set<string> s;
    for(int i = 0; i < TAM; i++)
        s.insert(matrix[i][col]);
    
    pthread_exit((void *)(s.size() == TAM));
}

int main(){

    ios_base::sync_with_stdio(false);

    cin >> TAM;
    matrix.resize(TAM);
    for(int i = 0; i < TAM; i++){

        matrix[i].resize(TAM);
        for(int j = 0; j < TAM; j++)
            cin >> matrix[i][j];
    }

    //threads produtoras e consumidoras
    pthread_t threads_lin[TAM];
    pthread_t threads_col[TAM];
    int param[TAM];

    for(int i = 0; i < TAM; i++){

        param[i] = i;
        pthread_create(&threads_lin[i], NULL, checkRow, (void *)&param[i]);
        pthread_create(&threads_col[i], NULL, checkCol, (void *)&param[i]);
    }
    
    bool lat = true;
    void *retval;

    for(int i = 0; i < TAM; i++){

        pthread_join(threads_lin[i], &retval);
        if(!lat)
            break;
        lat = (bool *)retval;
        pthread_join(threads_lin[i], &retval);
        if(!lat)
            break;
        lat = (bool *)retval;
    }

    lat ? cout << "latino" << endl : cout << "not latino" << endl;
    
    pthread_exit(NULL);
}