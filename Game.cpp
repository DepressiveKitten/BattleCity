#include "Game.h"
#include <fstream> 

using namespace std;

bool check_collision(float x_left,float y_left,float x_right, float y_right,int x,int y)
{
    if ( x_left > x+GSCALE*16 || x_right < x || y_left > y+GSCALE*16 ||y_right < y ) {
        return false;
    } 
    return true;
}

bool Game::is_empty(int x,int y)
{
    int x_right=x+16*GSCALE;
    int y_right=x+16*GSCALE;
    for(auto i = tanks.begin(); i != tanks.end();i++)
    {
        if(check_collision((*i)->getx()+GSCALE*2,
        (*i)->gety()+GSCALE*2,
        (*i)->getx()+(*i)->get_sprite_size()-GSCALE,
        (*i)->gety()+(*i)->get_sprite_size()-GSCALE,x,y))
        {
            return false;
        }
    }
    for(auto i = blocks.begin(); i != blocks.end();i++)
    {
        if((*i)->get_type()<3)
        {
            if(check_collision((*i)->getx()+GSCALE*2,
            (*i)->gety()+GSCALE*2,
            (*i)->getx()+(*i)->get_sprite_size()-GSCALE,
            (*i)->gety()+(*i)->get_sprite_size()-GSCALE,x,y))
            {
                return false;
            }
        }
    }
    return true;
}



void Game::set_forced_controls()
{
    char direction=0;
    if(Keyboard::isKeyPressed(Keyboard::Key::W))
    {
        direction=1;
    }
    if(Keyboard::isKeyPressed(Keyboard::Key::A))
    {
        direction=2;
    }
    if(Keyboard::isKeyPressed(Keyboard::Key::S))
    {
        direction=3;
    }
    if(Keyboard::isKeyPressed(Keyboard::Key::D))
    {
        direction=4;
    }
    char isShouting=0;
    if(Keyboard::isKeyPressed(Keyboard::Key::Space))
    {
        isShouting=1;
    }
    message_former->set_input_message(direction,isShouting);
}

//----------------------------------------------------------------------------------------------------------------------------------------

bool Game::play()
{
    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||Keyboard::isKeyPressed(Keyboard::Key::Escape))
                app.close();
        }
        message_former->recieve();

        //---------------------------------------------------------
        if(player1_respawn_time>=-1){player1_respawn_time--;}
        if(player2_respawn_time>=-1){player2_respawn_time--;}
        if(player1_respawn_time==0)
        {
            player1 = new Ally(GSCALE*4*16,GSCALE*12*16,tank_textures[0],true);
            player1->set_id(1);
            tanks.push_back(player1);
            player1->hit(false);
        }
        if(player2_respawn_time==0)
        {
            player2 = new Ally(GSCALE*8*16,GSCALE*12*16,tank_textures[1],false);
            player2->set_id(2);
            tanks.push_back(player2);
            player2->hit(false);
        }


        //---------------------------------------------------------


        if(enemy_spawn_time>0){enemy_spawn_time--;}
        if(enemy_amount==0&&alive_enemy==0)
        {
            win=true;
            app.close();
        }
        if(enemy_spawn_time==GSPAWNTICK*4&&enemy_amount>0) //создать эффект спавна
        {
            bool flag=true;
            for(int i=0;i<10;i++)
            {
                int x=(rand()%25)*8*GSCALE;
                int y=(rand()%8)*8*GSCALE;
                if(is_empty(x,y))
                {
                    Blocks *block=new Blocks(x,y,tile_textures[7],1,16,GSPAWNTICK,GSPAWNTICK*4);
                    blocks.push_back(block);
                    enemy_y=y;
                    enemy_x=x;
                    flag=false;
                    break;
                }
            }
            if(flag){enemy_spawn_time++;}
        }
        if(enemy_spawn_time==0&&enemy_amount>0)
        {
            enemy_amount--;
            enemy_spawn_time=GENEMYSPAWN;
            alive_enemy++;
            Tank *tank = new Enemy(enemy_x,enemy_y,tank_textures[3]);
            tanks.push_back(tank);
        }
        //---------------------------------------------------------


        input_message temp_message=message_former->get_input_message();
        player2->set_forced_direction(temp_message.direction);
        player2->set_control(temp_message.isShooting);
        
        app.clear();
        for(auto i = tanks.begin(); i != tanks.end();)
        {
            int flag=(*i)->update(app,false,tanks,blocks);
            if(flag==0)
            {
                Blocks *block=new Blocks((*i)->getx()-8*GSCALE,(*i)->gety()-8*GSCALE,tile_textures[6],3,32,GFRAMERATE/4,GFRAMERATE/2);
                blocks.push_back(block);
                if(player1==*i)
                {
                    player1_respawn_time=GPLAYERSPAWN;
                }
                if(player2==*i)
                {
                    player2_respawn_time=GPLAYERSPAWN;
                }
                if(*i!=player1&&*i!=player2)
                {
                    alive_enemy--;
                }
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                message_former->add_id((*i)->get_id());
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                i=tanks.erase(i);
                continue;
            }
            if(flag==2)
            {
                Bullet *bullet=new Bullet((*i)->get_bullet_x(),(*i)->get_bullet_y(),bullet_texture,(*i)->get_direction(),(*i)->if_enemy(),false);
                bullets.push_back(bullet);
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
                common_info object;
                object.id=bullet->get_id();
                object.direction=bullet->get_direction();
                object.x=(int)bullet->getx();
                object.y=(int)bullet->gety();
                message_former->add_object(object);

                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            }

            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
            common_info object;
            object.id=(*i)->get_id();
            object.direction=(*i)->get_direction();
            object.x=(int)(*i)->getx();
            object.y=(int)(*i)->gety();
            message_former->add_object(object);

            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            i++;
        }
        for(auto i = blocks.begin(); i != blocks.end();)
        {
            if(!(*i)->update(app))
            {
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                if((*i)->get_id()<2000)
                {
                    message_former->add_id((*i)->get_id());
                }
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                if((*i)==base)
                {
                    win=false;
                    base->Next_Sprite();
                    app.close();
                }
                i=blocks.erase(i);
                continue;
            }
            i++;
        }
        for(auto i = bullets.begin(); i != bullets.end();)
        {
            if(!(*i)->update(app,tanks,blocks))
            {
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                message_former->add_id((*i)->get_id());
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                Blocks *block=new Blocks((*i)->getx()-12*GSCALE/2,(*i)->gety()-12*GSCALE/2,tile_textures[5],3,16,GFRAMERATE/9,GFRAMERATE/3);
                blocks.push_back(block);
                i=bullets.erase(i);
                continue;
            }
            i++;
        }
        app.display();
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        message_former->send();
        message_former->inc_frame();
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    }
    tanks.clear();
    bullets.clear();
    return win;
}


