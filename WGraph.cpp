#include <iostream>
#include <math.h>
#include "WGraph.h"


using namespace std;

/* 
	Distance of two vertices of an edge is set as wieght of that edge
	For a diagonal element (an edge with the same vertices) we set 10 as weight
*/

WGraph::WGraph(vector<vector<unsigned>>& NodeNeighb, vector<double>& weights)
	: Graph(NodeNeighb), // Base class constructor 
	weigh(weights) {}



WGraph::WGraph(vector<vector<unsigned>>& NodeNeighb, vector<vector<double>>& nodeCoords) 
	: Graph(NodeNeighb), // Base class constructor
	weigh(0)
{
	double lx, ly;
	vector<Edge_>& digrEdg(getEdges());
	weigh.clear();

	for (auto it = digrEdg.begin(); it != digrEdg.end(); ++it)
	{
		lx = nodeCoords[it->vhead][0] - nodeCoords[it->vtail][0];
		ly = nodeCoords[it->vhead][1] - nodeCoords[it->vtail][1];

		// We set 10 for diagonal elements since nodes distance is infinity
		if (fabs(lx) + fabs(ly) < 1e-12) // <-- diagonal element
			weigh.push_back(10);
		else
			weigh.push_back(1.0 / (lx*lx + ly*ly));

	}

}



void WGraph::Show() {
	vector<Edge_>& edgs(getEdges());
	cout << "\n\n WGraph::Show()\n" << endl;
	for (size_t c = 0; c < edgs.size(); ++c)
		cout << " (" << edgs[c].vhead << "," << edgs[c].vtail << ", " << weigh[c] << ")" << endl;
}
