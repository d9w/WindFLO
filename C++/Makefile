CC=g++
CFLAGS =-Wall -std=c++0x -g
OPTFLAGS=-O2
INCLUDES=-I$(PWD) -lcurl

SOURCES=tinyxml2.cpp WindScenario.cpp WindFarmLayoutEvaluator.cpp KusiakLayoutEvaluator.cpp
OBJECTS=$(SOURCES:.cpp=.o)
COMP_SOURCES=jsoncpp.cpp CompetitionScenario.cpp WindFarmLayoutEvaluator.cpp CompetitionEvaluator.cpp
COMP_OBJECTS=$(COMP_SOURCES:.cpp=.o)

main:$(COMP_OBJECTS)
	$(CC) $(CFLAGS) -o competition.o $(COMP_OBJECTS) GA.cpp main.cpp $(INCLUDES) $(OPTFLAGS)

evaluate:$(OBJECTS)
	$(CC) $(CFLAGS) -o evaluate.o $(OBJECTS) evaluate.cpp $(INCLUDES) $(OPTFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@ -w $(INCLUDES) $(OPTFLAGS)

clean:
	rm $(OBJECTS) $(COMP_OBJECTS) competition.o evaluate.o 2> /dev/null
