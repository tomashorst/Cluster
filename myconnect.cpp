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
#include "myconnect.h"
using namespace std;

int myconnect(int nro_port, uint32_t IP){
    sockaddr_in master_address;
    //socket
    int fd_socket;
    fd_socket = socket(AF_INET, SOCK_STREAM, 0);                // creo el socket 
    if (fd_socket < 0) 
        perror("ERROR opening socket(myconnect)\n");
    //connect
    bzero((char *) &master_address, sizeof(master_address));    // pongo en 0 la estructura
    master_address.sin_family = AF_INET;                        // seteo la familia
    master_address.sin_addr.s_addr = IP;                        // copio la direccion del master
    //bcopy((char *)master->h_addr, (char *)&master_address.sin_addr.s_addr, master->h_length);   // copio la direccion del master
    master_address.sin_port = htons(nro_port);                  // seteo el puerto

    if (connect(fd_socket,(struct sockaddr *) &master_address,sizeof(master_address)) < 0)      // me conecto al master
        perror("ERROR connecting(myconnect)\n");
    return fd_socket;
}
//Faltará cerrar el socket