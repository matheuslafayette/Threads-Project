#define _XOPEN_SOURCE 600
#include <bits/stdc++.h>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

int NUM_THREADS;
#define ITERACOES 20

//mudar de acordo com a entrada desejada
#define ENTRADA 1
string str = "./entrada" + to_string(ENTRADA) + "/input";
const char *nameArq = str.c_str();

pthread_barrier_t barrier;

int SIZE;
double **matriz;
double *res;
double *x;
double *aux;

//inicia o sistema de equação linear
//lendo dos arquivos de entrada
//com valores para x0 = 1
void init();

//free no que foi alocado
void destroy();

//print sistema linear, com resultado
void print();

void *threadCode(void *id){

    int idthread = *(int *)id;

    int k = ITERACOES;

    while(k--){

        //numero predefinido de elementos para cada thread
        for(int i = idthread; i < SIZE; i += NUM_THREADS){

            double sum = 0;

            for(int j = 0; j < SIZE; j++){

                if(i != j)
                    sum += matriz[i][j] * x[j];
            }

            //guarda elementos da iteração (k+1) no vetor aux
            aux[i] = (res[i] - sum) / matriz[i][i];
        }

        //barreira para que todas as threads calculem o valor da iteração atual
        pthread_barrier_wait(&barrier);

        //atualiza resposta no vetor
        for(int i = idthread; i < SIZE; i += NUM_THREADS)
            x[i] = aux[i];
        
        //barreita para esperar que todos os valores do vetor sejam atualizados
        pthread_barrier_wait(&barrier);
    }

    pthread_exit(NULL);
}

int main(){

    ios_base::sync_with_stdio(false);

    init();

    pthread_t thread[NUM_THREADS];
    int *idthread[NUM_THREADS];

    //inicializando dinamicamente a barreira
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for(int i = 0; i < NUM_THREADS; i++){

        //cout << "main: creating thread " << i << endl; 

        idthread[i] = (int *)malloc(1 * sizeof(int));
        *idthread[i] = i;

        pthread_create(&thread[i], NULL, threadCode, (void *)idthread[i]);
    }

    //espera todas as threads terminarem de executar
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(thread[i], NULL);

    print();

    pthread_barrier_destroy(&barrier);
    destroy();
    pthread_exit(NULL);
}

void init(){

    FILE *arq = fopen(nameArq, "r");
    if(arq == NULL) exit(1);

    fscanf(arq, "%d", &NUM_THREADS);
    fscanf(arq, "%d", &SIZE);

    int i;

    matriz = (double **)malloc(sizeof(double *) * SIZE);
    
    for(i = 0; i < SIZE; i++)
        matriz[i] = (double *)malloc(sizeof(double) * SIZE);
    
    res = (double *)malloc(sizeof(double) * SIZE);
    x = (double *)malloc(sizeof(double) * SIZE);
    aux = (double *)malloc(sizeof(double) * SIZE);

    for(i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            fscanf(arq, "%lf", &matriz[i][j]);
        
    for(i = 0; i < SIZE; i++)
        fscanf(arq, "%lf", &res[i]);
    
    for(i = 0; i < SIZE; i++)
        x[i] = 1;
    
    fclose(arq);
}

void destroy(){

    free(res);
    free(x);
    free(aux);

    for(int i = 0; i < SIZE; i++)
        free(matriz[i]);
    
    free(matriz);
}

void print(){

    cout << "A =" << endl;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++)
            cout << matriz[i][j] << " ";

        cout << endl;
    }
    cout << endl;

    cout << "B =" << endl;
    for(int i = 0; i < SIZE; i++)
        cout << res[i] << " ";
    cout << endl << endl;
    
    cout << "X =" << endl;
    for(int i = 0; i < SIZE; i++)
        cout << x[i] << " ";
    cout << endl;
}

