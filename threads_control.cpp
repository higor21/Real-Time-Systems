/*
 *  thread3.cpp
 *  teste
 *
 *  Created by Luiz Affonso on 04/09/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

// programa thread3.cpp
// Programa que executar duas threads simultaneamente: main () e thread_function() 
// Para compilá-lo utilise: g++ -o thread3 thread3.cpp -lpthread

//#include "thread3.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "BlackGPIO/BlackGPIO.h"
#include "PWM/PWM.h"
#include "ADC/Adc.h"
#include <sched.h>

using namespace BlackLib;

void *thread_function(void *arg);
void carga(int k);

int main() {

    ADC adc1(AINx::AIN0); // potentiometer 
    ADC adc2(AINx::AIN1); // potentiometer 
    BlackGPIO led_1;
    BlackGPIO led_2;

    //pthread_t a_thread;
    int res_task_1, res_task_2;
    pthread_t task_1, task_2;

    struct sched_param params_task_1;
    struct sched_param params_task_2;

    res_task_1 = pthread_create(&task_1, NULL, thread_function, (BlackGPIO*) led_1);
    if (res_task_1 != 0) {
        perror("task_1 creation failed");
        exit(EXIT_FAILURE);
    }
    res_task_2 = pthread_create(&task_1, NULL, thread_function, (BlackGPIO*) led_2);
    if (res_task_2 != 0) {
        perror("task_2 creation failed");
        exit(EXIT_FAILURE);
    }
    //void *thread_result;
    //int print_count1 = 0;   
    while(true){
        params_task_1.sched_priority = int(adc1.getPercentValue()*(SCHED_FIFO-2)/100.0 + 2);
        params_task_2.sched_priority = int(adc2.getPercentValue()*(SCHED_FIFO-2)/100.0 + 2);

        std::cout<<"Prioridade da Tarefa 1: "<<params_task_1.sched_priority<<std::endl;
        std::cout<<"Prioridade da Tarefa 2: "<<params_task_2.sched_priority<<std::endl;

        ret = pthread_setschedparam(task_1, SCHED_FIFO, &params_task_1);
        if (ret != 0) {
            std::cout << "Unsuccessful in setting thread realtime prio" << std::endl;
            return;     
        }

        ret = pthread_setschedparam(task_2, SCHED_FIFO, &params_task_2);
        if (ret != 0) {
            std::cout << "Unsuccessful in setting thread realtime prio" << std::endl;
            return;     
        }

        sleep(1);
    }
    
    exit(EXIT_SUCCESS);
}

void *thread_function(BlackGPIO* led) {
    while(true) {
        led.setValue(high);
        carga(1000);
        led.setValue(low);
        carga(1000);
    }
}


// função para simular carregamento da CPU
void carga(int k){
    float f = 0.999999;
    for(int i=0; i<k; i++){
        f = f*f*f*f*f;
        f = 1.56;
        for(int j=0; j<k; j++)
	     f = sin(f)*sin(f)*f*f*f;
    }
}
