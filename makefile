#makefile

cc = g++
source = LKMain.cpp LKMatrix.cpp
FLAG = -Wall -g
LKSolver:$(source)
	${cc} $(FLAG) $^ -o $@ 
.PHONY:clean
clean:
	rm  LKSolver  *.tsp out
