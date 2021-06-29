#include <SFML/Graphics.hpp>
#include "Game_object.h"
#include "Tank.h"
#include "Bullet.h"
#include "Ally.h"
#include "Enemy.h"
#include "Blocks.h"
#include <list>
#include <iostream>
#include "Message_former.h"

using namespace std;

class Game
{
private:
    Message_former *message_former;

    bool win;
    Ally* player1;
    Ally* player2;
    Blocks* base;
    list<Tank*> tanks;
    list<Bullet*> bullets;
    list<Blocks*> blocks;
    sf::RenderWindow app;
    bool remote;
    Texture tile_textures[8];
    //0-brick, 1-wall, 2-grass, 3-water,
    //4-base,5-6-explosion,7-spawn
    Texture tank_textures[6];//0-2 players,3-5 enemy
    Texture bullet_texture;

    int enemy_spawn_time;
    int enemy_amount;
    int alive_enemy;
    int player1_respawn_time;
    int player2_respawn_time;
    int enemy_x,enemy_y;

    bool is_empty(int x,int y);
    void load_textures();
    void load_level(string level);
    void set_forced_controls();
public:
    bool play();
    bool play_remote();
    Game(int level,bool remote,Connection* con);
};
