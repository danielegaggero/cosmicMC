SHELL=/bin/bash
WORKDIR=$(shell pwd)

CC=gcc
CXX=g++
CFLAGS = -fopenmp -g -Wall -O3 
INCS=-I.

${WORKDIR}/%.o : ${WORKDIR}/%.cpp
	@(echo "Compiling $(@F)")
	$(CC) $(CFLAGS) -c $< -o $@

NOBJS = ${WORKDIR}/main.o ${WORKDIR}/sourcesMC.o ${WORKDIR}/randgen.o  ${WORKDIR}/input.o ${WORKDIR}/tinystr.o ${WORKDIR}/tinyxml.o ${WORKDIR}/tinyxmlerror.o ${WORKDIR}/tinyxmlparser.o 

all:    $(NOBJS) 
	$(CXX) $(CFLAGS) $(INCS) -o ${WORKDIR}/sources_MC ${WORKDIR}/main.o ${WORKDIR}/sourcesMC.o ${WORKDIR}/randgen.o ${WORKDIR}/input.o ${WORKDIR}/tinystr.o ${WORKDIR}/tinyxml.o ${WORKDIR}/tinyxmlerror.o ${WORKDIR}/tinyxmlparser.o 

clean:
	rm -f ${WORKDIR}/*.o ${WORKDIR}/sources_MC
