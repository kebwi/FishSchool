CC = g++#gcc
FLAGS = -Wall -O3 #-g
DIR = Source/

all: fishSchool




fishSchool: fishSchool.o Agent.o Prey.o Predator.o Random.o tgasave.o screengrab.o
	# Mac
	$(CC) $(FLAGS) -o fishSchool fishSchool.o Agent.o Prey.o Predator.o Random.o tgasave.o screengrab.o -framework OpenGL -framework GLUT
	# Linux
	# $(CC) $(FLAGS) -o fishSchool fishSchool.o Agent.o Prey.o Predator.o Random.o tgasave.o screengrab.o -lGL -lGLU -lGLEW -lglut




fishSchool.o: $(DIR)fishSchool.cpp $(DIR)Random.h $(DIR)MathStuff.h $(DIR)smallStructs.h
	$(CC) $(FLAGS) -c $(DIR)fishSchool.cpp

Agent.o: $(DIR)Agent.cpp $(DIR)Random.h $(DIR)MathStuff.h $(DIR)smallStructs.h
	$(CC) $(FLAGS) -c $(DIR)Agent.cpp

Prey.o: $(DIR)Prey.cpp $(DIR)Random.h $(DIR)MathStuff.h $(DIR)smallStructs.h
	$(CC) $(FLAGS) -c $(DIR)Prey.cpp

Predator.o: $(DIR)Predator.cpp $(DIR)Random.h $(DIR)MathStuff.h $(DIR)smallStructs.h
	$(CC) $(FLAGS) -c $(DIR)Predator.cpp

Random.o: $(DIR)Random.cpp $(DIR)Random.h $(DIR)MathStuff.h
	$(CC) $(FLAGS) -c $(DIR)Random.cpp

tgasave.o: $(DIR)screengrab/tgasave.cpp $(DIR)screengrab/tgasave.h
	$(CC) $(FLAGS) -c $(DIR)screengrab/tgasave.cpp

screengrab.o: $(DIR)screengrab/screengrab.cpp $(DIR)screengrab/screengrab.h
	$(CC) $(FLAGS) -c $(DIR)screengrab/screengrab.cpp




clean:
	rm -f *.o fishSchool
