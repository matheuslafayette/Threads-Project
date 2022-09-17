#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

#define N 5 //reader
#define M 2 //writer
#define TAM_BUFFER 20

vector<int> buffer(TAM_BUFFER, 0); 

pthread_mutex_t mutexBuffer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexReading = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexWriting = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t readingBuffer = PTHREAD_COND_INITIALIZER;
pthread_cond_t writingBuffer = PTHREAD_COND_INITIALIZER;

int num_reading = 0;
bool writing = false;

void *write(void *id){
    
    while(true){

        pthread_mutex_lock(&mutexBuffer);

            pthread_mutex_lock(&mutexWriting);
                writing = true;
            pthread_mutex_unlock(&mutexWriting);

            pthread_mutex_lock(&mutexReading);
                while(num_reading > 0)
                    pthread_cond_wait(&readingBuffer, &mutexReading);
            pthread_mutex_unlock(&mutexReading);

            int rand_pos = rand() % TAM_BUFFER;
            int rand_num = rand() % 100;

            buffer[rand_pos] = rand_num;
            cout << "writing: v[" << rand_pos << "] = " << rand_num << endl;

            pthread_mutex_lock(&mutexWriting);
                writing = false;
                pthread_cond_broadcast(&writingBuffer);    
            pthread_mutex_unlock(&mutexWriting);

        pthread_mutex_unlock(&mutexBuffer);
    }

    pthread_exit(NULL);
}

void *read(void *id){
    
    while(true){

        pthread_mutex_lock(&mutexWriting);

            while(writing)
                pthread_cond_wait(&writingBuffer, &mutexWriting);

            pthread_mutex_lock(&mutexReading);
                num_reading++;
            pthread_mutex_unlock(&mutexReading);

        pthread_mutex_unlock(&mutexWriting);

        int rand_pos = rand() % TAM_BUFFER;
        cout << "reading: v[" << rand_pos << "] = " << buffer[rand_pos] << endl;

        pthread_mutex_lock(&mutexReading);
            num_reading--;
            if(num_reading == 0)    
                pthread_cond_broadcast(&readingBuffer);
        pthread_mutex_unlock(&mutexReading);
    }

    pthread_exit(NULL);
}


int main(){

    ios_base::sync_with_stdio(false);
    srand(time(0));

    //threads produtoras e consumidoras
    pthread_t threads_write[M];
    pthread_t threads_read[N];
    
    //create writer
    for(int i = 0; i < M; i++)
        pthread_create(&threads_write[i], NULL, write, NULL);
    
    //create reader
    for(int i = 0; i < N; i++)
        pthread_create(&threads_read[i], NULL, read, NULL);
    
    //join xd
    for(int i = 0; i < M; i++)
        pthread_join(threads_write[i], NULL);
    
    //join xd
    for(int i = 0; i < N; i++)
        pthread_join(threads_read[i], NULL);
    
    pthread_exit(NULL);
}