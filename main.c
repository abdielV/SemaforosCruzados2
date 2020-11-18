#include<fcntl.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

const int repeticiones = 30;
char buffer1[10];
char buffer2[10];

void * productor(void * parametro);
void * consumidor(void * parametro);

int main(void){
    pthread_t prod1, prod2, prod3, cons1, cons2, cons3;
    sem_open("productor1", O_CREAT, S_IRUSR | S_IWUSR, 1);//Lectura y escritura para el usuario
    sem_open("consumidor1", O_CREAT, S_IRUSR | S_IWUSR, 0);//S_IRUSR y S_IWUSR son parte de fcntl.h
    sem_open("productor2", O_CREAT, S_IRUSR | S_IWUSR, 1);
    sem_open("consumidor2", O_CREAT, S_IRUSR | S_IWUSR, 0);

    pthread_create(&prod1, NULL, productor, "0--");
    pthread_create(&prod2, NULL, productor, "-1-");
    pthread_create(&prod3, NULL, productor, "--2");
    pthread_create(&cons1, NULL, consumidor, NULL);
    pthread_create(&cons2, NULL, consumidor, NULL);
    pthread_create(&cons3, NULL, consumidor, NULL);

    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(prod3, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);
    pthread_join(cons3, NULL);
    
    return 0;
}

void * productor(void * parametro){
    char* cadena = (char *)malloc(10 * sizeof(char));
    if(parametro == NULL)
        strcpy(cadena, "Vacio\0");
    else
        strcpy(cadena, (char *)parametro);
    
    sem_t * semProd;
    sem_t * semCons;
    char * buffer;

    for(int i = 0; i < repeticiones; i++){
        if(i%2){
            semProd = sem_open("productor1", 0);
            semCons = sem_open("consumidor1", 0);
            buffer = buffer1;
        } else {
            semProd = sem_open("productor2", 0);
            semCons = sem_open("consumidor2", 0);
            buffer = buffer2;
        }
        sem_wait(semProd);
        strcpy(buffer, cadena);
        printf("P(%s):%d\n", buffer, i);fflush(stdout);
        sem_post(semCons);
    }

    pthread_exit(NULL);
}

void * consumidor(void * parametro){
    sem_t * semProd;
    sem_t * semCons;
    char * buffer;
    
    for(int i = 0; i < repeticiones; i++){
        if(i%2){
            semProd = sem_open("productor1", 0);
            semCons = sem_open("consumidor1", 0);
            buffer = buffer1;
        } else {
            semProd = sem_open("productor2", 0);
            semCons = sem_open("consumidor2", 0);
            buffer = buffer2;
        }
        sem_wait(semCons);
        printf("C(%s):%d\n", buffer, i);fflush(stdout);
        sem_post(semProd);
    }

    pthread_exit(NULL);
}