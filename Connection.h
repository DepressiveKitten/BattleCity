#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#define _GNU_SOURCE
#include <fcntl.h>


using namespace std;
class Connection
{
private:
    int udp_socket;
    struct sockaddr *home_adress,*sendto_adress,*recieve_adress;
    bool isServer;
    list<char*> send_query,recieve_query;


    void set_length(char* buffer,int length);
    int get_length(char* buffer);
    
public:
    void send(char*);
    char* recieve();
    void set_address(const char* src,u_int16_t inPort,struct sockaddr **init_adress);
    void set_address(u_int32_t inAddress, u_int16_t inPort,struct sockaddr **init_adress);
    void set_send_adress(const char* src,u_int16_t inPort){set_address(src,inPort,&sendto_adress);};
    void listen();
    void sending();
    void add_to_query(char* buffer);
    char* get_from_query();
    struct sockaddr* get_recieve_adress(){return recieve_adress;};
    Connection(bool isServer,const char* strAdress, u_int16_t inPort);
    ~Connection(){close(udp_socket); };
};
