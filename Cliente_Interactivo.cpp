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

int master_nro_port;
hostent* master;
Job job;
uint8_t jobid;
int main(int argc, char** argv) {
    int option;
    string ip_address;
    master_nro_port = 0;
    master = nullptr;
    jobid = 0;
    job.interactive = 1;
    while ((option = getopt(argc, argv, "p:i:e:d:a:")) != -1) {
        switch (option) {
            case 'p':
                master_nro_port = stoi(optarg);
                break;
            case 'i':
                ip_address = optarg;
                master = gethostbyname(ip_address.c_str());
                if (master == nullptr) {
                    cerr << "Error: al cargar ip del master" << endl;
                    return 1;
                }
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
                cout << "Usage: ./Cliente_Interactivo -p <port> -i <ip> -e <ejecutarJob> -d <direcciónJob> -a <argumentosJob>" << endl;
                return -1;
        }
    }
    Client client(master_nro_port, *(uint32_t*)master->h_addr, job.interactive);
    client.Submit(job);
    client.RunInteractive();

    // hay que hacer un select, para mandarle al server lo que tipee en el cliente
    // (Ejemplo que lee de standar input y de un pipe, y del socket con un select). P2_ej4.cpp

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
                break;
            default:
                break;
        }
    }
    client.JoinInteractive();
    return 0;
}