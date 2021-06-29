#pragma once
#include "Game_object.h"

class Blocks : public Game_object
{
private:
    static unsigned short int block_id;
    int durability;
    int type;           //0-can be destroyed,1-can't be destroyed,2-ignored by bullets,3-ignored by tanks and bullets
    int sprite_size;
    int timeout;
    int animation_time;
    int animation_util;

public:
    static void set_Block_id(int id){block_id = id;};
    bool update(RenderWindow& app);
    Blocks(float x,float y,const Texture& Texture,int type,int sprite_size);
    Blocks(float x,float y,const Texture& Texture,int type,int sprite_size,int animation_time);
    Blocks(float x,float y,const Texture& Texture,int type,int sprite_size,int animation_time,int timeout);
    int get_sprite_size(){return sprite_size;}
    int get_type(){return type;}
    int get_timeout(){return timeout;}
    void hit(){if(type==0)durability--;}
};
