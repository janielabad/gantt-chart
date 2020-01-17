# Makefile for Gantt chart + graph adjacency list project

CC = g++ -g -Wall -Wextra -pedantic -std=c++11 -O3
DEPS = linkedQueue.h
OBJS = projectInfo.o ganttUtils.o

all: projectInfo

ganttUtils: ganttUtils.o
	$(CC) -o ganttUtils ganttUtils.o

ganttUtils.o: ganttUtils.cpp ganttUtils.h $(DEPS)
	$(CC) -c ganttUtils.cpp

projectInfo: projectInfo.o ganttUtils.o
	$(CC) -o projectInfo projectInfo.o ganttUtils.o

projectInfo.o: projectInfo.cpp ganttUtils.o
	$(CC) -c projectInfo.cpp

# -----
# clean by removing object files

clean:
	rm $(OBJS)
