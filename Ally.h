#pragma once
#include "Tank.h"

class Ally: public Tank
{
private:
	int stun;
    int shoot_cooldown;
    bool controllable;
    int forced_direction;
    bool forced_shooting;
    int check_direction();
    bool check_shooting();
public:
    void set_forced_direction(int forced_direction){this->forced_direction=forced_direction;};
    void set_control(bool forced_shooting){this->forced_shooting=forced_shooting;};
    virtual bool if_enemy()override{return false;};
    virtual void hit(bool isEnemy)override {if(isEnemy){Tank::hit(isEnemy);}else{stun=2*GFRAMERATE;}};
    virtual int update(RenderWindow& app,bool remote,std::list<Tank*>&,std::list<Blocks*>& blocks) override;
    Ally(float x,float y,const Texture& Texture,bool controllable);

};
