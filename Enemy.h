#pragma once
#include "Tank.h"


class Enemy:public Tank
{
private:
    int stun;
    int shoot_cooldown;
    int get_next_direction();
public:
    virtual bool if_enemy()override{return true;};
    virtual void hit(bool isEnemy)override {if(!isEnemy){Tank::hit(isEnemy);}else{stun=2*GFRAMERATE;}};
    virtual int update(RenderWindow& app,bool remote,std::list<Tank*>&,std::list<Blocks*>& blocks) override;
    Enemy(float x,float y,const Texture& Texture);
};
