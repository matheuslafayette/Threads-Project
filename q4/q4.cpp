#include <bits/stdc++.h>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

#define NUM_THREADS 3
#define ENTRADA 2
string str = "./entrada" + to_string(ENTRADA) + "/input";
const char *nameArq = str.c_str();

//grafo (considera-se como conectado)
vector<vector<int>> adj;
//vetor de visitados
vector<bool> vis;
stack<int> buffer;
int n; //numero de vertices
int m; //numero de arestas

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bufferempty = PTHREAD_COND_INITIALIZER;

//coloca um vértice no buffer
void put(int vert){

    //coloca vértice no buffer
    buffer.push(vert);

    //vértice marcado como visitado
    vis[vert] = true;

    //caso buffer estivesse vazio
    //manda sinal para a variável condicinal que pode estar dormindo
    if(buffer.size() == 1)
        pthread_cond_broadcast(&bufferempty);
}

//retira um vértice do buffer
int get(){

    pthread_mutex_lock(&mymutex);

    //espera enquanto o buffer está vazio
    while(buffer.empty())
        pthread_cond_wait(&bufferempty, &mymutex);
    
    //guarda e retira o vértice do buffer
    int ret = buffer.top();
    buffer.pop();
    
    pthread_mutex_unlock(&mymutex);

    return ret;
}

void dfs(int origem){

    //for para visitar todos os vértices adjascentes a um dado vértice
    for(auto i = adj[origem].begin(); i != adj[origem].end(); i++){

        pthread_mutex_lock(&mymutex);

        //caso o vértice ainda nao tenha sido visitado
        //coloca ele no buffer
        if(!vis[*i])
            put(*i);
        
        pthread_mutex_unlock(&mymutex);
    }
}

void *threadCode(void *id){

    int idthread = *(int *)id;

    //numero predefinido de vértices para cada thread
    for(int i = idthread; i < n; i += NUM_THREADS){

        //cada thread pega um vértice do buffer
        //coloca no buffer todos os vértices adjascentes
        //e repete
        int vert = get();
        cout << "thread " << idthread << " visitou " << vert << endl;
        dfs(vert);
    }

    pthread_exit(NULL);
}

int main(){

    ios_base::sync_with_stdio(false);

    FILE *arq = fopen(nameArq, "r");
    if(arq == NULL) exit(1);

    fscanf(arq, "%d %d", &n, &m);

    adj.resize(n);
    vis.resize(n);

    int a, b;
    while(m--){
      
        fscanf(arq, "%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    pthread_t thread[NUM_THREADS];
    int *idthread[NUM_THREADS];
    put(0);

    for(int i = 0; i < NUM_THREADS; i++){

        //cout << "main: creating thread " << i << endl; 

        idthread[i] = (int *)malloc(1 * sizeof(int));
        *idthread[i] = i;

        pthread_create(&thread[i], NULL, threadCode, (void *)idthread[i]);
    }

    //espera todas as threads terminarem de executar
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(thread[i], NULL);
    
    fclose(arq);
    
    pthread_exit(NULL);
}

