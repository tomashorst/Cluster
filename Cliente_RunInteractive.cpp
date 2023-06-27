#include <iostream>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "Class_Client.h"
#include "protocolo.h"
void Client::RunInteractive(){
    thread_client = thread(&Client::RunAccept, this);
}

void Client::RunAccept(){
    int cs = accept(interactive_fd_socket, (struct sockaddr *) &server2client_address, (socklen_t *) &len_server2client_address);    
    if (cs == -1) {
        perror("Error en el accept del interactive\n");
        exit(1);
    }
    printf("Server connected\n");
    //Un nuevo thread
    thread th = thread(&Client::STDreader, this, cs);
    while(1){
        char buffer[1024];
        int bytes = recv(cs, buffer, sizeof(buffer), 0);
        if (bytes == -1) {
            perror("Error en el recv del RunAccept\n");
            exit(1);
        }
        if(bytes == 0){
            printf("Se desconectó el server\n");
            break;
        }
        buffer[bytes] = '\0';
        write(1, buffer, strlen(buffer));
    }
    th.join();
}

void Client::STDreader(int cs){
    while(1){
        char buffer[1024];
        int bytes = read(0, buffer, sizeof(buffer));
        if (bytes == -1) {
            perror("Error en el read del STDreader\n");
            exit(1);
        }
        if(bytes == 0){
            printf("Se desconectó el cliente\n");
            close(cs);
            break;
        }
        write(cs, buffer, bytes);
    }
}

void Client::JoinInteractive(){
    thread_client.join();
}
