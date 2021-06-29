#include "Bullet.h"


bool Bullet::update(RenderWindow& app,std::list<Tank*>& tanks,std::list<Blocks*>& blocks)
{
    if(!remote)
    {
        if(collide(tanks,blocks))
        {
            x_speed=0;
            y_speed=0;
            return false;
        }
    }
    this->move(x_speed,y_speed);
    this->show(app);
    return true;
}

bool Bullet::collide(std::list<Tank*>& tanks,std::list<Blocks*>& blocks)
{
    int x=this->getx()+x_speed;
    int y=this->gety()+y_speed;

    if(x<0||x+sprite_size>GBORDER)
    {
        return true;
    }
    if(y<0||y+sprite_size>GBORDER)
    {
        return true;
    }

    for(auto i = tanks.begin(); i != tanks.end(); i++)
    {
        if(check_collision((*i)->getx(),(*i)->gety(),(*i)->getx()+(*i)->get_sprite_size(),(*i)->gety()+(*i)->get_sprite_size()))
        {
            (*i)->hit(isEnemy);
            return true;
        }
    }
    for(auto i = blocks.begin(); i != blocks.end(); i++)
    {
        if((*i)->get_type()<2)
        {
            if(check_collision((*i)->getx(),(*i)->gety(),(*i)->getx()+(*i)->get_sprite_size(),(*i)->gety()+(*i)->get_sprite_size()))
            {
                (*i)->hit();
                return true;
            }
        }
    }

    return false;
}

bool Bullet::check_collision(float x_left,float y_left,float x_right, float y_right)
{
    if ( x_left > this->getx()+sprite_size || x_right < this->getx() || y_left > this->gety()+sprite_size ||y_right < this->gety() ) {
        return false;
    } 
    return true;
}

Bullet::Bullet(float x,float y,const Texture& Texture,char direction,bool isEnemy,bool isRemote):Game_object(x,y,4,Texture,1,bullet_id)
{
    remote=isRemote;
    bullet_id++;
    if(bullet_id>255)
    {
        bullet_id=100;
    }
    sprite_size=4*GSCALE;
    this->isEnemy=isEnemy;
    if(direction==1)
    {
        x_speed=0;
        y_speed=-GBULLETSPEED;
        this->set_direction(direction);
        this->Next_Sprite();
    }
    if(direction==2)
    {
        x_speed=-GBULLETSPEED;
        y_speed=0;
        this->set_direction(direction);
        this->Next_Sprite();
    }
    if(direction==3)
    {
        x_speed=0;
        y_speed=GBULLETSPEED;
        this->set_direction(direction);
        this->Next_Sprite();
    }
    if(direction==4)
    {
        x_speed=GBULLETSPEED;
        y_speed=0;
        this->set_direction(direction);
        this->Next_Sprite();
    }
}