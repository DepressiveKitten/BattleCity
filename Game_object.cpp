#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "Game_object.h"

using namespace sf;

    //Game_object---------------------------------------------------------------------
	//функции
	void Game_object::show(RenderWindow &app)                                  //отрисовка
	{
		app.draw(image);
	}
	

	void Game_object::Next_Sprite()                                            //переход на следующий спрайт
	{
		if(direction==0)
		{
			if ((cur_frame + 1) * texture_wight >= image.getTexture()->getSize().x)//проверка, является ли текщий кусок текстуры последним
				cur_frame = 0;
			else
				cur_frame++;
			image.setTextureRect(IntRect(texture_wight*cur_frame, 0, texture_wight, image.getTexture()->getSize().y));
		}
		else
		{
			if(cur_frame>animation_len-1)
				cur_frame=0;
			image.setTextureRect(IntRect(texture_wight*(((direction-1)*animation_len)+cur_frame), 0, texture_wight, image.getTexture()->getSize().y));
			cur_frame++;
		}
	}
	void Game_object::move(float x, float y)
	{
		pos_x += x;
		pos_y += y;
		image.setPosition(pos_x, pos_y);
	}
	//конструктор
	Game_object::Game_object(float x, float y, int texture_wight, const Texture &texture,int animation_len)
	{
		this->id=0;
		this->animation_len=animation_len;
		direction=0;
		pos_x = x;
		pos_y = y;
        cur_frame = 0;
		this->texture_wight = texture_wight;
		image.setTexture(texture);
		image.setTextureRect(IntRect(0,0,texture_wight, image.getTexture()->getSize().y));
		image.setPosition(x, y);
		image.setScale(GSCALE,GSCALE);
	}          

	Game_object::Game_object(float x, float y, int texture_wight, const Texture &texture,int animation_len,int id)          
	{
		this->id=id;
		this->animation_len=animation_len;
		direction=0;
		pos_x = x;
		pos_y = y;
        cur_frame = 0;
		this->texture_wight = texture_wight;
		image.setTexture(texture);
		image.setTextureRect(IntRect(0,0,texture_wight, image.getTexture()->getSize().y));
		image.setPosition(x, y);
		image.setScale(GSCALE,GSCALE);
	}
