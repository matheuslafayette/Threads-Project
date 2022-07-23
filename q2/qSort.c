#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

//Array global que funciona como stack
int *data;

//Número de itens a serem organizados
int n = 15;


typedef struct args{
  int left, right;
} Par;

Par createPar(int l, int r){
  Par P;
  P.left = l;
  P.right = r;
  return P;
}

void swap(int *x, int *y) {
  int aux = *x;
  *x = *y;
  *y= aux;
}

//Particiona a posição
int partition(int l, int r) {
  int auxPivot = data[r];
  int i = (l - 1);

  // Compara elementos com o pivô
  for (int j = l; j < r; j++) {
    if (data[j] <= auxPivot) {
      i++;
      swap(&data[i], &data[j]);
    }
  }

  //Troca o pivô com o maior elemento
  swap(&data[i + 1], &data[r]);

  return (i + 1);
}

void printData(){
   for (int i = 0; i < n; ++i) {
    printf("%d  ", data[i]);
    }
    printf("\n");
}

void* qSort(void*args) {
  int l = ((Par*)args)->left;
  int r = ((Par*)args)->right;

  if (l < r) {
    //Imprime os dados
    printData();

    //Encontra o pivô
    int pivotIndex = partition(l,r); 

    //Parametros para as threads
    Par parLeft = createPar(l, pivotIndex-1);
    Par parRight = createPar(pivotIndex+1, r);
    //Chamada recursiva para os elementos a esquerda do pivô

    //Inicializa as threads
    pthread_t tLeft, tRight;

    pthread_create(&tLeft, NULL, qSort,&parLeft);
    pthread_create(&tRight, NULL, qSort,&parRight); //Chamada recursiva para os elementos a direita do pivô

    //Espera que as operações sejam concluidas para não haver conflito
    pthread_join(tLeft, NULL);
    pthread_join(tRight, NULL);
  }
}

void threadInitializer(){
  pthread_t t0;
  Par P0= createPar(0, n-1);
  pthread_create(&t0, NULL, qSort,&P0);

  pthread_join(t0, NULL);
}


int main() {
  //Aloca um array de tamanho n
  data = malloc(n*sizeof(int));

  //Cria um array aleatório
  srand(time(NULL));
  for(int i=0;i <n;i++){
    data[i] = rand()%50;
  }

  printf("Processando...\n");

  threadInitializer();

  printf("\nSequência final:\n");
  printData();

  //Limpa a memória
  free(data);
}