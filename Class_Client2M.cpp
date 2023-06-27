#include "Class_Client2M.h"
#include "Class_Master.h"
#include "protocolo.h"
#include "Class_Job.h"
#include <iostream>

void Client2M::Reading_client(){

    Msg msg;
    while( 1 ) {
        if( recvMsg(fd_socket, &msg) <= 0 )
            break;
        //Chequeos de mensaje:
        switch (msg.getType()){
        case CLIENT_SUBMIT_JOB:
            master->addJob(msg);
            msg.payload.submitJob.interactive_address = client;
            if( sendMsg(msg) <= 0 ){
                goto out;
            }
            break;
        case CLIENT_DELETE_JOB:
            master->deleteJob(msg);
            if( sendMsg(msg) <= 0 )
                goto out;
            break;
        case CLIENT_QUERY_STATUS:
            master->stateJob(msg);
            if( sendMsg(msg) <= 0 )
                goto out;
            break;
        default:
            goto out;
        }
    }
    out:
    thread_client.detach(); 
}

int Client2M::sendMsg(const Msg &msg)
{
    std::unique_lock<std::mutex> lck{send_mtx};

    return ::sendMsg(fd_socket, &msg);
}
