#include <bits/stdc++.h>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

#define BUFFER_SIZE 5
#define NUM_THREADS 3

vector<vector<int>> adj;
vector<bool> vis;
vector<int> buffer(BUFFER_SIZE);
int n;
int m;
int last = 0;
int first = 0;
int items = 0;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty_ = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill_ = PTHREAD_COND_INITIALIZER;

void put(int i)
{

  while (items == BUFFER_SIZE)
  {
    pthread_cond_wait(&empty_, &mymutex);
  }

  buffer[last] = i;
  // printf("pos %d: ", last);
  items++;
  last++;

  if (last == BUFFER_SIZE)
  {
    last = 0;
  }

  if (items == 1)
  {
    pthread_cond_broadcast(&fill_);
  }

  pthread_mutex_unlock(&mymutex);
}

int get()
{
  int result;
  pthread_mutex_lock(&mymutex);

  while (items == 0)
  {
    pthread_cond_wait(&fill_, &mymutex);
  }

  result = buffer[first];

  //  printf("pos %d: ", first);
  items--;
  first++;
  vis[result] = true;

  if (first == BUFFER_SIZE)
  {
    first = 0;
  }

  if (items == BUFFER_SIZE - 1)
  {
    pthread_cond_signal(&empty_);
  }

  pthread_mutex_unlock(&mymutex);

  return result;
}

void dfs(int origem){

    // vis[origem] = true;

    for(auto i = adj[origem].begin(); i != adj[origem].end(); i++){

      pthread_mutex_lock(&mymutex);

      if(!vis[*i])
          put(*i);
      else
          pthread_mutex_unlock(&mymutex);
    }
}

void *threadCode(void *id){

    int idthread = *(int *)id;

    for(int i = idthread; i < n; i += NUM_THREADS){
        
        int origem = get();
        cout << "thread " << idthread << " visiting " << origem << endl;
        dfs(origem);
    }

    pthread_exit(NULL);
}

int main(){

    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    cin >> n >> m;
    adj.resize(n+1);
    vis.resize(n+1);

    int a, b;
    while(m--){
      
      cin >> a >> b;
      adj[a].push_back(b);
    }

    pthread_t thread[NUM_THREADS];
    int *idthread[NUM_THREADS];
    put(0);

    for(int i = 0; i < NUM_THREADS; i++){

        cout << "main: creating thread " << i << endl; 

        idthread[i] = (int *)malloc(1 * sizeof(int));
        *idthread[i] = i;

        pthread_create(&thread[i], NULL, threadCode, (void *)idthread[i]);
    }

    // for(int i = 0; i < n; i++)
    //     if(!vis[i])
    //         dfs(i);

    for(int i = 0; i < NUM_THREADS; i++)
      pthread_join(thread[i], NULL);
    
    pthread_exit(NULL);
}

/*
20 19
0 1
1 2
1 3
1 4
1 5
1 6
1 7
2 8
3 9
4 10
5 11
6 12
7 13
8 14
14 15
15 16
16 17
17 18
18 19
*/