#include "Message_former.h"

int Message_former::get_length(char* buffer)
{
    return (unsigned char)buffer[0]*256+(unsigned char)buffer[1];
}

void Message_former::set_length(char* buffer,int length)
{
    unsigned char temp=length/256;
    buffer[0]=temp;
    temp=length%256;
    buffer[1]=temp;
    return;
}


int Message_former::decode_int(char* buffer)
{
    return (unsigned char)buffer[0]*256+(unsigned char)buffer[1];
}

void Message_former::encode_int(char* buffer,int length)
{
    unsigned char temp=length/256;
    buffer[0]=temp;
    temp=length%256;
    buffer[1]=temp;
    return;
}


int Message_former::get_id()
{
    if(idList.size()==0)
    {
        return NULL;
    }
    int id=*(idList.begin());
    idList.pop_front();
    return id;
}

common_info Message_former::get_object()
{
    if(objectList.size()==0)
    {
        common_info object;
        object.id=0;
        return object;
    }
    common_info object=*(objectList.begin());
    objectList.pop_front();
    return object;
}

void Message_former::send()
{
    //printf("%ld \n",objectList.size());
    //формируем сообщение обо всех обьектах требующих изменения
    char* buffer=(char*)malloc(1000*sizeof(char));

    buffer[2]=0;
    buffer[3]=frame;
    int b=4;
    int limit=0;
    for(auto i=objectList.begin();i!=objectList.end();)
    {
        if((*i).id>0&&(*i).id<100)
        {
            limit++;


            encode_int(buffer+b,(*i).x);
            b+=2;
            encode_int(buffer+b,(*i).y);
            b+=2;
            unsigned char temp=(*i).direction*30+(*i).id;
            buffer[b]=temp;
            b++;

            i=objectList.erase(i);
            continue;
        }
        i++;
    }
    set_length(buffer,4+limit*5);
    con->send(buffer);
    free(buffer);

    //формируем сообщение обо всех новых/удаляемых обьектах   пули|блоки|обьекты на удаление


    buffer=(char*)malloc(1000*sizeof(char));
    buffer[2]=frame;
    b=3;
    limit=0;
    for(auto i=objectList.begin();i!=objectList.end();)
    {
        if((*i).id<1000)
        {
            limit++;
            encode_int(buffer+b,(*i).x);
            b+=2;
            encode_int(buffer+b,(*i).y);
            b+=2;
            unsigned char temp=(*i).id;
            buffer[b]=temp;
            b++;
            temp=(*i).direction;
            buffer[b]=temp;
            b++;
            i=objectList.erase(i);
            continue;
        }
    }
    if(limit>0||idList.size()>0)
    {
        if(idList.size()==0)
        {
            set_length(buffer,4+limit*6);
            con->add_to_query(buffer);
        }
        else
        {
            set_length(buffer,4+limit*6+idList.size()*2+2);
            encode_int(buffer+b,65535);
            b+=2;
            for(auto i=idList.begin();i!=idList.end();)
            {
                encode_int(buffer+b,*i);
                b+=2;
                i=idList.erase(i);
                continue;
            }
            con->add_to_query(buffer);
        }
    }
    else
    {
        free(buffer);
    }

    con->sending();
}

void Message_former::recieve()
{
    con->listen();
    char* buffer;
    do
    {
        buffer=con->get_from_query();
        if(buffer==nullptr)
        {
            return;
        }
        if(buffer[2]==0)//принимаем сообщение обо всех обьектах требующих изменения
        {
            if(buffer[3]==0&&get_length(buffer)==6)
            {
                input.isShooting=buffer[4];
                input.direction=buffer[5];
            }
            else
            {
                char message_frame=buffer[3];
                for(int b=4;b<get_length(buffer);)
                {
                    common_info new_object;


                    new_object.x=decode_int(buffer+b);
                    b+=2;
                    new_object.y=decode_int(buffer+b);
                    b+=2;
                    unsigned char temp=buffer[b];
                    new_object.direction=temp/30;
                    new_object.id=temp%30;
                    b++;


                    bool flag=false;
                    for(auto i=objectList.begin();i!=objectList.end();i++)
                    {
                        if(new_object.id==(*i).id)
                        {
                            flag=true;
                            if(new_object.frame>(*i).frame||(*i).frame-new_object.frame>200)
                            {
                                (*i)=new_object;
                            }
                        }
                    }
                    if(flag==false)
                    {
                        objectList.push_back(new_object);
                    }
                }
            }
        }
        else        //принимаем сообщение обо всех новых/удаляемых обьектах   пули|блоки|обьекты на удаление
        {
            char message_frame=buffer[2];
            int b=3;
            for(b=3;b<get_length(buffer);)
            {
                if(decode_int(buffer+b)==65535)
                {
                    b+=2;
                    break;
                }
                common_info new_object;


                new_object.x=decode_int(buffer+b);
                b+=2;
                new_object.y=decode_int(buffer+b);
                b+=2;
                unsigned char temp=buffer[b];
                new_object.id=temp;
                b++;
                temp=buffer[b];
                new_object.direction=temp;
                b++;


                objectList.push_back(new_object);
            }
            for(;b<get_length(buffer);)
            {
                int temp=decode_int(buffer+b);
                bool flag=false;
                for(auto i=idList.begin();i!=idList.end();i++)
                {
                    if(temp==*i)
                    {
                        flag=true;
                        break;
                    }
                }
                if(flag==false)
                {
                    idList.push_back(temp);
                }
                b+=2;
            }
        }
        free(buffer);
        //printf("%ld\n",idList.size());
    }while(buffer!=nullptr);
}

void Message_former::send_input()
{
    char* buffer=(char*)malloc(1000*sizeof(char));
    set_length(buffer,6);
    buffer[2]=0;
    buffer[3]=0;
    buffer[4]=input.isShooting;
    buffer[5]=input.direction;
    con->send(buffer);
    free(buffer);
}



Message_former::Message_former(Connection *con)
{
    this->con=con;
    frame=1;
    input.isShooting=0;
    input.direction=0;
}