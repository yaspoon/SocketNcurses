CC=g++
LIBS= -lncurses -luuid -lpthread
CPPFLAGS= -I./include -g

OUTPUT=./obj
EXEOUTPUT=./bin/Debug

VPATH=src:include:obj

CPPFILES=$(shell ls ./src/*.cpp | sed -e 's/.\/src\///')
OBJS=$(shell ls ./src/*.cpp | sed -e 's/src/obj/;s/cpp/o/')
OBJSTAR=$(shell ls ./src/*.cpp | sed -e 's/.\/src\///;s/cpp/o/')
EXE=SocketNcurses

$(EXE):	$(OBJSTAR) main.o
	$(CC) -o $(EXEOUTPUT)/$@ ./obj/main.o $(OBJS) $(LIBS) $(CPPFLAGS)
	
main.o: main.cpp
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
Client.o: Client.cpp Client.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)

Common.o: Common.cpp Common.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
Network.o: Network.cpp Network.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
Player.o: Player.cpp Player.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
Server.o: Server.cpp Server.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
Timer.o: Timer.cpp Timer.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
Event.o: Event.cpp Event.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
GameState.o: GameState.cpp GameState.h
	$(CC) -c -o $(OUTPUT)/$@ $< $(LIBS) $(CPPFLAGS)
	
clean:
	rm -rf $(OBJS) $(EXEOUTPUT)/$(EXE)
	

