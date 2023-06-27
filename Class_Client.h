#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <thread>   
#include "Class_Job.h"
#include "myconnect.h"
#include "protocolo.h"
#include "thread"
using namespace std;

class Client {
private:
    //Comunicación con Master
    int fd_socket;
    uint32_t IP_MASTER;
    int nro_port;
    //Comunicación con Servers
    int interactive_fd_socket;
    uint32_t IP_interactive_server;
    uint16_t interactive_nro_port;
    thread thread_client;
    struct sockaddr_in server2client_address;
    socklen_t len_server2client_address;
    
public:
    Client(int nro_port_, uint32_t IP_, uint8_t interactive){  
        IP_MASTER = IP_;
        nro_port = nro_port_;
        fd_socket = myconnect(nro_port_, IP_MASTER);                                  
        if(interactive == 1){
            interactive_fd_socket = socket(AF_INET, SOCK_STREAM, 0);                 // SOCK_STREAM : TCP, AF_INET
            if (interactive_fd_socket  == -1) {
                perror("socket");
                exit(1);
            }
            server2client_address.sin_family = AF_INET;
            server2client_address.sin_addr.s_addr = INADDR_ANY;                        
            server2client_address.sin_port = htons(00);                              // Nro arriba de mil
            bind(interactive_fd_socket, (struct sockaddr *) &server2client_address, sizeof(server2client_address)); // Asociar el socket a un path
            listen(interactive_fd_socket, 5);                                        // Chequear nro de clientes 5
            len_server2client_address = sizeof(server2client_address);
            if( getsockname(interactive_fd_socket, (struct sockaddr *)&server2client_address, &len_server2client_address) != 0){
                perror("getsockname");
                exit(1);
            }
            // En este puerto me pongo a hacer accept() del server
            interactive_nro_port = ntohs(server2client_address.sin_port);
        }
    }

//Métodos de la clase Client

    int get_fd_socket(){
        return this->fd_socket;
    }

    int get_fd_socket_interactive(){
        return this->interactive_fd_socket;
    }

    void Submit(Job job){
        //Tengo el Job en el cliente para ser entregado al master. Usar sendMsg
        Msg msg;
        setSubmitJob(&msg, job.working_dir, job.program, job.args, job.interactive, job.interactive_address, job.interactive_nro_port);
        msg.payload.submitJob.interactive_nro_port = interactive_nro_port;
        sendMsg(get_fd_socket(), &msg);
    }

    void Delete(uint8_t jobid){
        Msg msg;
        setDeleteJob(&msg, jobid);
        sendMsg(get_fd_socket(), &msg);
    }

    void State(uint8_t jobid){
        Msg msg;
        setQueryJob(&msg, jobid);
        sendMsg(get_fd_socket(), &msg);
    }
/*
    //Trabajo a futuro: ver la cantidad de jobs encolados
    void Count(){
        Msg msg;
        setQueryCount(&msg);
        sendMsg(get_fd_socket(), &msg);
    }
*/

void RunInteractive();

void JoinInteractive();

void RunAccept();

void STDreader(int cs);

};
