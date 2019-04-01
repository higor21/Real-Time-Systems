#include <iostream>
#include "BlackGPIO/BlackGPIO.h"
#include "PWM/PWM.h"
#include "ADC/Adc.h"
#include <stdio.h>
#include <unistd.h>  // sleep()
#include <cstring>
#include <sys/time.h>
#include <cstdlib> // rand()
#include <vector>


#define MAX_CONT 100

using namespace BlackLib;
using namespace std;
	
// change the display to show the number 'number'
void set_display(std:vector<BlackGPIO> &display, unsigned int number){

}

void show_to_user(){

}

void set_leds(unsigned led /* 0, 1 ou 2 */){

}

int calc_time_ms(struct timeval time_init, struct timeval time_finish){
    return (int) (1000 * (time_finish.tv_sec - time_init.tv_sec) + (time_finish.tv_usec - time_init.tv_usec) / 1000.0);
}

bool verify_if_pressed(BlackGPIO button){
    // retorna verdadeiro se o botão foi pressionado 
    /* Obs.: Lembrar de acender a luz no memento q o botão for pressionado 
    e de colocar um 'loop' para esperar o botão parar de ser pressionado */
}

int main(){

    /* Variable Definitions */

	// buttons
	BlackGPIO redButton(GPIO_71, input);
	BlackGPIO greenButton(GPIO_73, input);
	BlackGPIO blueButton(GPIO_75, input);

    // leds
	BlackGPIO redLed(GPIO_70, output);
	BlackGPIO greenLed(GPIO_72, output);
    BlackGPIO blueLed(GPIO_74, output);

    // analog pins
    ADC adcPtm(AINx::AIN0); // potentiometer 
	PWM analogLed(P9_21); // led to show when the user lost
    int period_pwm = 1000000;
	analogLed.setState(statePwm::run);
	analogLed.setPeriod(period_pwm);

    // display
    int num_leds = 8;
    std::vector<BlackGPIO> displayLed(num_leds);
    int ports[num_leds] = [GPIO_38, GPIO_39, GPIO_34, GPIO_35, GPIO_66, GPIO_67, GPIO_68, GPIO_69];
    for(int i=0; i<num_leds; i++) displayLed.push_back(new BlackGPIO(ports[i], output));

    /* 	BlackGPIO displayLed_a(GPIO_38, output); // a
	BlackGPIO displayLed_b(GPIO_39, output); // b
	BlackGPIO displayLed_c(GPIO_34, output); // c
	BlackGPIO displayLed_d(GPIO_35, output); // d
	BlackGPIO displayLed_e(GPIO_66, output); // e
	BlackGPIO displayLed_f(GPIO_67, output); // f
	BlackGPIO displayLed_g(GPIO_68, output); // g
	BlackGPIO displayLed_dp(GPIO_69, output); // DP */

    // user informations
    unsigned int score = 4;
    unsigned int dificult = 1;
    char name[256];

	srand(time(0));

    float time_to_show, time_to_answer;
    int correct_sequence[15]; // 0, 1, 2 ou -1 (todas apagadas)
    int real_sequence[15]; // 0, 1, 2 ou -1 (todas apagadas)
    int current_led;

	struct timeval time_init, time_finish;
    int real_time_of_player;
	int cont_sequence_number;

	while(true){
		
        printf("Informe seu nome: ")
        scanf("%s", name);
        printf("Informe o nível de dificuldade (1 => facil, 2 => medio e 3 => dificil): ");
        do{ scanf("%d", &dificult) }while(dificult > 3 || dificult <1);

        printf("Start in 3 seconds ...")
        sleep(3)

        while(score < 10 || score > 0){
            // informar situação do jogador

            // mostrar pisca-pisca de acordo com o tempo
            time_to_show = dificult*5;
            while(time_to_show > 0){
                current_led = rand()%3;
                correct_sequence[current_led];
                set_leds(current_led);
                usleep(500000); // dorme 500ms para mostrar um led aceso
                time_to_show -= 1;
            }
            
            printf("Apos 3 segundos o jogador porderá responder ...")
            sleep(3);

            // informa quanto tempo o jogador tem para responder 
            time_to_answer = adcPtm.getPercentValue()*15/100.0 - dificult*3; // máximo 12s e mínimo 6s para resposta
            printf("%s tem %.2f segundos para responder", name, time_to_answer);
            
            // pegando a sequencia do jogador
            real_time_of_player = 0; // [T] = ms
            cont_sequence_number = 0;
            while(time_to_answer >= real_time_of_player/1000.0){
                gettimeofday(&time_init, NULL);
                /* lembrar de enviar os leds para serem acionados */
                if(verify_if_pressed(redButton)){
                    real_sequence[cont_sequence_number++] = 0; //  red
                }else if(verify_if_pressed(greenButton)){
                    real_sequence[cont_sequence_number++] = 1; // green
                }else if(verify_if_pressed(blueButton)){
                    real_sequence[cont_sequence_number++] = 2; // blue
                }else{
                    real_sequence[cont_sequence_number++] = -1; // não apertou o botão
                }
                gettimeofday(&time_finish, NULL);
                real_time_of_player += calc_time_ms(time_init, time_finish); // acrescenta tempo de resposta
            }

        }
    }
	return 0;
}