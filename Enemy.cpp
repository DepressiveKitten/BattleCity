#include "Enemy.h"


int Enemy::get_next_direction()
{
    if(this->get_direction()==0)
    {
        set_direction((char)(rand()%4+1));
    }
    if(ifStack())
    {
        if(rand()%(int)(GFRAMERATE/4)==0)
        {
            if(rand()%3==0)
            {
                return (get_direction()+2)%4;
            }
            return get_direction()%4+1;
        }
    }
    if(((int)gety()+1)%8<=2&&((int)getx()%8+1)<=2)
    {
        if(rand()%(GFRAMERATE*2)==0)
        {
            if(rand()%5==0)
            {
                return rand()%4+1;
            }
            else//умная функция
            {
                if(GBORDER-gety()>abs(GBORDER/2-getx()))
                {
                    return 3;
                }
                else
                {
                    if((GBORDER/2)-getx()>0)
                    {
                        return 4;
                    }
                    else
                    {
                        return 2;
                    }
                }
            }
        }
    }
    return 0;
}


int Enemy::update(RenderWindow& app,bool remote,std::list<Tank*>& tanks,std::list<Blocks*>& blocks)
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
        

        int temp = get_next_direction();
        if(temp!=0)
        {
            this->set_direction(temp);
        }
        if(shoot_cooldown<=0)
        {
            if(rand()%(int)(GFRAMERATE*1.5))
            {
                this->shoot();
                shoot_cooldown=GENEMYSHOTCOOLDOWN;
            }
        }
        shoot_cooldown--;
    }
    return Tank::update(app,remote,tanks,blocks);
}

Enemy::Enemy(float x,float y,const Texture& Texture):Tank(x,y,Texture)
{
    shoot_cooldown=5*GFRAMERATE;
    stun=0;
}

