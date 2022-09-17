#include <bits/stdc++.h>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

//pasta com as entradas
#define IN 1
string strCaminho = "./in" + to_string(IN) + "/in";
const char *caminhoArq = strCaminho.c_str();

//N arquivos, T threads, P produtos
int N, T, P;
vector<int> buffer;
pthread_mutex_t *mymutex = NULL;

//atualiza contador de cada produto
//travando mutex relativo a cada posição
void put(int prod){

    prod--;
    pthread_mutex_lock(&mymutex[prod]);
    buffer[prod]++;
    pthread_mutex_unlock(&mymutex[prod]);
}

void *threadCode(void *id){

    int idthread = *(int *)id;
    FILE *arq = NULL;

    //arquivos predefinidos para cada thread
    for(int i = idthread; i < N; i += T){
        
        //caminho para a entrada
        string str = "./in" + to_string(IN) + "/" + to_string(i+1) + ".in";
        const char *nameArq = str.c_str();

        arq = fopen(nameArq, "r");
        if(arq == NULL) exit(1);

        int num;

        //lê produto e chama função que atualiza seu contador
        while( !feof(arq) ){

            fscanf(arq, "%d", &num );
            put(num);
        }

        fclose(arq);
    }

    pthread_exit(NULL);
}

int main(){

    ios_base::sync_with_stdio(false);

    FILE *arq = fopen(caminhoArq, "r");
    if(arq == NULL) exit(1);

    fscanf(arq, "%d %d %d", &N, &T, &P);

    pthread_t thread[T];
    int *idthread[T];
    buffer.resize(P);

    //garantindo exclusao mútua separada para cada posição do array
    //array de mutexs
    mymutex = (pthread_mutex_t *)malloc(P * sizeof(pthread_mutex_t));
    for(int i = 0; i < P; i++)
        mymutex[i] = PTHREAD_MUTEX_INITIALIZER; 
    
    //cria thread
    for(int i = 0; i < T; i++){

        idthread[i] = (int *)malloc(1 * sizeof(int));
        *idthread[i] = i;

        pthread_create(&thread[i], NULL, threadCode, (void *)idthread[i]);
    }

    //espera todas as thread finalizarem
    for(int i = 0; i < T; i++)
        pthread_join(thread[i], NULL);

    //print do resultado
    cout << "produto   qtd" << endl;
    for(int i = 0; i < buffer.size(); i++){

        cout << setw(4) << i+1 << setw(8) << buffer[i] << endl;
    }

    for(int i = 0; i < T; i++)
        free(idthread[i]);
    free(mymutex);
    
    pthread_exit(NULL);
}