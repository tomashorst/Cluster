#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/socket.h>
#include <thread>
#include "Class_Job.h"
#include "myconnect.h"
using namespace std;

class Master;   // Existe una clase Master


class Server2M{
private:
    int fd_socket;
    sockaddr_in server;
    thread thread_server;
    Master* master;

public:

Server2M(int fd_socket_, sockaddr_in server_address_, Master* master_){
    fd_socket = fd_socket_;
    server = server_address_;
    master = master_;
    thread_server = thread(&Server2M::Reading_server, this);

}

void Reading_server();

};