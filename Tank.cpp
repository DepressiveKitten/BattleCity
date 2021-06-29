#pragma once
#include "Tank.h"

void Tank::set_direction(char direction)
{
    if(direction==0)
    {
        x_speed=0;
        y_speed=0;
    }
    if(direction==1)
    {
        x_speed=0;
        y_speed=-GTANKSPEED;
        Game_object::set_direction(direction);
        this->Next_Sprite();
    }
    if(direction==2)
    {
        x_speed=-GTANKSPEED;
        y_speed=0;
        Game_object::set_direction(direction);
        this->Next_Sprite();
    }
    if(direction==3)
    {
        x_speed=0;
        y_speed=GTANKSPEED;
        Game_object::set_direction(direction);
        this->Next_Sprite();
    }
    if(direction==4)
    {
        x_speed=GTANKSPEED;
        y_speed=0;
        Game_object::set_direction(direction);
        this->Next_Sprite();
    }

}




bool Tank::collide(std::list<Tank*>& tanks,std::list<Blocks*>& blocks)
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
        if(this!=*i)
        {
            if(check_collision(
            (*i)->getx(),
            (*i)->gety(),
            (*i)->getx()+(*i)->get_sprite_size(),
            (*i)->gety()+(*i)->get_sprite_size()
            ))
            {
                return true;
            }
        }
    }
    for(auto i = blocks.begin(); i != blocks.end(); i++)
    {
        if((*i)->get_type()<3)
        {
            if(check_collision((*i)->getx()+GSCALE*2,(*i)->gety()+GSCALE*2,(*i)->getx()+(*i)->get_sprite_size()-GSCALE,(*i)->gety()+(*i)->get_sprite_size()-GSCALE))
            {
                return true;
            }
        }
    }

    return false;
}

bool Tank::check_collision(float x_left,float y_left,float x_right, float y_right)
{
    if ( x_left >= this->getx()+sprite_size+x_speed ||
     x_right <= this->getx() + x_speed ||
      y_left >= this->gety()+sprite_size + y_speed ||
      y_right <= this->gety() + y_speed ) {
        return false;
    } 
    return true;
}

















int Tank::update(RenderWindow& app,bool remote,std::list<Tank*>& tanks,std::list<Blocks*>& blocks)
{
    if(!remote)
    {
        if(hp<=0)
        {
            this->show(app);
            return 0;
        }
        if(x_speed!=0||y_speed||0)
            animation_utility++;
        if(animation_utility>=GTANKANIM)
        {
            this->Next_Sprite();
            animation_utility=0;
        }
        isStack = collide(tanks,blocks);
        if(!isStack)
        {
            this->move(x_speed,y_speed);
        }

        if(isShooting)
        {
            this->show(app);
            isShooting=false;
            return 2;
        }
    }

    this->show(app);
    return 1;
}

float Tank::get_bullet_y()
{
    float y;
    if(this->get_direction()==1)
    {
        y=this->gety()-y_speed-6*GSCALE;
    }
    if(this->get_direction()==2)
    {
        y=this->gety()+sprite_size/2-2*GSCALE;
    }
        if(this->get_direction()==3)
    {
        y=this->gety()+sprite_size+y_speed+6*GSCALE;
    }
    if(this->get_direction()==4)
    {
        y=this->gety()+sprite_size/2-2*GSCALE;
    }
    return y;
}
float Tank::get_bullet_x()
{
    float x;
    if(this->get_direction()==1)
    {
        x=this->getx()+sprite_size/2-2*GSCALE;
    }
    if(this->get_direction()==2)
    {
        x=this->getx()-x_speed-6*GSCALE;
    }
        if(this->get_direction()==3)
    {
        x=this->getx()+sprite_size/2-2*GSCALE;
    }
    if(this->get_direction()==4)
    {
        x=this->getx()+sprite_size+x_speed+6*GSCALE;
    }
    return x;
}







Tank::Tank(float x,float y,const Texture& Texture):Game_object(x,y,16,Texture,2,Tank_id)
{
    for(unsigned short int j=3;j<=25;j++)
    {
        bool flag=false;
        for(auto i = (*tanks_for_id).begin();i!=(*tanks_for_id).end();i++)
        {
            if((*i)->get_id()==j||Tank_id==j)
            {
                flag=true;
                break;
            }
        }
        if(flag==false)
        {
            Tank_id=j;
            break;
        }
    }
    isShooting=false;
    hp=1;
    sprite_size=16*GSCALE;
    animation_utility=0;
    x_speed=0;
    x_speed=0;
    sprite_size = 16*GSCALE;
}