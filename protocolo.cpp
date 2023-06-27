#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "protocolo.h"

void printType(const Msg *msg)
{
    printf("Type: %x\n", msg->hdr.type);
}

// Envia un Msg, el Msg está en un buffer lineal,
// reintento hasta mandarlo entero
// retorna 0 si cerró el socket, -1 en caso de error
// retorna 1 si está ok
int sendMsg(int sockfd, const Msg *msg)
{
    size_t toSend = ntohs(msg->hdr.size8);
    ssize_t sent;
    uint8_t *ptr = (uint8_t *) msg;

    while( toSend ) {
        sent = send(sockfd, ptr, toSend, 0);        //lo mismo que la write. Me garatinza de mandar una cantidad de bytes especificos
        if( (sent == -1 && errno != EINTR) || sent == 0 )
            return sent;
        toSend -= sent;
        ptr += sent;
    }
    return 1;
}

// Recibe un Msg, no sabemos el tamaño, hay que recibir
// primero el header, y después el payload cuando sepamos el tamaño a partir del header
// reintento hasta recibirlo entero
// retorna 0 si cerró el socket, -1 en caso de error
// retorna 1 si está ok
int recvMsg(int sockfd, Msg *msg)
{
    size_t toRecv = sizeof(Header);
    ssize_t recvd;
    uint8_t *ptr = (uint8_t *) &msg->hdr;
    int headerRecvd = 0;

    while( toRecv ) {   
        recvd = recv(sockfd, ptr, toRecv, 0); //lo mismo que la read. Me garatinza de leer una cantidad de bytes especificos
        if( (recvd == -1 && errno != EINTR) || recvd == 0 )
            return recvd;
        toRecv -= recvd;
        ptr += recvd;
        if( toRecv == 0 && headerRecvd == 0) {
            headerRecvd = 1;
            ptr = (uint8_t *) &msg->payload;
            toRecv = ntohs(msg->hdr.size8) - sizeof(Header);
        }
    }
    return 1;
}
