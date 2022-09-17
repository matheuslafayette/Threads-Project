#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

//pasta com as entradas
#define IN 2
string strCaminho = "./in" + to_string(IN) + "/in";
const char *caminhoArq = strCaminho.c_str();

//N arquivos, T threads, P produtos
int N, T, C;
vector<int> buffer;
vector<pthread_mutex_t> mymutex;
pthread_mutex_t mutex_cont = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_total = PTHREAD_MUTEX_INITIALIZER;
int cont = 0;
int total_votos = 0;
vector<int> votos_pref;

//atualiza contador de cada produto
//travando mutex relativo a cada posição
void put(int prod){

    pthread_mutex_lock(&mymutex[prod]);
    buffer[prod]++;
    pthread_mutex_unlock(&mymutex[prod]);

    pthread_mutex_lock(&mutex_total);
    total_votos++;
    pthread_mutex_unlock(&mutex_total);
}

void *threadCode(void *id){

    int idthread = *(int *)id;
    FILE *arq = NULL;

    //arquivos definidos dinamicamente
    while (true){
        
        int numArq;
        pthread_mutex_lock(&mutex_cont);
        if(cont >= N){

            pthread_mutex_unlock(&mutex_cont);
            break;
        }
        else{

            numArq = ++cont;
            pthread_mutex_unlock(&mutex_cont);
        }
        //cout << idthread << " reading file " << numArq << endl;
        //caminho para a entrada
        string str = "./in" + to_string(IN) + "/" + to_string(numArq) + ".in";
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

    fscanf(arq, "%d %d %d", &N, &T, &C);

    pthread_t thread[T];
    int idthread[T];
    buffer.resize(C+1);
    buffer.assign(C+1, 0);
    votos_pref.resize(C+1);
    votos_pref.assign(C+1, 0);

    //garantindo exclusao mútua separada para cada posição do array
    //array de mutexs
    mymutex.resize(C+1);
    mymutex.assign(C+1, PTHREAD_MUTEX_INITIALIZER);
    
    //cria thread
    for(int i = 0; i < T; i++){

        idthread[i] = i;
        pthread_create(&thread[i], NULL, threadCode, (void *)&idthread[i]);
    }

    //espera todas as thread finalizarem
    for(int i = 0; i < T; i++)
        pthread_join(thread[i], NULL);

    //print do resultado
    cout << "total votos = " << total_votos << endl;
    cout << "candidato | votos | percent" << endl;
    cout << "   nulo   |" << setw(4) << buffer[0] << "   |" << setw(6) << setprecision(2) << fixed << (buffer[0]*100.0)/total_votos << "%" << endl;
    for(int i = 1; i < buffer.size(); i++)
        cout << setw(6) << i << "    |" << setw(4) << buffer[i] << "   |" << setw(6) << setprecision(2) << fixed << (float)(buffer[i]*100)/total_votos << "%" << endl;
    
    pthread_exit(NULL);
}