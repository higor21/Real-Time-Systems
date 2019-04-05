#include "BlackGPIO/BlackGPIO.h"
#include "PWM/PWM.h"
#include "ADC/Adc.h"
#include <stdio.h>
#include <unistd.h>  // sleep()
#include <cstring>
#include <sys/time.h>
#include <cstdlib> // rand()
#include <time.h>
#include <vector>
#include <iostream>

#define MAX_STR 255
#define NUM_BUTTONS 3
#define NUM_LEDS 3
#define NUM_LEDS_DISPLAY 8

using namespace BlackLib;
	
// change the display to show the number 'number'
void set_display(BlackGPIO display_obj[], unsigned int number){
	vector<int> display;

    switch(number) {

        case 0:
            display = {1, 1, 1, 1, 1, 1, 0, 0};
            break;
        case 1:
            display = {0, 1, 1, 0, 0, 0, 0, 0};
            break;
        case 2:
            display = {1, 1, 0, 1, 1, 0, 1, 0};
            break;
        case 3:
            display = {1, 1, 1, 1, 0, 0, 1, 0};
            break;
        case 4:
            display = {0, 1, 1, 0, 0, 1, 1, 0};
            break;
        case 5:
            display = {1, 0, 1, 1, 0, 1, 1, 0};
            break;
        case 6:
            display = {1, 0, 1, 1, 1, 1, 1, 0};
            break;
        case 7:
            display = {1, 1, 1, 0, 0, 0, 0, 0};
            break;
        case 8:
            display = {1, 1, 1, 1, 1, 1, 1, 0};
            break;
        case 9:
            display = {1, 1, 1, 1, 0, 1, 1, 0};
            break;
        default:         
        	display = {0, 0, 0, 0, 0, 0, 0, 1};
    }	

    for (int i = 0; i < NUM_LEDS_DISPLAY; ++i){
    	display_obj[i].setValue((display[i]) ? high : low);
    }
}

void set_leds(BlackGPIO leds[], int led /* 0, 1, 2 ou -1*/){
	for (unsigned i = 0; i < NUM_LEDS; ++i){
		leds[i].setValue((i != (unsigned) led || led == -1) ? low : high);
	}
}

int calc_time_ms(struct timeval time_init, struct timeval time_finish){
    return (int) (1000 * (time_finish.tv_sec - time_init.tv_sec) + (time_finish.tv_usec - time_init.tv_usec) / 1000.0);
}

bool verify_if_pressed(BlackGPIO button, BlackGPIO led){
    // retorna verdadeiro se o botão for pressionado. Enquanto ele for pressionado, o led correspondente ficará aceso.
    if(button.getValue() == "1"){
    	led.setValue(high);
    	while(button.getValue() == "1")
    	led.setValue(low);
    	return true;
    }
    return false;
}

bool verify_hit(int r_vet[], int c_vet[], int t){
	for (int i = 0; i < t; ++i)
		if (c_vet[i] != r_vet[i]) return false;
	return true;
}

int main(){

    /* Variable Definitions */

    // analog pins
    ADC adcPtm(AINx::AIN0); // potentiometer 
	PWM analogLed(P9_21); // led to show when the user lost
    int period_pwm = 1000000;
	analogLed.setState(statePwm::run);
	analogLed.setPeriod(period_pwm);
	
    // display
    BlackGPIO displayLed[] = {
    	BlackGPIO(GPIO_10, output), BlackGPIO(GPIO_9, output), BlackGPIO(GPIO_8, output),
    	BlackGPIO(GPIO_78, output), BlackGPIO(GPIO_76, output), BlackGPIO(GPIO_74, output),
    	BlackGPIO(GPIO_72, output), BlackGPIO(GPIO_70, output)
    };
	// buttons
    BlackGPIO buttons[] = {BlackGPIO(GPIO_38, input), BlackGPIO(GPIO_39, input), BlackGPIO(GPIO_67, input)};
    // leds
    BlackGPIO leds[] = {BlackGPIO(GPIO_26, output), BlackGPIO(GPIO_65, output), BlackGPIO(GPIO_46, output)};

    // user informations
    unsigned int score = 4;
    unsigned int dificult = 1;
    char name[MAX_STR];

	srand(time(0));

    float time_to_show, time_to_answer;
    int correct_sequence[15]; // 0, 1, 2 ou -1 (todas apagadas)
    int real_sequence[15]; // 0, 1, 2 ou -1 (todas apagadas)
    int current_led;
    
    // time
	//struct timeval time_init, time_finish;
    //int real_time_of_player;

    time_t time_init, time_finish, real_time_of_player;
	int cont_sequence_number;
	//int not_pressed;

	while(true){
		
        printf("Informe seu nome: ");
        scanf("%s", name);
        printf("Informe o nível de dificuldade (1 => facil, 2 => medio e 3 => dificil): ");
        do{ scanf("%d", &dificult); }while(dificult > 3 || dificult < 1);

        printf("Start in 3 seconds ...");
        sleep(3);

        //set_display(displayLed, 4);
        while(score < 10 && score > 0){
            // informar situação do jogador
            // mostrar pisca-pisca de acordo com o tempo
            //printf("score: %d", score);
            std::cout<<"score: " << score << std::endl;
            cont_sequence_number = 0;
            time_to_show = dificult*5;
            
	    while(time_to_show > 0){
                current_led = rand()%3;
                correct_sequence[cont_sequence_number++] = current_led;
		//printf("%d ", correct_sequence[cont_sequence_number - 1]);
		set_leds(leds, current_led);
                usleep(500000); // dorme 500ms para mostrar um led aceso
                time_to_show -= 1;
                std::cout << "TIME_TO_SHOW: " << time_to_show << endl;
            }
            
            printf("Apos 3 segundos o jogador porderá responder ...\n");
            sleep(3);

            // informa quanto tempo o jogador tem para responder 
            time_to_answer = adcPtm.getPercentValue()*15/100.0 - dificult*3; // máximo 12s e mínimo 6s para resposta
            printf("\n%s, vc tem %.2f segundos para responder", name, time_to_answer);
            
            // pegando a sequencia do jogador
            real_time_of_player = 0; // [T] = ms
            cont_sequence_number = 0;
            while(time_to_answer >= real_time_of_player/1000.0 || cont_sequence_number >= (int) dificult*5){
				std::cout<<"\ntime: "<<real_time_of_player;
				time_init = time(NULL);
                //gettimeofday(&time_init, NULL);
                //not_pressed = 0;
                printf("pass 1"); 
                for (int i = 0; i < NUM_BUTTONS; ++i){
                	if (verify_if_pressed(buttons[i], leds[i])){
                		real_sequence[cont_sequence_number++] = i;
                	}
                }
                //if(not_pressed == 3) real_sequence[cont_sequence_number++] = -1;
                printf("pass 2"); 

                //gettimeofday(&time_finish, NULL);
                time_finish = time(NULL);
                printf("pass 3"); 
                real_time_of_player += difftime(time_finish,time_init);
                //real_time_of_player += calc_time_ms(time_init, time_finish); // acrescenta tempo de resposta	    
                printf("pass 4"); 
            }

            if(verify_hit(real_sequence, correct_sequence, (int) dificult*5))
            	set_display(displayLed, ++score);
	    	else
				--score;
        }
        (score == 10) ? printf("Parabens!!\n") : printf("Tente novamente, nao foi dessa vez!\n");
    }
	return 0;
}
