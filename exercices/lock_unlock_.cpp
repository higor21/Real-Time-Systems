//  thread7.cpp
//  
//
//  Created by Affonso on 25/10/16.
//
//

// #include "thread7.hpp"

// http: pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_init.html

// Programa que sincroniza threads utilizando-se mutexes
// Para compilá-lo utilise: g++ -o thread7 thread7.cpp -lpthread


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *thread_function(void *arg);
pthread_mutex_t work_mutex; /* proteção para: work_area e time_to_exit */
int var = 0;

int main() {
    int res;
    pthread_t a_thread, b_thread;
    void *thread_result_a, *thread_result_b;
    char message_t1[] = "1";
    char message_t2[] = "2";

    res = pthread_mutex_init(&work_mutex, NULL);
    if (res != 0) {
        perror("Iniciação do Mutex falhou");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&a_thread, NULL, thread_function, (void*) message_t1);
    if (res != 0) {
        perror("Criação da Thread falhou");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&b_thread, NULL, thread_function, (void*) message_t2);
    if (res != 0) {
        perror("Criação da Thread falhou");
        exit(EXIT_FAILURE);
    }
    sleep(1);
    printf("\nMAIN() --> Esperando a thread terminar...\n");
    res = pthread_join(a_thread, &thread_result_a);
    if (res != 0) {
        perror("Jun��o da Thread falhou");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(b_thread, &thread_result_b);
    if (res != 0) {
        perror("Jun��o da Thread falhou");
        exit(EXIT_FAILURE);
    }
    printf("\nMAIN() --> Thread foi juntada com sucesso\n");
    pthread_mutex_destroy(&work_mutex);
    exit(EXIT_SUCCESS);
}

void *thread_function(void *msg) {
    while(true) {
        printf("\nTHREAD() --> %s", (char*) msg);
    	pthread_mutex_lock(&work_mutex);
	printf("\nAlterando variavel");
	sleep((strcmp("2", (char*) msg)) ? 2 : 5);
	var = (strcmp("2", (char*) msg)) ? 1 : 2;
	printf("\nVariavel alterada: %d", var);	
        pthread_mutex_unlock(&work_mutex);
        sleep(1);
    }
    pthread_exit(0);
}
