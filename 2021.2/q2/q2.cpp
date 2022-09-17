#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define TAM 15
int v[TAM];

typedef struct{

    int inicio;
    int fim;

}Data;

//inicio vetor com valores pseudo-aleatorios
void inicV(int *v, int tam){

    srand(time(0));
    
    for(int i = 0; i < TAM; i++)
        v[i] = rand() % 101;
}


int hoarePartition(int l, int r){

    int i = l;
    int j = r+1;

    do{

        do i++; 
        while(v[i] < v[l] && i < r );

        do j--; 
        while(v[j] > v[l]);
            
        int aux = v[i];
        v[i] = v[j];
        v[j] = aux;

    }while(i < j);
    
    int aux = v[i];
    v[i] = v[j];
    v[j] = aux;

    aux = v[l];
    v[l] = v[j];
    v[j] = aux;

    return j;
}

void *threadCode(void *id){

    Data idthread = *(Data *)id;
    int l = idthread.inicio;
    int r = idthread.fim;

    if(l < r){

        int s = hoarePartition(l, r);

        pthread_t newthreadLeft;
        Data dataLeft;
        dataLeft.inicio = l;
        dataLeft.fim = s-1;

        pthread_t newthreadRight;
        Data dataRight;
        dataRight.inicio = s+1;
        dataRight.fim = r;

        pthread_create(&newthreadLeft, NULL, threadCode, (void *)&dataLeft);
        pthread_create(&newthreadRight, NULL, threadCode, (void *)&dataRight);

        pthread_join(newthreadLeft, NULL);
        pthread_join(newthreadRight, NULL);
    }    

    pthread_exit(NULL);
}

void print(){

    for(int i = 0; i < TAM; i++)
        cout << v[i] << " ";
    cout << endl;
}


int main(){

    ios_base::sync_with_stdio(false);

    inicV(v, TAM);

    cout << "array desordenado: " << endl;
    print();
    cout << endl;

    pthread_t thread;
    Data data;
    data.inicio = 0;
    data.fim = TAM-1;

    pthread_create(&thread, NULL, threadCode, (void *)&data);
    pthread_join(thread, NULL);

    cout << "array ordenado: " << endl;
    print();

    pthread_exit(NULL);
}   