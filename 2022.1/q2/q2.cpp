#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;
#define NUM_INTERS 5
#define NUM_THREADS 10

int inters[NUM_INTERS];
pthread_mutex_t mymutex[NUM_INTERS];
pthread_cond_t intersFull[NUM_INTERS];

void *threadCode(void *id){

    int idthread = *(int *)id;
    int i = 0;

    while(true){

        pthread_mutex_lock(&mymutex[i]);

        while(inters[i] >= 2){

            cout << "thread " << idthread << " -> " << "insters " << i << " is full!" << endl;
            pthread_cond_wait(&intersFull[i], &mymutex[i]);
        }

        cout << "thread " << idthread << " -> " << "entered inters " << i << endl;

        inters[i]++;
        pthread_mutex_unlock(&mymutex[i]);

        this_thread::sleep_for(chrono::milliseconds(500));

        pthread_mutex_lock(&mymutex[i]);
        inters[i]--;

        cout << "thread " << idthread << " -> " << "left inters " << i << endl;

        if(inters[i] == 1)
            pthread_cond_signal(&intersFull[i]);
            
        pthread_mutex_unlock(&mymutex[i]); 

        i = ++i % NUM_INTERS;
    }

    pthread_exit(NULL);
}

int main(){

    ios_base::sync_with_stdio(false);

    pthread_t threads[NUM_THREADS];
    int idthread[NUM_THREADS];

    for(int i = 0; i < NUM_INTERS; i++){

        inters[i] = 0;
        mymutex[i] = PTHREAD_MUTEX_INITIALIZER;
        intersFull[i] = PTHREAD_COND_INITIALIZER;
    }

    for(int i = 0; i < NUM_THREADS; i++){

        idthread[i] = i;
        pthread_create(&threads[i], NULL, threadCode, (void *)&idthread[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_exit(NULL);
}