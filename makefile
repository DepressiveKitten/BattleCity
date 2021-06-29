CC = g++
FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
CLASSES = main.o Game_object.o Tank.o Bullet.o Ally.o Enemy.o Blocks.o Game.o Connection.o Message_former.o


ALL: clean BattleCity

BattleCity: $(CLASSES)

BattleCity:
	$(CC) -o BattleCity $(CLASSES) $(FLAGS)	
	rm *.o


Game_object.o: Game_object.cpp Game_object.h
	$(CC) -c Game_object.cpp

main.o: main.cpp Game_object.h
	$(CC) -c main.cpp

Tank.o: Tank.cpp Tank.h
	$(CC) -c Tank.cpp

Bullet.o: Bullet.cpp Bullet.h
	$(CC) -c Bullet.cpp

Ally.o: Ally.cpp Ally.h
	$(CC) -c Ally.cpp

Enemy.o: Enemy.cpp Enemy.h
	$(CC) -c Enemy.cpp

Blocks.o: Blocks.cpp Blocks.h
	$(CC) -c Blocks.cpp

Game.o: Game.cpp Game.h
	$(CC) -c Game.cpp

Connection.o: Connection.cpp Connection.h
	$(CC) -c Connection.cpp

Message_former.o: Message_former.cpp Message_former.h
	$(CC) -c Message_former.cpp

clean:
	rm -rf BattleCity