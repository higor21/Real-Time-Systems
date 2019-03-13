#include <iostream>
#include "BlackGPIO/BlackGPIO.h"
#include "PWM/PWM.h"
#include "ADC/Adc.h"
#include <cstring>

#define MAX_CONT 100

using namespace BlackLib;

	
int main(){
	// pinos digitais
	BlackGPIO pushbutton(GPIO_14, input);
	BlackGPIO digLed(GPIO_67, output);

	// pinos analógicos (PWM e ADC)
	ADC adcPtm(AINx::AIN0);
	PWM analogLed(P9_21);
	int period = 1000000;

	analogLed.setState(statePwm::run);
	analogLed.setPeriod(period);
 
	unsigned int cont = 0;
	float dutyCycle;
	
	while(true){
		if(!strcmp(pushbutton.getValue().c_str(),"0")){
			analogLed.setDutyCycle(0.0);
			digLed.setValue(low);
		}else{
			
			digLed.setValue(high);
			
			dutyCycle = adcPtm.getPercentValue();
			//std::cout<<"duty: "<<(dutyCycle*period/100.0)<<std::endl;
			analogLed.setDutyCycle(dutyCycle*period/100.0);
			//std::cout<<analogLed.getDutyCycle()<<std::endl;

		}
		// show state
		if(cont == MAX_CONT){
			std::cout<<"\n\nState: \n";
			std::cout<<"Led: "<<(strcmp(pushbutton.getValue().c_str(),"0") ? "high" : "low") << "\n";
			std::cout<<"PWM: "<<(analogLed.getDutyCycle()*100.0/period)<<"%\n";
			cont = 0;
		}
		cont++;
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
