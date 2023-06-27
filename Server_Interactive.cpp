#include <iostream>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/select.h>
#include "Class_Server.h"
#include "protocolo.h"
#define BUFFER_SIZE 1064

void Server::Interactive(Msg msg, Job job){
    // stdin, stdout, y stderr se redirigen al Cliente Interactivo.
    // antes de hacer el exec, duplicarlo el stdin, stdout y stderr a ese socket
    // En el server ejecuto: un shell ./bin/bash o ./bin/sh

    //Conectarse al host y port que vino en el msj,
    //redirigir los stdin y sdout con (dup) a ese socket
    //y ahí hacer fork, luego el exec
    //y el padre se queda esperando que el hijo termine
    //y cuando termina, el padre le manda al cliente el resultado
    //y el cliente lo imprime por pantalla
    int interactive_nro_port = job.interactive_nro_port;
    interactive_fd_socket = myconnect(interactive_nro_port, interactive_client);
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Error: al hacer fork en Server_Interactive.cpp\n" << endl;
        return;
    }
    if (pid == 0) {// hijo
        dup2(interactive_fd_socket, STDIN_FILENO);
        dup2(interactive_fd_socket, STDOUT_FILENO);
        dup2(interactive_fd_socket, STDERR_FILENO);
        const char* args[] = { "/bin/sh", "-i",NULL };   //Ejecutar algo interactivo
        if(execvp(args[0], const_cast<char**>(args)) == -1){
            setJobStatus(&msg, job.jobid, ESTADO::ERROR_EXEC);
            sendMsg(fd_socket, &msg);
        }
    }
    // padre
    int status;
    waitpid(pid, &status, 0);
    if(status == 0){
            setJobStatus(&msg, job.jobid, ESTADO::FINISHED);
            sendMsg(fd_socket, &msg);
    }
    else{
        setJobStatus(&msg, job.jobid, ESTADO::ERROR_EXEC);
        sendMsg(fd_socket, &msg);
        }

}



