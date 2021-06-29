#include <SFML/Graphics.hpp>
#include "Game.h"
#include <list>

unsigned short int Bullet::bullet_id;
unsigned short int Blocks::block_id;
list<Tank*>* Tank::tanks_for_id;
unsigned short int Tank::Tank_id;

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "Russian");
    Bullet::set_Bullet_id(100);
    Blocks::set_Block_id(1000);
    Tank::set_Tank_id(1);
    printf("press 1 to become SERVER or 2 to become CLIENT ");
    int choise;
    do{
    scanf("%d",&choise);
    }while(choise<1||choise>2);
    bool win=false;
    if(choise==1)
    {
        Connection con1(true,"127.0.0.1\0",45000);
        con1.set_send_adress("127.0.0.1\0",45001);
        int level;
        do{
            printf("choose level ");
            scanf("%d",&level);
        }while(level<1&&level>35);
        Game game(level,false,&con1);
        win=game.play();
    }
    else
    {

        Connection con2(false,"127.0.0.1\0",45001);
        int level;
        do{
            printf("choose level ");
            scanf("%d",&level);
        }while(level<1&&level>35);
        Game game(level,false,&con2);
        win=game.play_remote();
    }
    if (win)
    {
        printf("CONGRATS, YOU WIN!!!");
    }
    else
    {
        printf("YOU LOSE");
    }
    return 0;
}
