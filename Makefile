CFLAG=-std=c++11

EXEC=GestionContainers

$(EXEC): main.o GestionFichier.o Docker.o Socket.o Timer.o
	g++ -o $(EXEC) main.o GestionFichier.o Docker.o Socket.o Timer.o -lrt

GestionFichier.o: GestionFichier.cpp
	g++ -o GestionFichier.o -c GestionFichier.cpp $(CFLAG) -lm

Docker.o: Docker.cpp
	g++ -o Docker.o -c Docker.cpp $(CFLAG) -lm

Socket.o: Socket.cpp
	g++ -o Socket.o -c Socket.cpp $(CFLAG) -lm

Timer.o: Timer.cpp
	g++ -o Timer.o -c Timer.cpp $(CFLAG) -lm -lrt

main.o: main.cpp
	g++ -o main.o -c main.cpp $(CFLAG) -lm
	
clean:
	rm -rf *.o
	
mrproper: clean
	rm -rf $(EXEC))
