#pragma once
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
using namespace std;

//Función que se llamará 

int myconnect(int nro_port, uint32_t IP);
//Faltará cerrar el socket