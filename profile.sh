g++ -g temp.cpp knnSearch.cpp
valgrind --tool=callgrind --cache-sim=yes --branch-sim=yes --dump-instr=yes --collect-jumps=yes --log-file=cg.out ./a.out