//----------------------------------------------------------------------------------------------------------------------------------------


bool Game::play_remote()
{
    player1=nullptr;
    player2=nullptr;
    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed||Keyboard::isKeyPressed(Keyboard::Key::Escape))
                app.close();
        }
        if(enemy_amount==0&&alive_enemy==0)
        {
            win=true;
            app.close();
        }

        set_forced_controls();
        message_former->send_input();
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        message_former->recieve();
        for(;;)
        {
            common_info object=message_former->get_object();
            if(object.id==0)
            {
                break;
            }
            if(object.id==1)
            {
                if(player1==nullptr)
                {
                    player1 = new Ally(GSCALE*4*16,GSCALE*12*16,tank_textures[0],false);
                    player1->set_id(1);
                    tanks.push_back(player1);
                    continue;
                }
            }
            if(object.id==2)
            {
                if(player2==nullptr)
                {
                    player2 = new Ally(GSCALE*8*16,GSCALE*12*16,tank_textures[1],false);
                    player2->set_id(2);
                    tanks.push_back(player2);
                    continue;
                }
            }
            if(object.id<=26)
            {
                bool flag=false;
                for(auto i = tanks.begin(); i != tanks.end();i++)
                {
                    if((*i)->get_id()==object.id)
                    {
                        flag=true;
                        (*i)->setx(object.x);
                        (*i)->sety(object.y);
                        (*i)->set_direction(object.direction);
                        break;
                    }
                }
                if(flag==false)
                {
                    enemy_amount--;
                    alive_enemy++;
                    Tank *tank = new Enemy(object.x,object.y,tank_textures[3]);
                    tank->set_id(object.id);
                    tanks.push_back(tank);
                }
            }
            else
            {
                if(object.id<1000)
                {
                    bool flag=false;
                    for(auto i = bullets.begin(); i != bullets.end();i++)
                    {
                        if((*i)->get_id()==object.id)
                        {
                            flag=true;
                            break;
                        }
                    }
                    if(flag==false)
                    {
                        Bullet *bullet=new Bullet(object.x,object.y,bullet_texture,object.direction,true,true);
                        bullet->set_id(object.id);
                        bullets.push_back(bullet);
                    }
                }
            }

        }

        for(;;)
        {
            int id=message_former->get_id();
            if(id==NULL)
            {
                break;
            }
            if(id>0&&id<100)
            {
                for(auto i = tanks.begin(); i != tanks.end();i++)
                {
                    if(id==(*i)->get_id())
                    {
                        Blocks *block=new Blocks((*i)->getx()-8*GSCALE,(*i)->gety()-8*GSCALE,tile_textures[6],3,32,GFRAMERATE/4,GFRAMERATE/2);
                        block->set_id(5000);
                        blocks.push_back(block);
                        if(id>2)
                        {
                            alive_enemy--;
                        }
                        if(id==1)
                        {
                            player1=nullptr;
                        }
                        if(id==2)
                        {
                            player2=nullptr;
                        }
                        i=tanks.erase(i);
                        break;
                    }
                }
            }
            if(id>=100&&id<1000)
            {
                for(auto i = bullets.begin(); i != bullets.end();i++)
                {
                    if(id==(*i)->get_id())
                    {
                        Blocks *block=new Blocks((*i)->getx()-12*GSCALE/2,(*i)->gety()-12*GSCALE/2,tile_textures[5],3,16,GFRAMERATE/9,GFRAMERATE/3);
                        block->set_id(5000);
                        blocks.push_back(block);
                        i=bullets.erase(i);
                        break;
                    }
                }
            }
            if(id>=1000)
            {
                for(auto i = blocks.begin(); i != blocks.end();i++)
                {
                    if(id==(*i)->get_id())
                    {
                        if((*i)==base)
                        {
                            win=false;
                            base->Next_Sprite();
                            app.close();
                        }
                        i=blocks.erase(i);
                    }
                }
            }
        }
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        app.clear();
        for(auto i = tanks.begin(); i != tanks.end();i++)
        {
            (*i)->update(app,true,tanks,blocks);
        }
        for(auto i = blocks.begin(); i != blocks.end();i++)
        {
            (*i)->update(app);
            if((*i)->get_timeout()==0)
            {
                i=blocks.erase(i);
            }
        }
        for(auto i = bullets.begin(); i != bullets.end();i++)
        {
            (*i)->update(app,tanks,blocks);
        }
        app.display();
    }
    tanks.clear();
    bullets.clear();
    return win;
}
 

