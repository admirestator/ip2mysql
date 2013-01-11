######################################################################
## Filename:      Makefile
## Version:       0.01
## Author:        吴旭 <admirestator@gmail.com>
## Created at:    Sat Apr 21 17:10:53 2012
## Description:   makefile for czip2mysql
######################################################################

CC=clang
CXX=clang++
#CXXFLAGS=-Wall -Werror -ggdb -O3
CXXFLAGS=-Wall -Werror -O3

ALL=import
SRC=cz2mysql.cpp
OBJ=cz2mysql.o

INLCUDE=./include
LIB=./lib
LD=-lmysqlcppconn

$(ALL):cz2mysql.o
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(ALL) $(LD) -L$(INCLUDE) -L$(LD)

cz2mysql.o:cz2mysql.cpp
	$(CXX) $(CXXFLAGS) -c cz2mysql.cpp

clean:
	rm -rf $(ALL) $(OBJ)
