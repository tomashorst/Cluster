#include "Class_Server2M.h"
#include "Class_Master.h"
#include "protocolo.h"
#include <wait.h>

void Server2M::Reading_server(){
    while(1){
        Msg msg;
        recvMsg(fd_socket, &msg);
        //Chequeos de mensaje:
        if(msg.getType() == S2M_GET_JOB){
            Job job_received = master->getJob();
            setRunJob(&msg, job_received.jobid, job_received.working_dir, job_received.program, job_received.args
            , job_received.interactive, job_received.interactive_address, job_received.interactive_nro_port);
            sendMsg(fd_socket, &msg);
        }
        else if(msg.getType() == JOB_STATUS) {
            uint32_t jobid;       
            uint8_t status;
            getJobStatus(&msg, &jobid, &status);
            master->setJobState(jobid, (ESTADO) status);
        }        
    }
    thread_server.detach(); 

}