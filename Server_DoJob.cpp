#include "Class_Server.h"
#include "protocolo.h"
#include <wait.h>

void Server::DoJob(){
    while(1){

        Msg msg;
        setGetJob(&msg);
        int check1 = sendMsg(fd_socket, &msg); 
        if (check1 <= 0){
            break;
        }else{
            int check2 = recvMsg(fd_socket, &msg); 
            if (check2 <= 0){
                break;
            }else{
                if(msg.getType() == M2S_RUN_JOB){
                    Job job_received;
                    getRunJob(&msg, &job_received.jobid, job_received.working_dir, job_received.program, job_received.args, 
                    &job_received.interactive, &job_received.interactive_address, &job_received.interactive_nro_port);
                    interactive_client = job_received.interactive_address;
                    if(job_received.interactive == 0)
                        RunJob(msg, job_received);
                    else
                        Interactive(msg, job_received);
                }   
            }
        }
    }   
}

