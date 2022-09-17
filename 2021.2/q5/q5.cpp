#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

typedef struct elem{

    int value;
    struct elem *prox;

}Elem;

typedef struct blockingQueue{

    unsigned sizeBuffer, statusBuffer;
    Elem *head, *last;
    
}BlockingQueue;

#define NUM_CONS 5
#define NUM_PROD 2
#define TAM_BUFFER 10
int buffer[TAM_BUFFER];
BlockingQueue *buffer_queue;

pthread_mutex_t mymutex;
pthread_cond_t bufferEmpty;
pthread_cond_t bufferFull;

Elem *newElem(int newValue, Elem *next);
BlockingQueue *newBlockingQueue(unsigned inSizeBuffer);
void putBlockingQueue(BlockingQueue *Q, int newValue);
int takeBlockingQueue(BlockingQueue *Q);

//codigo para o produtor
void *threadCodeProd(void *id){
    
    //produz infinitamente
    while(1){

        int randnumber = rand() % 20;
        putBlockingQueue(buffer_queue, randnumber);
    }

    pthread_exit(NULL);
}

//codigo para o consumidor
void *threadCodeCons(void *id){
    
    //consome infinitamente
    while(1){

        takeBlockingQueue(buffer_queue);
    }

    pthread_exit(NULL);
}


int main(){

    ios_base::sync_with_stdio(false);
    srand(time(0));

    //threads produtoras e consumidoras
    pthread_t threads_prod[NUM_PROD];
    pthread_t threads_cons[NUM_CONS];

    mymutex = PTHREAD_MUTEX_INITIALIZER;
    bufferEmpty = PTHREAD_COND_INITIALIZER;
    bufferFull = PTHREAD_COND_INITIALIZER;

    //buffer como queue
    buffer_queue = newBlockingQueue(TAM_BUFFER);
    
    //criaçao threads produtoras
    for(int i = 0; i < NUM_PROD; i++){

        pthread_create(&threads_prod[i], NULL, threadCodeProd, NULL);
    }

    //criaçao threads consumidoras
    for(int i = 0; i < NUM_CONS; i++){

        pthread_create(&threads_cons[i], NULL, threadCodeCons, NULL);
    }

    //espera todas as thread finalizarem
    // for(int i = 0; i < NUM_THREADS; i++)
    //     pthread_join(threads[i], NULL);

    // for(int i = 0; i < NUM_THREADS; i++)
    //     free(idthread[i]);
    
    pthread_exit(NULL);
}

//cria novo elemento
Elem *newElem(int newValue, Elem *next){

    Elem *ret = (Elem *)malloc(sizeof(Elem));
    ret->value = newValue;
    ret->prox = next;

    return ret;
}

//cria nova blocking queue
BlockingQueue *newBlockingQueue(unsigned inSizeBuffer){

    BlockingQueue *ret = (BlockingQueue *)malloc(sizeof(BlockingQueue));
    ret->sizeBuffer = inSizeBuffer;
    ret->statusBuffer = 0;

    ret->head = newElem(0, NULL);
    ret->last = ret->head;

    return ret;
}

//coloca elemento na queue, garantindo exclusão mútua
void putBlockingQueue(BlockingQueue *Q, int newValue){

    pthread_mutex_lock(&mymutex);   

    //enquanto a fila estiver cheia as threads produtoras sao colocadas para dormir
    while(Q->statusBuffer == Q->sizeBuffer){

        cout << "Fila cheia!" << endl;
        pthread_cond_wait(&bufferFull, &mymutex);
    }
    
    //adiciona novo elemento na queue
    Q->last->prox = newElem(newValue, NULL);
    Q->last = Q->last->prox;
    Q->statusBuffer += 1;

    cout << "Colocando " << setw(2) << newValue << " no buffer!" << endl;

    //caso tenha alguma thread consumidora dormindo
    if(Q->statusBuffer == 1)
        pthread_cond_broadcast(&bufferEmpty);

    pthread_mutex_unlock(&mymutex);
}

//retira elemento da queue
int takeBlockingQueue(BlockingQueue *Q){

    pthread_mutex_lock(&mymutex);

    //enquanto não há elementos para consumir as threads consumidoras dormem
    while(Q->statusBuffer == 0){

        cout << "Fila vazia!" << endl;
        pthread_cond_wait(&bufferEmpty, &mymutex);
    }

    //retirando elemento da queue
    Elem *aux = Q->head->prox;
    int ret = aux->value;

    Q->head->prox = Q->head->prox->prox;

    if(Q->head->prox == NULL)
        Q->last = Q->head;
    
    Q->statusBuffer--;

    cout << "Retirando " << setw(2) << ret << " do buffer!" << endl;

    //caso alguma thread produtora esteja dormindo
    if(Q->statusBuffer == Q->sizeBuffer - 1)
        pthread_cond_broadcast(&bufferFull);

    free(aux);

    pthread_mutex_unlock(&mymutex);

    return ret;
}