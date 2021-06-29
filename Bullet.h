#pragma once
#include "Game_object.h"
#include "Tank.h"
#include <list>
#include "Blocks.h"

class Bullet:public Game_object
{
private:
    static unsigned short int bullet_id;
    bool remote;
    int sprite_size;
    float x_speed,y_speed;
    bool isEnemy;
    bool collide(std::list<Tank*>&,std::list<Blocks*>& blocks);
    bool check_collision(float,float,float,float);
public:
    static void set_Bullet_id(int id){bullet_id = id;};
    bool update(RenderWindow& app,std::list<Tank*>&,std::list<Blocks*>& blocks);
    Bullet(float x,float y,const Texture& Texture,char direction,bool isEnemy, bool isRemote);
};
