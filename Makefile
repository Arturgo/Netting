# Copyright 2022, Gurobi Optimization, LLC

PLATFORM = linux64
INC      = /home/arthur/ENS/PACE22/gurobi951/linux64/include/
CPP      = g++
CARGS    = -m64 -g
CPPLIB   = -L/home/arthur/ENS/PACE22/gurobi951/linux64/lib/ -lgurobi_c++ -lgurobi95


main_gurobi:
	$(CPP) $(CARGS) -o grid_lock grid_lock.cpp -std=c++11 $< -I$(INC) $(CPPLIB) -lm
	$(CPP) $(CARGS) -o optim optim.cpp -std=c++11 $< -I$(INC) $(CPPLIB) -lm
	$(CPP) $(CARGS) -o iterative_rounding iterative_rounding.cpp -std=c++11 $< -I$(INC) $(CPPLIB) -lm
