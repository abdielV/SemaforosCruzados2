#include<fcntl.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

const int repeticiones = 20;
char buffer[10];

void * productor(void * parametro);
void * consumidor(void * parametro);

int main(void){
    pthread_t prod1, prod2, cons;
    sem_open("productor", O_CREAT, S_IRUSR | S_IWUSR, 1);//Lectura y escritura para el usuario
    sem_open("consumidor", O_CREAT, S_IRUSR | S_IWUSR, 0);//S_IRUSR y S_IWUSR son parte de fcntl.h
    
    pthread_create(&prod1, NULL, productor, "1234");
    pthread_create(&prod2, NULL, productor, "5678");
    pthread_create(&cons, NULL, consumidor, NULL);

    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(cons, NULL);
    
    return 0;
}

void * productor(void * parametro){
    char* cadena = (char *)malloc(10 * sizeof(char));
    if(parametro == NULL)
        strcpy(cadena, "Vacio\0");
    else
        strcpy(cadena, (char *)parametro);
    

    sem_t * semProd = sem_open("productor", 0);
    sem_t * semCons = sem_open("consumidor", 0);

    for(int i = 0; i < repeticiones; i++){
        sem_wait(semProd);
        printf("Productor en la iteración(%d)\n", i);fflush(stdout);
        strcpy(buffer, cadena);
        sem_post(semCons);
    }

    pthread_exit(NULL);
}

void * consumidor(void * parametro){
    sem_t * semProd = sem_open("productor", 0);
    sem_t * semCons = sem_open("consumidor", 0);
    
    for(int i = 0; i < repeticiones * 2; i++){
        sem_wait(semCons);
        printf("\t\t\t\t...Consumido___%s___\n", buffer);fflush(stdout);
        sem_post(semProd);
    }

    pthread_exit(NULL);
}