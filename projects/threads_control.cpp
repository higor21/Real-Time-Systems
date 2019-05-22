#include "../../BlackGPIO/BlackGPIO.h"
#include "../../ADC/Adc.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/resource.h>

using namespace BlackLib;

int priorities[2] = {1, 1};
pthread_mutex_t mutex[2] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

struct Params
{
    BlackGPIO *led;
    int taskID;
    
    Params(int id, BlackGPIO *led): taskID(id), led(led) {}
};

void weight(int);

void* task(void*);

int main()
{    
    pthread_t thread0, thread1;
    ADC pot0 = ADC(AIN0);
    ADC pot1 = ADC(AIN1);
    BlackGPIO led0 = BlackGPIO(GPIO_48, output);
    BlackGPIO led1 = BlackGPIO(GPIO_51, output);    
    struct Params params_thread0(0, &led0);
    struct Params params_thread1(1, &led1);  
    
    pthread_create(&thread0, NULL, task, &params_thread0);
    pthread_create(&thread1, NULL, task, &params_thread1);
    
    while(true)
    {
        pthread_mutex_lock(&mutex[0]);
        priorities[0] = (int) 19*pot0.getPercentValue()/100.0;
        pthread_mutex_unlock(&mutex[0]);
        
        pthread_mutex_lock(&mutex[1]);
        priorities[1] = (int) 19*pot1.getPercentValue()/100.0;
        pthread_mutex_unlock(&mutex[1]);
        
        sleep(1);
    }
}

void weight(int n = 4000)
{
    double sum = 0.0;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            sum += (double) 0.9*0.9*(1.0/3) - 0.27;
        }
    }
}

void* task(void* params)
{
    struct Params *P = (struct Params*) params;
    while(true)
    {
        pthread_mutex_lock(&mutex[P->taskID]);
        setpriority(PRIO_PROCESS, 0, priorities[P->taskID]); 
        pthread_mutex_unlock(&mutex[P->taskID]);
        
        std::cout << "Priority of Task " << P->taskID << ": " << getpriority(PRIO_PROCESS, 0) << std::endl;
        
        P->led->setValue(high);
        weight(4000);
        P->led->setValue(low);
        weight(4000);
    }
}