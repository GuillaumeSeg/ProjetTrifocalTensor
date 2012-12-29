CXX      =  g++
CXXFLAGS = -Wall -Werror -O2 -g #-pg #gprof
LIB      = -lSDL -lSDL_image
INCLUDES = -I ~/eigen/

CXXFLAGS += $(INCLUDES)

OBJ      = src/MathIO.o src/draw.o src/main.o
RM       = rm -f
BIN      = bin/trifocal
DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)
STDNAME  = $(DIRNAME).tgz

all : $(BIN)

$(BIN) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIB) $(INCLUDES)  -o $(BIN)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN) &"
	@echo "--------------------------------------------------------------"

MathIO.o : MathIO.cpp MathIO.hpp 
	@echo "compile MathIO"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

draw.o : draw.cpp draw.hpp 
	@echo "compile MathIO"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

main.o : main.cpp MathIO.o draw.o
	@echo "compile main"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ) $(BIN)  

bigclean :
	@echo "**************************"
	@echo "BIG CLEAN"
	@echo "**************************"
	find . -name '*~' -exec rm -fv {} \;
	$(RM) *~ $(OBJ) $(BIN) output/*

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)
