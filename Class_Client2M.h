#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include "Class_Job.h"
#include "myconnect.h"
#include "protocolo.h"
using namespace std;

class Master;   // Existe una clase Master

class Client2M{
private:
    int fd_socket;
    uint32_t client;
    thread thread_client;
    Master* master;
    std::mutex send_mtx;

public:

Client2M(int fd_socket_, uint32_t client_address_, Master* master_){
    fd_socket = fd_socket_;
    client = client_address_;
    master = master_;
    thread_client = thread(&Client2M::Reading_client, this); 
}

void Reading_client();

int sendMsg(const Msg &msg);

};
