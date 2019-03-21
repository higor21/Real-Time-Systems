#include <iostream>
#include "BlackGPIO/BlackGPIO.h"
#include "PWM/PWM.h"
#include "ADC/Adc.h"
#include <stdio.h>
#include <unistd.h>  // sleep()
#include <cstring>
#include <sys/time.h>
#include <cstdlib> // rand()


#define MAX_CONT 100

using namespace BlackLib;
using namespace std;
	
int main(){
	// pinos digitais
	BlackGPIO pushbutton(GPIO_14, input);
	BlackGPIO redLed(GPIO_68, output);
	BlackGPIO greenLed(GPIO_67, output);

	struct timeval time_init, time_finish;
	int dT, time_to_wait;
	srand(time(0));

	while(true){
		//cout<<strcmp(pushbutton.getValue().c_str(),"0")<<endl;
		if(strcmp(pushbutton.getValue().c_str(),"0")){
			redLed.setValue(low);
			greenLed.setValue(low);
		}else{
			redLed.setValue(high);
			greenLed.setValue(low);
			
			// espera um tempo aleatório
			time_to_wait = (rand()%5 + 3); // número aleatório entre 3 e 7
			sleep(time_to_wait);

			redLed.setValue(low);
			greenLed.setValue(high);
		
			// tempo de reação	
			gettimeofday(&time_init, NULL);
			while(!strcmp(pushbutton.getValue().c_str(),"0")){}
			gettimeofday(&time_finish, NULL);

			dT = (int) (1000 * (time_finish.tv_sec - time_init.tv_sec) + (time_finish.tv_usec - time_init.tv_usec) / 1000.0);
			if(dT > 10)
				printf("Tempo de reacao: %d milissegundos\n", dT);
			else
				printf("Erro: voce acionou o sistema antes do tempo previsto\n");
		}
	}

	return 0;
}

/*
	Escrita em pino digital
	Leitura em pino digital
	Uso de pino PWM 
	Uso de pino ADC
	Log dos estados de cada pino
*/
