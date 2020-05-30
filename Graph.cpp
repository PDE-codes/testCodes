#include <iostream>
#include "Graph.h"

using namespace std;

Graph::Graph(vector<vector<unsigned>>& NodeNeighb) : Edges(0)
{
	Edges.clear();
	for (unsigned head = 0; head < NodeNeighb.size(); ++head) {
		Edges.push_back(Edge_(head, head)); // This line adds diagonal element
		for (auto tailIt = NodeNeighb[head].begin(); tailIt != NodeNeighb[head].end(); ++tailIt)
			Edges.push_back(Edge_(head, *tailIt));
	}
}



void Graph::Show() {
	cout << "\n\n Graph::Show()\n" << endl;
	for (auto gIt = Edges.begin(); gIt != Edges.end(); ++gIt)
		cout << " (" << gIt->vhead << "," << gIt->vtail << ")" << endl;
}
