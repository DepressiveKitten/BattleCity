#include "Blocks.h"

bool Blocks::update(RenderWindow& app)
{
    if(durability<=0)
    {
        return false;
    }
    if(timeout>0){timeout--;}
    if(timeout==0)
    {
        return false;
    }
    if(animation_time!=0)
    {
        animation_util++;
        if(animation_util==animation_time)
        {
            animation_util=0;
            this->Next_Sprite();
        }
    }
    this->show(app);
    return true;
}

Blocks::Blocks(float x,float y,const Texture& Texture,int type,int sprite_size):Game_object(x,y,sprite_size,Texture,1,block_id) 
{
    block_id++;
    if(block_id>=30000)
    {
        block_id=2000;
    }
    animation_util=0;
    this->sprite_size=sprite_size*GSCALE;
    this->type=type;
    this->durability=1;
    this->timeout=-1;
    this->animation_time=0;
}
Blocks::Blocks(float x,float y,const Texture& Texture,int type,int sprite_size,int animation_time):Game_object(x,y,sprite_size,Texture,1,block_id) 
{
    block_id++;
    if(block_id>=30000)
    {
        block_id=2000;
    }
    animation_util=0;
    this->sprite_size=sprite_size*GSCALE;
    this->type=type;
    this->durability=1;
    this->timeout=-1;
    this->animation_time=animation_time;
}

Blocks::Blocks(float x,float y,const Texture& Texture,int type,int sprite_size,int animation_time,int timeout):Game_object(x,y,sprite_size,Texture,1,block_id) 
{
    block_id++;
    if(block_id>=30000)
    {
        block_id=2000;
    }
    animation_util=0;
    this->sprite_size=sprite_size*GSCALE;
    this->type=type;
    this->durability=1;
    this->timeout=timeout;
    this->animation_time=animation_time;
}