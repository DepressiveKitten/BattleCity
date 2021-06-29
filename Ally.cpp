#include "Ally.h"


int Ally::update(RenderWindow& app,bool remote,std::list<Tank*>& tanks,std::list<Blocks*>& blocks)
{
    if(!remote)
    {
        if(stun>0)
        {
            stun--;
            if(this->get_hp()<=0)
            {
                this->show(app);
                return 0;
            }
            this->show(app);
            return 1;
        }
        //if(controllable)
        //{
            set_direction(check_direction());
            if(check_shooting())
            {
                this->shoot();
            }
        //}
    }
    return Tank::update(app,remote,tanks,blocks);
}

int Ally::check_direction()
{
    if(!controllable)//temporary
    {
        return forced_direction;
    }
    if(controllable)
    {
        if(Keyboard::isKeyPressed(Keyboard::Key::W))
        {
            return 1;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::A))
        {
            return 2;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::S))
        {
            return 3;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::D))
        {
            return 4;
        }
    }
    return 0;
}

bool Ally::check_shooting()
{
    if(controllable)
    {
        if(Keyboard::isKeyPressed(Keyboard::Key::Space))
        {
            if(shoot_cooldown<=0)
            {
                shoot_cooldown=GFRAMERATE/2.5;
                return true;
            }
            if(shoot_cooldown>1)
            {
                shoot_cooldown--;
            }
        }
        else
        {
            if(shoot_cooldown<=0)
                shoot_cooldown=0;
            shoot_cooldown--;
        }
    }
    if(!controllable)
    {
        if(forced_shooting)
        {
            if(shoot_cooldown<=0)
            {
                shoot_cooldown=GFRAMERATE/2.5;
                return true;
            }
            if(shoot_cooldown>1)
            {
                shoot_cooldown--;
            }
        }
        else
        {
            if(shoot_cooldown<=0)
                shoot_cooldown=0;
            shoot_cooldown--;
        }
    }
    return false;
}

Ally::Ally(float x,float y,const Texture& Texture,bool controllable):Tank(x,y,Texture)
{
    forced_shooting=false;
    forced_direction=1;
    shoot_cooldown=0;
    stun=0;
    this->controllable=controllable;
}