//----------------------------------------------------------------------------------------------------------------------------------------



void Game::load_level(string level)
{
    base = new Blocks(GSCALE*6*16,GSCALE*12*16,tile_textures[4],0,16);
    blocks.push_back(base);

    int tempmas[27][27];
    for(int i=0;i<26;i++){for(int j=0;j<26;j++){tempmas[i][j]=0;}}
    ifstream textures(level);
    for(int i=0;i<26;i++)
    {
        for(int j=0;j<26;j++)
        {
            char temp;
		    textures >> temp;
            if(tempmas[i][j]==1)
            {
                continue;
            }
            if(temp=='#')
            {
                Blocks *block = new Blocks(j*8*GSCALE,i*8*GSCALE,tile_textures[0],0,8);
                blocks.push_back(block);
            }
            if(temp=='@')
            {
                Blocks *block = new Blocks(j*8*GSCALE,i*8*GSCALE,tile_textures[1],1,8);
                blocks.push_back(block);
            }
            if(temp=='%')
            {
                Blocks *block = new Blocks(j*8*GSCALE,i*8*GSCALE,tile_textures[2],3,16);
                blocks.push_back(block);
                tempmas[i+1][j+1]=1;
                tempmas[i+1][j]=1;
                tempmas[i][j+1]=1;
            }
            if(temp=='~')
            {
                Blocks *block = new Blocks(j*8*GSCALE,i*8*GSCALE,tile_textures[3],2,16,GFRAMERATE/3);
                blocks.push_back(block);
                tempmas[i+1][j+1]=1;
                tempmas[i+1][j]=1;
                tempmas[i][j+1]=1;
            }
        }
    }
    Blocks::set_Block_id(2000);
}

void Game::load_textures()
{
    for(int i = 0;i<3;i++){
        tank_textures[i].loadFromFile("resourses/player"+to_string(i+1)+".png");
    }
    for(int i = 0;i<3;i++){
        tank_textures[i+3].loadFromFile("resourses/enemy"+to_string(i+1)+".png");
    }
    bullet_texture.loadFromFile("resourses/bullet.png");
    ifstream textures("resourses/textures.txt");
    for(int i=0;i<8;i++)
    {
        string temp;
		textures >> temp;
        tile_textures[i].loadFromFile(temp);
    }
}

Game::Game(int level,bool remote,Connection* con)
{
    message_former=new Message_former(con);
    Tank::tanks_for_id=&tanks;
    load_textures();
    load_level("levels/"+to_string(level));


    enemy_amount=10;
    alive_enemy=0;
    enemy_spawn_time=GSPAWNTICK*6;
    player1_respawn_time=1;
    player2_respawn_time=1;


    app.create(sf::VideoMode(GBORDER, GBORDER), "Battle City");
    app.setFramerateLimit(GFRAMERATE);
    this->remote=remote;
}