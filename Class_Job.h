#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define PATH 256                    // Valor a cambiar ṕosiblemente
#define ARGS 256                    // Valor a cambiar posiblemente

enum class ESTADO : uint8_t {
    QUEUED = 1,
	RUNNING = 2,
    FINISHED = 3,   
    OK = 4,
	ERROR = 5,                       
    ERROR_FORK = 6,
    ERROR_EXEC = 7,
    DELETED = 8,
    NOT_FOUND = 9
};

class Job {
public:
	ESTADO estado;				    // Estado del job
   	uint32_t jobid;                 // Se identifica por su jobid
   	char working_dir[PATH];         // Directorio de trabajo
   	char program[PATH];             // Programa a ejecutar
   	char args[ARGS];                // Argumentos del programa 
    uint8_t interactive;            // 0 si no es interactivo, 1 si lo es
    uint32_t interactive_address;   // Host(IP) del cliente interactivo
    uint16_t interactive_nro_port;  // Puerto del cliente interactivo

void printfEstado(ESTADO estado) {
    switch (estado) {
        case ESTADO::QUEUED:
            printf("QUEUED");
			return;
        case ESTADO::RUNNING:
            printf("RUNNING");
			return;
        case ESTADO::FINISHED:
            printf("FINISHED");
			return;
        case ESTADO::OK:
            printf("OK");
			return;
        case ESTADO::ERROR:
            printf("ERROR");
			return;
        case ESTADO::ERROR_FORK:
            printf("ERROR_FORK");
			return;
        case ESTADO::ERROR_EXEC:
            printf("ERROR_EXEC");
			return;
        case ESTADO::DELETED:
            printf("DELETED");
			return;
        case ESTADO::NOT_FOUND:
            printf("NOT_FOUND");
			return;
        default:
            return;
    }
}

};