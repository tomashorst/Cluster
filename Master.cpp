//Proceso que instancia el Class_Master

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
#include "Class_Master.h"
using namespace std;
#define PORT_CLIENTS 8080
#define PORT_SERVERS 9090

//El constructor del master recibe dos argumentos: los dos puertos; uno para clientes y otro para servidores
int main(){
    Master master(PORT_CLIENTS, PORT_SERVERS);
    //El master se queda esperando a que se conecten clientes, porque no larga el thread todavía y se va a trabar allí
    return 0;
}
