#pragma once
#include "Game_object.h"
#include "Blocks.h"
#include <list>

class Tank : public Game_object
{
private:
static unsigned short int Tank_id;
int sprite_size;
int animation_utility;
float x_speed,y_speed;
bool isStack;
bool isShooting;
int hp;
    bool collide(std::list<Tank*>&,std::list<Blocks*>& blocks);
    bool check_collision(float,float,float,float);
public:
    static std::list<Tank*>* tanks_for_id;


    static void set_Tank_id(unsigned short int id){Tank_id=id;};
    float get_bullet_x();
    float get_bullet_y();
    void shoot(){isShooting=true;};
    virtual void hit(bool isEnemy){hp--;};
    virtual bool if_enemy(){return true;};
    bool ifStack(){return isStack;};
    virtual int update(RenderWindow& app,bool remote,std::list<Tank*>&,std::list<Blocks*>& blocks);
    virtual void set_direction(char direction) override;
    int get_hp(){return hp;};
    int get_sprite_size(){return sprite_size;}
    Tank(float x,float y,const Texture& Texture);
};

