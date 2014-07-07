PROJECT_NAME = HttpIntervalStorage

CPP = $(wildcard *.cpp)

INCLUDE = -I

LIBS = -lboost_system -lboost_thread -lpthread

LIB_DIR = -L

all: make_project

make_project:
	g++ -Wall -O2 -g0 $(CPP) -o $(PROJECT_NAME) $(LIBS)


#	g++ -Wall -O2 -g0 $(CPP) $(INCLUDE) -o $(PROJECT_NAME) $(LIB_DIR) $(LIBS)
    
