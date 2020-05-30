/*
Class Graph (for directed graphs) as base class of weighted Graph (WGraph) and CRS (CRSMat)
*/


#ifndef  DIGRAPH_H
#define  DIGRAPH_H

#include <vector>



struct Edge_ {
	Edge_() : vhead(unsigned(0)), vtail(unsigned(0)) {}
	Edge_(unsigned h, unsigned t) : vhead(h), vtail(t) {}
	unsigned vhead; // head vertex
	unsigned vtail; // tail vertex
};


class Graph {

public:
	Graph() : Edges(0) {}
	explicit Graph(std::vector<std::vector<unsigned>>& NodeNeighb); // Constructor based on Node -> Node
	size_t ESize() { return Edges.size(); }
	std::vector<Edge_>& getEdges() { return Edges; }
	virtual void Show(); // Subclasses (like WGraph) might redefine it
	virtual ~Graph() { Edges.clear(); }

private:
	std::vector<Edge_> Edges;

};



#endif // ! DIGRAPH_H
