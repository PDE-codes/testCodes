/*
	Weighted graph derived from Graph
*/

#ifndef  WDIGRAPH_H
#define  WDIGRAPH_H

#include "Graph.h"

class WGraph : public Graph {

public:
	WGraph() : weigh(0) {}
	WGraph(std::vector<std::vector<unsigned>>& NodeNeighb, std::vector<std::vector<double>>& nodeCoords);
	WGraph(std::vector<std::vector<unsigned>>& NodeNeighb, std::vector<double>& weights);
	std::vector<double>& getWeigh() { return weigh; }
	void Show();
	~WGraph() { weigh.clear(); }

private:
	std::vector<double> weigh;
};


#endif // ! WeightedDIGRAPH_H

