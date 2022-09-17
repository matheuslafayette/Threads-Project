#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

const int TAM = 5;
vector<bool> forks(TAM, true);
vector<pthread_mutex_t> mutexs(TAM, PTHREAD_MUTEX_INITIALIZER);

void *dinner(void *id_t){

    int id = *(int *)id_t;
    int esq = (id+1) % TAM;
    int dir = id;
    
    while(true){

        //get_forks();
        int try_esq = pthread_mutex_trylock(&mutexs[esq]);
        int try_dir = pthread_mutex_trylock(&mutexs[dir]);

        if(!try_esq && !try_dir){

            cout << "thread " << id << " eating" << endl;
            //eat
            this_thread::sleep_for(chrono::milliseconds(500));
            pthread_mutex_unlock(&mutexs[dir]);
            pthread_mutex_unlock(&mutexs[esq]);

            //think
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        else if(!try_esq)
            pthread_mutex_unlock(&mutexs[esq]);
        
        else if(!try_dir)
            pthread_mutex_unlock(&mutexs[dir]);
    }

    pthread_exit(NULL);
}

int main(){

    ios_base::sync_with_stdio(false);

    //threads produtoras e consumidoras
    pthread_t threads[TAM];
    int id[TAM];

    for(int i = 0; i < TAM; i++){

        id[i] = i;
        pthread_create(&threads[i], NULL, dinner, (void *)&id[i]);
    }

    for(int i = 0; i < TAM; i++)
        pthread_join(threads[i], NULL);
    

    pthread_exit(NULL);
}