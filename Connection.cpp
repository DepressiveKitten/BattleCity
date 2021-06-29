#include "Connection.h"

int Connection::get_length(char* buffer)
{
    return (unsigned char)buffer[0]*256+(unsigned char)buffer[1];
}

void Connection::set_length(char* buffer,int length)
{
    unsigned char temp=length/256;
    buffer[0]=temp;
    temp=length%256;
    buffer[1]=temp;
    return;
}

void Connection::send(char* buffer)
{
    int temp = sendto(udp_socket,buffer,get_length(buffer),0,sendto_adress,sizeof(*sendto_adress));
    //printf("send %d %s\n",temp,buffer);
}

char* Connection::recieve()
{
    char *buffer=(char*)malloc(1000);
    socklen_t size=sizeof(*recieve_adress);
    int temp =recvfrom(udp_socket,buffer,1000,0,recieve_adress,&size);
    if(temp<0)
    {
        return nullptr;
    }
    //printf("recieve %d %s\n",temp,buffer);
    return buffer;
}

void Connection::listen()//если бервый байт не 0, то вернуть ack
{
    do
    {
        char* buffer=recieve();
        if(buffer==nullptr)
        {
            return;
        }
        if(buffer[2]!=0&&get_length(buffer)>4)
        {
            char* answer=(char*)malloc(sizeof(char)*4);
            set_length(answer,4);
            answer[2]=buffer[2];
            answer[3]=06;
            sendto_adress=recieve_adress;
            send(answer);
            free(answer);
        }
        if(get_length(buffer)>4)
        {
            recieve_query.push_back(buffer);
        }
        else
        {
            for(auto i = send_query.begin(); i != send_query.end();)
            {
                if((*i)[2]==buffer[2])
                {
                    free(buffer);
                    free(*i);
                    i=send_query.erase(i);
                    continue;
                }
                i++;
            }
        }
    }while(true);
}

void Connection::sending()
{
    //printf("\n\n%ld\n\n",send_query.size());
    for(auto i = send_query.begin(); i != send_query.end();)
    {
        char* buffer=*i;
        send(buffer);

        i++;
    }
}


char* Connection::get_from_query()
{
    if(recieve_query.size()==0)
        return nullptr;
    char* buffer=*(recieve_query.begin());
    recieve_query.pop_front();
    return buffer;
}

void Connection::add_to_query(char* buffer)
{
    if(send_query.size()>=15)
    {
        free(*(send_query.begin()));
        send_query.pop_front();
    }
    send_query.push_back(buffer);
}




Connection::Connection(bool isServer,const char* strAdress, u_int16_t inPort) //127.0.0.1
{
    this->isServer=isServer;
    udp_socket=socket(AF_INET,SOCK_DGRAM,0);


    int flags=fcntl(udp_socket,F_GETFL, 0); //
    flags=flags|O_NONBLOCK;                 //делаем сокет неблокируемым
    fcntl(udp_socket, F_SETFL, flags);      //


    recieve_adress=(sockaddr *)malloc(sizeof(sockaddr));
    set_address(strAdress,inPort,&home_adress);
    int temp = bind(udp_socket,home_adress,sizeof(*home_adress));
    printf("%d\n",temp);
}

void Connection::set_address(const char* src,u_int16_t inPort,struct sockaddr **init_adress)
{
    sockaddr_in *sockaddr_in_mask = (sockaddr_in *)malloc(sizeof(sockaddr_in));
    sockaddr_in_mask->sin_family = AF_INET;
    sockaddr_in_mask->sin_port = htons(inPort);
    inet_pton(AF_INET,src, &sockaddr_in_mask->sin_addr);
    *init_adress =(struct sockaddr*)sockaddr_in_mask;
}
void Connection::set_address(u_int32_t inAddress, u_int16_t inPort,struct sockaddr **init_adress)
{
    in_addr in_addr_mask;
    in_addr_mask.s_addr=htonl(inAddress);
    sockaddr_in *sockaddr_in_mask = (sockaddr_in *)malloc(sizeof(sockaddr_in));
    memset(sockaddr_in_mask->sin_zero, 0, sizeof(sockaddr_in_mask->sin_zero));
    sockaddr_in_mask->sin_family=AF_INET;
    sockaddr_in_mask->sin_addr=in_addr_mask;
    sockaddr_in_mask->sin_port=htons(inPort);
    *init_adress =(struct sockaddr*)sockaddr_in_mask;
}