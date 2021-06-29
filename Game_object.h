#pragma once
#include <SFML/Graphics.hpp>
#include "globals.h"
using namespace sf;

class Game_object
{
private:
	unsigned short int id;
	int cur_frame, texture_wight; //информация о текстуре обьекта
	float pos_x, pos_y;           //позиция обьекта
	char direction;
	int animation_len;
	Sprite image;
public:
	//----------функции
	void show(RenderWindow& app); //отрисовывает обьект
	virtual int update() { return 0; }   //функция вызываемая каждый фрейм
	void Next_Sprite();           //переход на следующую текстуру
	void move(float x,float y);   //перемещение обьекта
	void fading(float visibility)       //задане прозрачности
	{
		image.setColor(Color(255, 255, 255, (int)((float)255 * visibility)));
	}
	//get-теры позиции
	virtual void set_direction(char direction)
	{
		this->direction=direction;
	}
	char get_direction(){return direction;}
	float getx()
	{
		return pos_x;
	}
	float gety()
	{
		return pos_y;
	}
	void setx(float x)
	{
		pos_x=x;
		image.setPosition(pos_x,pos_y);
	}
	void sety(float y)
	{
		pos_y=y;
		image.setPosition(pos_x,pos_y);
	}
	void set_id(unsigned short int id){this->id=id;};
	unsigned short int get_id(){return id;};
	//----------конструкторы
	Game_object(float x, float y, int texture_wight, const Texture &texture,int animation_len);
	Game_object(float x, float y, int texture_wight, const Texture &texture,int animation_len,int id);
};