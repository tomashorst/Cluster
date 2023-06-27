#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <thread>
#include "Class_Job.h"
#include "Class_Client2M.h"
#include "Class_Server2M.h"
#include "protocolo.h"
#include "Syn_Q.h"
#include "JobQueue.h"
using namespace std;

class Master {
private:
    JobQueue jobs;         // Cola sincronizada de jobs
    vector <Client2M*>clients;
    vector <Server2M*>servers;
    int jobs_id;           // Contador de jobs
    // Sockets para esperar requerimientos de clientes y servidores
    int fd_socket_client;  // File descriptor del socket del master para escuchar a los clientes
    int fd_socket_server;  // File descriptor del socket del master para escuchar a los servidores
    int nro_port_client;   // Número de puerto del master para escuchar a los clientes
    int nro_port_server;   // Número de puerto del master para escuchar a los servidores

public:

Master (int nro_port_client, int nro_port_server){
    jobs_id = 1;    //Inicializo el contador de jobs en 1
    this->nro_port_client = nro_port_client;
    this->nro_port_server = nro_port_server;

    // Socket para escuchar a los clientes
    fd_socket_client = socket(AF_INET, SOCK_STREAM, 0);                 // SOCK_STREAM : TCP, AF_INET
    if (fd_socket_client == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in master_address_client;                     
    master_address_client.sin_family = AF_INET;
    master_address_client.sin_addr.s_addr = INADDR_ANY;                        
    master_address_client.sin_port = htons(nro_port_client);            // Nro arriba de mil
    int reuse_client = 1;
    int result = setsockopt(fd_socket_client, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_client, sizeof(reuse_client));
    if ( result < 0 ) {
        perror("ERROR SO_REUSEADDR:");
    }
    bind(fd_socket_client, (struct sockaddr *) &master_address_client, sizeof(master_address_client)); // Asociar el socket a un path
    listen(fd_socket_client, 5);                                        // Chequear nro de clientes 5
    thread TC(&Master::Run_Client_Static,this);
    
    // Socket para escuchar a los servidores
    fd_socket_server = socket(AF_INET, SOCK_STREAM, 0);                 // SOCK_STREAM : TCP, AF_INET : UNIX
    if (fd_socket_server == -1) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in master_address_server;                     
    master_address_server.sin_family = AF_INET;
    master_address_server.sin_addr.s_addr = INADDR_ANY;                        
    master_address_server.sin_port = htons(nro_port_server);             // Nro arriba de mil 
    int reuse_server = 1;
    int result_server = setsockopt(fd_socket_server, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_server, sizeof(reuse_server));
    if ( result_server < 0 ) {
        perror("ERROR SO_REUSEADDR:");
    }
    bind(fd_socket_server, (struct sockaddr *) &master_address_server, sizeof(master_address_server)); // Asociar el socket a un path
    listen(fd_socket_server, 5);    
    thread TS(&Master::Run_Server_Static,this);

    TC.join();
    printf("Master: Terminated with TC\n");
    TS.join();
    printf("Master: Terminated with TS\n");
}

static void Run_Client_Static(Master *S){
    S->Run_Client();
}

void Run_Client (){   
    while(1){          
        struct sockaddr_in client_address;     // Ver si está bien crearlo acá, sino se pasa por argumento...
        socklen_t client_address_len = sizeof(client_address);
        int cs = accept(fd_socket_client, (struct sockaddr *) &client_address, (socklen_t *) &client_address_len);    
        if (cs == -1) {
            perror("Run_Client: accept");
            exit(1);
        }
        Client2M* clientnew = new Client2M(cs, client_address.sin_addr.s_addr, this);
        clients.push_back(clientnew);
        printf("Run_Client: Client Connected %s\n", inet_ntoa(client_address.sin_addr));
    }
}



static void Run_Server_Static(Master *S){
    S->Run_Server();
}

void Run_Server(){
    while(1){          
        struct sockaddr_in server_address;     
        socklen_t server_address_len = sizeof(server_address);
        int cs = accept(fd_socket_server, (struct sockaddr *) &server_address, (socklen_t *) &server_address_len);    
        if (cs == -1) {
            perror("Run_Server: accept");
            exit(1);
        }
        Server2M* servernew = new Server2M(cs, server_address, this);
        servers.push_back(servernew);
        printf("Run_Server: Server Connected %s\n", inet_ntoa(server_address.sin_addr));
    }
}

bool addJob(Msg &msg){
    Job job;
    getSubmitJob(&msg, job.working_dir, job.program, job.args, &job.interactive, &job.interactive_address, &job.interactive_nro_port);
    job.estado = ESTADO::QUEUED; 
    job.jobid = jobs_id++;
    jobs.put(job);
    printf("Jobid: %d\n", job.jobid);
    setJobStatus(&msg, job.jobid, job.estado);
    return true; 
}

bool deleteJob(Msg &msg){
    uint32_t job_id; 
    getDeleteJob(&msg, &job_id);
    ESTADO st = ESTADO::DELETED;
    if( ! jobs.remove(job_id) ) {
        printf("Job %hhu no encontrado\n", job_id);
        st = ESTADO::ERROR;
    }
    setJobStatus(&msg, job_id, st);
    return true;
}

void stateJob(Msg &msg){
    uint32_t job_id;
    getQueryJob(&msg, &job_id);
    ESTADO st = jobs.getStatus(job_id);
    setJobStatus(&msg, job_id, st);
}

Job getJob(){
    return jobs.getJobForRunning();
}

void setJobState(uint32_t jobid, ESTADO st) {
    jobs.setJobState(jobid, st);
}

};
