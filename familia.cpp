#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

void make_for(int n, pid_t pid_pai, pid_t pid_filho, string nome){
    for(int i=0; i<n; i++){
        sleep(1);
        cout<<"Eu sou o "<<nome<<endl;
        cout<<"Meu PID eh "<<pid_filho<<endl;
        cout<<"O PID do meu pai eh: "<<pid_pai<<"\n\n";
    }
}

int main()
{
    make_for(5, getppid(), getpid(), "Pai");
    
    pid_t pid_pf1, pid_f1n1, pid_f1n2, pid_pf2, pid_f2n3, pid_f2n4;
    
    pid_pf1 = fork(); // divide o processo
    
    switch(pid_pf1){
        case -1:
            exit(-1);
        case 0: 
            make_for(5, getppid(), getpid(), "Filho1");
            pid_f1n1 = fork();
            
            switch(pid_f1n1){
                case -1:
                    exit(-1);
                case 0: 
                    make_for(40, getppid(), getpid(), "Neto1");
                    break;
                default:
                    make_for(10, getppid(), getpid(), "Filho1");
                    pid_f1n2 = fork();
                    switch(pid_f1n2){
                        case -1:
                            exit(-1);
                        case 0:
                            make_for(40, getppid(), getpid(), "Neto2");
                            break;
                        default:
                            make_for(10, getppid(), getpid(), "Filho1");
                            break;
                    }
                    break;
            }
            break;
        default:
            make_for(10, getppid(), getpid(), "Pai");
            pid_pf2 = fork();
            
            switch(pid_pf2){
                case -1:
                    exit(-1);
                case 0: 
                    make_for(5, getppid(), getpid(), "Filho2");
                    pid_f2n3 = fork();
                    switch(pid_f2n3){
                        case -1:
                            exit(-1);
                        case 0:
                            make_for(40, getppid(), getpid(), "Neto3");
                            break;
                        default:
                            make_for(10, getppid(), getpid(), "Filho2");
                            pid_f2n4 = fork();
                            switch(pid_f2n4){
                                case -1:
                                    exit(-1);
                                case 0:
                                    make_for(40, getppid(), getpid(), "Neto4");
                                    break;
                                default:
                                    make_for(10, getppid(), getpid(), "Filho2");
                                    break;
                            }
                            break;
                    }
                    break;
                default:
                    make_for(10, getppid(), getpid(), "Pai");
                    break;
            }
            break;
    }
    
    return 0;
}

