#include "BlackGPIO/BlackGPIO.h"
#include "PWM/PWM.h"
#include "ADC/Adc.h"
#include <stdio.h>
#include <unistd.h>  // sleep()
#include <cstring>
#include <sys/time.h>
#include <cstdlib> // rand()
#include <vector>

#define MAX_STR 255
#define NUM_BUTTONS 3
#define NUM_LEDS 3
#define NUM_LEDS_DISPLAY 8

using namespace BlackLib;
	
// change the display to show the number 'number'
void set_display(std::vector<BlackGPIO> &display, unsigned int number){

}

void show_to_user(){

}

void set_leds(std::vector<BlackGPIO> leds, int led /* 0, 1, 2 ou -1*/){
	for (unsigned i = 0; i < leds.size(); ++i){
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

void initialize_gpio(std::vector<BlackGPIO> itens, int ports[], int n_ports, char c){
    for(int i=0; i<n_ports; i++)
    	itens.push_back(BlackGPIO((gpioName)ports[i], (c == 'i') ? input : output));
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

    int display_ports[] = {GPIO_38, GPIO_39, GPIO_34, GPIO_35, GPIO_66, GPIO_67, GPIO_68, GPIO_69};
    int input_ports[] = {GPIO_71, GPIO_73, GPIO_75};
    int output_ports[] = {GPIO_70, GPIO_72, GPIO_74};
	
    // display
    std::vector<BlackGPIO> displayLed;
	// buttons
    std::vector<BlackGPIO> buttons;
    // leds
    std::vector<BlackGPIO> leds;

	initialize_gpio(buttons, input_ports, NUM_BUTTONS, 'i');
	initialize_gpio(leds, output_ports, NUM_LEDS, 'o');
	initialize_gpio(displayLed, display_ports, NUM_LEDS_DISPLAY, 'o');

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
    char name[MAX_STR];

	srand(time(0));

    float time_to_show, time_to_answer;
    int correct_sequence[15]; // 0, 1, 2 ou -1 (todas apagadas)
    int real_sequence[15]; // 0, 1, 2 ou -1 (todas apagadas)
    int current_led;

	struct timeval time_init, time_finish;
    int real_time_of_player;
	int cont_sequence_number;
	int not_pressed;

	while(true){
		
        printf("Informe seu nome: ");
        scanf("%s", name);
        printf("Informe o nível de dificuldade (1 => facil, 2 => medio e 3 => dificil): ");
        do{ scanf("%d", &dificult); }while(dificult > 3 || dificult < 1);

        printf("Start in 3 seconds ...");
        sleep(3);

        set_display(displayLed, 4);
        while(score < 10 || score > 0){
            // informar situação do jogador

            // mostrar pisca-pisca de acordo com o tempo
            cont_sequence_number = 0;
            time_to_show = dificult*5;
            while(time_to_show > 0){
                current_led = rand()%3;
                correct_sequence[cont_sequence_number++] = current_led;
                set_leds(leds, current_led);
                usleep(500000); // dorme 500ms para mostrar um led aceso
                time_to_show -= 1;
            }
            
            printf("Apos 3 segundos o jogador porderá responder ...");
            sleep(3);

            // informa quanto tempo o jogador tem para responder 
            time_to_answer = adcPtm.getPercentValue()*15/100.0 - dificult*3; // máximo 12s e mínimo 6s para resposta
            printf("%s tem %.2f segundos para responder", name, time_to_answer);
            
            // pegando a sequencia do jogador
            real_time_of_player = 0; // [T] = ms
            cont_sequence_number = 0;
            while(time_to_answer >= real_time_of_player/1000.0 || cont_sequence_number >= (int) dificult*5){
                gettimeofday(&time_init, NULL);
                
                not_pressed = 0;
                for (int i = 0; i < NUM_BUTTONS; ++i){
                	if (verify_if_pressed(buttons[i], leds[i])){
                		real_sequence[cont_sequence_number++] = i;
                	}
                }
                if(not_pressed == 3) real_sequence[cont_sequence_number++] = -1;

                gettimeofday(&time_finish, NULL);
                real_time_of_player += calc_time_ms(time_init, time_finish); // acrescenta tempo de resposta
            }

            if(verify_hit(real_sequence, correct_sequence, (int) dificult*5))
            	set_display(displayLed, ++score);
        }
        (score == 10) ? printf("Parabens!!\n") : printf("Tente novamente, nao foi dessa vez!\n");
    }
	return 0;
}