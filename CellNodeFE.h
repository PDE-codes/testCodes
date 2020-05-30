#ifndef CELLNODEFE_H
#define CELLNODEFE_H


class CellNodeFE {

public:
	CellNodeFE() : Cells(0), Nodes(0), N2C(0),
			CellNeigb(0), NodeNeigb(0), minmaxCells(0),
			 nCell_(0), nNode_(0) {}
			 
	bool readFiles_setNodesCells();
	void setNodeToCell();								// Finds cells that share a node (N2C)
	void findNeigbourCells();							// Finds Neigbouring cells (CellNeigb)
	void findNeigbourNodes();							// Finds Neigbouring Nodes (NodeNeigb)
	std::vector<std::vector<unsigned>>& getNodeNeigbour() { return NodeNeigb; }
	std::vector<std::vector<double>>&   getNodeCoordinate() { return Nodes; }
	size_t findNearestCell(std::vector<double> &p, double &R); // It returns index of the closest cell to point p (R is the distance)

	void printData(int wdata);


private:
	std::vector<std::vector<unsigned>> Cells;						// Node indices of a Cell
	std::vector<std::vector<double>>   Nodes;						// Nodes coordinates
	std::vector<std::vector<unsigned>> N2C;						// Index of cells that share one node (for each node)
	std::vector<std::vector<unsigned>> CellNeigb;					// Index of cells which are neighbours of a cell
	std::vector<std::vector<unsigned>> NodeNeigb;					// Index of nodes which are neighbours of a node
	std::vector<std::vector<double>> minmaxCells;					// Finds lengths of a rectangle that covers a cell
	void setMinMaxCells();
	double findDistance(std::vector<double> &p, std::vector<unsigned>& Cellc, double refR);
	template<typename T>
	void printStructure(std::vector<std::vector<T>>& prData);		// Prints private members
	size_t nCell_, nNode_;								// number of Cells and Nodes
};

#endif // ! CELLNODEFE_H

