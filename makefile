#outgpp : main_file.cpp CellNodeFE.cpp Graph.cpp WGraph.cpp CRS.cpp
#	g++ -O3 -std=c++17 -Wall -pedantic -Wextra -Weffc++ -Woverloaded-virtual -Wfloat-equal -Wshadow -Wredundant-decls -fmax-errors=1 main_file.cpp CellNodeFE.cpp Graph.cpp WGraph.cpp CRS.cpp -o outgpp





#outclang : main_file.cpp CellNodeFE.cpp Graph.cpp WGraph.cpp CRS.cpp
#	clang++ -O3 -std=c++17 -Weverything -Wno-sign-conversion -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-shorten-64-to-32 -Wno-padded -ferror-limit=1 main_file.cpp CellNodeFE.cpp Graph.cpp WGraph.cpp CRS.cpp -o outclang




# source ~/intel/bin/compilervars.sh intel64



outicpc : main_file.cpp CellNodeFE.cpp Graph.cpp WGraph.cpp CRS.cpp
	icpc -O3 -fargument-noalias -std=c++17 -Wall -Weffc++ -Woverloaded-virtual -Wfloat-equal -Wshadow -wd2015,2012 -wn3 main_file.cpp CellNodeFE.cpp Graph.cpp WGraph.cpp CRS.cpp -o outicpc
