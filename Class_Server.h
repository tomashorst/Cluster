#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <wait.h>
#include <thread>
#include "Class_Job.h"
#include "myconnect.h"
#include "protocolo.h"
using namespace std;

class Server {
private:
    int fd_socket;
    uint32_t IP;
    int nro_port;

    //Comunicación con Clientes Interactivos
    int interactive_fd_socket;
    uint32_t interactive_client;
    int interactive_nro_port;
    
public:
Server(int nro_port_, uint32_t IP_){
    IP = IP_;
    nro_port = nro_port_;
}


int get_fd_socket(){
    return this->fd_socket;
}

void run() {
    fd_socket = myconnect(nro_port, IP);
    DoJob();
}

void DoJob();

void RunJob(Msg msg, Job job_received);

void Interactive(Msg msg, Job job_received);

};


