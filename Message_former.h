#pragma once
#include "Connection.h"

struct input_message
{
    char direction;
    char isShooting;
};

struct common_info
{
    int id;
    char direction;
    int x,y;
    char frame;
};

using namespace std;

class Message_former
{
private:
    unsigned char frame;
    Connection *con;
    input_message input;
    list<int> idList;
    list<common_info> objectList;


    int get_length(char* buffer);
    void set_length(char* buffer,int length);
    int decode_int(char* buffer);
    void encode_int(char* buffer,int length);
    
public:
    void inc_frame(){frame++;if(frame<1)frame=1;}
    input_message get_input_message(){return input;}
    void set_input_message(char direction,bool isShouting){input.direction=direction;input.isShooting=isShouting;}

    void add_id(int id){idList.push_back(id);};
    int get_id();

    void add_object(common_info obj){objectList.push_back(obj);};
    common_info get_object();

    void send();
    void recieve();
    void send_input();

    Message_former(Connection *con);
};

