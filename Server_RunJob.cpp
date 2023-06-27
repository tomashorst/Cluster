#include "Class_Server.h"
#include "protocolo.h"
#include <wait.h>

void Server::RunJob(Msg msg, Job job_received){
    pid_t child;
    if( (child = fork()) == 0){     
        //Hijo
        setJobStatus(&msg, job_received.jobid, ESTADO::RUNNING);
        sendMsg(fd_socket, &msg);
        // const char* args1[] = {job_received.working_dir, NULL };
        // convierte el arreglo args2 de un arreglo de punteros a const char, a un arreglo de punteros a char. 
        const char* args2[] = {job_received.program, job_received.args, NULL };
        if(chdir(job_received.working_dir) == 0){
            if(execvp(args2[0], const_cast<char**>(args2)) == -1){
                setJobStatus(&msg, job_received.jobid, ESTADO::ERROR_EXEC);
                sendMsg(fd_socket, &msg);
            }
        }
        else{
            setJobStatus(&msg, job_received.jobid, ESTADO::ERROR_EXEC);
            sendMsg(fd_socket, &msg);
        }
    }
    else if(child < 0) {    
        //Error en el fork
        setJobStatus(&msg, job_received.jobid, ESTADO::ERROR_FORK);
        sendMsg(fd_socket, &msg);
    }
    else{
        int status;
        //Padre
        do {
            waitpid(child, &status, 0);
        } while (!WIFEXITED(status));
        if(WEXITSTATUS(status) == 0){
            setJobStatus(&msg, job_received.jobid, ESTADO::FINISHED);
            sendMsg(fd_socket, &msg);
        }
        else{
            setJobStatus(&msg, job_received.jobid, ESTADO::ERROR);    
            sendMsg(fd_socket, &msg);
        }
    }
}

