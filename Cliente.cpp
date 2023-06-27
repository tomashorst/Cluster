#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>
#include "protocolo.h"
#include "Class_Job.h" 
#include "Class_Client.h"
#include "Class_Master.h"
#include "Class_Server.h"
using namespace std;

int nro_port;
hostent* master;
Job job;
uint8_t jobid;
int main(int argc, char** argv) {
    int option;
    string ip_address;
    nro_port = 0;
    master = nullptr;
    jobid = 0;
    job.interactive = 0;
    char* comando;
    comando = (char*)malloc(7*sizeof(char));    //Quizás debe ser más grande
    while ((option = getopt(argc, argv, "p:i:c:j:e:d:a:")) != -1) {
        switch (option) {
            case 'p':
                nro_port = stoi(optarg);
                break;
            case 'i':
                ip_address = optarg;
                master = gethostbyname(ip_address.c_str());
                if (master == nullptr) {
                    cerr << "Error: al cargar ip del master" << endl;
                    return 1;
                }
                break;
            case 'c':
                strcpy(comando, optarg);
                break;
            case 'j':
                jobid = stoi(optarg);
                break;
            case 'e':
                strcpy(job.program, optarg);
                break;
            case 'd':
                strcpy(job.working_dir, optarg);
                break;
            case 'a':
                strcpy(job.args, optarg);
                break;
            default:
                cout << "Usage: ./Cliente -p <port> -i <ip_address> -c <comando> -j <jobid> -e <ejecutarJob> -d <direcciónJob> -a <argumentosJob>" << endl;
                return 1;
        }
    }
    Client client(nro_port, *(uint32_t*)master->h_addr, job.interactive);
    if(strcmp(comando, "SUBMIT") == 0){
        client.Submit(job);
    }
    else if (strcmp(comando, "DELETE") == 0)
    {
        client.Delete(jobid);
    }
    else if (strcmp(comando, "STATE") == 0)
    {
        client.State(jobid);
    }else{
        printf("Error: comando en Cliente.cpp no reconocido");
    }
    //Ahora me falta que reciba el mensaje del master y lo imprima
    while(1){
        Msg msg;
        if( recvMsg(client.get_fd_socket(), &msg) <= 0 )
            break;
        switch (msg.getType()){
            case JOB_STATUS:
                uint8_t status;
                uint32_t jobid;
                getJobStatus(&msg,&jobid,&status);        
                printf("Jobid %d en estado ", jobid);
                job.printfEstado(ESTADO(status));
                printf("\n");
                return 0;
            default:
                break;
        }
    }
    return 0;
}