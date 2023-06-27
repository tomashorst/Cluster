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
#include "Class_Server.h"
#include "Class_Server2M.h"
using namespace std;

//argc: la cantidad de argumentos totales cuando se ejecuta el programa
//argv: cada uno de los argumentos, argv[0] es el nombre del programa
int nro_port;
hostent* master;

int main(int argc, char** argv) {
    int option;
    string ip_address;
    nro_port = 0;
    master = nullptr;
    while ((option = getopt(argc, argv, "p:i:")) != -1) {
        switch (option) {
            case 'p':
                nro_port = stoi(optarg);
                break;
            case 'i':
                ip_address = optarg;
                master = gethostbyname(ip_address.c_str());
                if (master == nullptr) {
                    cerr << "Failed to resolve host." << endl;
                    return 1;
                }
                break;
            default:
                cout << "Usage: ./Server -p <port> -i <ip_address>" << endl;
                return 1;
        }
    }
    if (nro_port == 0 || ip_address.empty()) {
        cout << "Usage: ./Server -p <port> -i <ip_address>" << endl;
        return 1;
    }

    Server server(nro_port, *(uint32_t*)master->h_addr);
    server.run();
    
    return 0;
}


// Ip del master: localhost o 127.0.0.1