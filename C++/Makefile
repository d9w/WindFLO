CC=g++
CFLAGS =-Wall -std=c++0x -g
OPTFLAGS=-O2
INCLUDES=-I$(PWD)

SOURCES=tinyxml2.cpp WindScenario.cpp WindFarmLayoutEvaluator.cpp KusiakLayoutEvaluator.cpp GA.cpp
OBJECTS=$(SOURCES:.cpp=.o)

main:$(OBJECTS)
	$(CC) $(CFLAGS) -o eval.o $(OBJECTS) main.cpp $(INCLUDES) $(OPTFLAGS)

evaluate:$(OBJECTS)
	$(CC) $(CFLAGS) -o evaluate.o $(OBJECTS) evaluate.cpp $(INCLUDES) $(OPTFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@ -w $(INCLUDES) $(OPTFLAGS)

clean:
	rm $(OBJECTS) eval.o 2> /dev/null
