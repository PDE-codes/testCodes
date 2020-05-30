#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <string>
#include <iomanip>
#include <math.h>


#include "CellNodeFE.h"

using namespace std;

void CellNodeFE::printData(int wdata) {

	switch (wdata)
	{

	case 0:
		cout << endl << " CELLS" << endl;
		cout << "index \t 1stNode \t 2ndNode \t 3rdNode \t 4thNode" << endl;
		printStructure(Cells);
		return;

	case 1:
		cout << endl << " NODES" << endl;
		cout << "Index \t x-coord \t y-coord" << endl;
		printStructure(Nodes);
		return;

	case 2:
		cout << endl << " N2C : Node -> Cell : Cell indices that share node" << endl;
		cout << "NodeIndex 1stCell \t ..." << endl;
		printStructure(N2C);
		return;

	case 3:
		cout << endl << " Cell Neighbours : Cell -> Cell : Cell indices that are neighbours" << endl;
		cout << "CellIndex  1stNeighbCell \t ..." << endl;
		printStructure(CellNeigb);
		return;

	case 4:
		cout << endl << " Node Neighbours : Node -> Node : Node indices that are neighbours" << endl;
		cout << " A neigbouring node is not necessirly connected by an edge, ";
		cout << " for example in a rectanglur element the diagonal nodes are neighbours, but not connected.\n";
		cout << "NodeIndex  1stNode \t ..." << endl;
		printStructure(NodeNeigb);
		return;

	default:
		cout << "bWill be implemented??" << endl;
		return;
	}
}


double CellNodeFE::findDistance(vector<double> &p, vector<unsigned>& Cellc, double refR)
{
	double R, v1x, v1y, v2x, v2y, inprod, normv1, normv2;
	vector<unsigned> NodeIdx(Cellc);
	vector<double> RV(Cellc.size());
	NodeIdx.push_back(Cellc[0]);

	for (unsigned i = 0; i < Cellc.size(); ++i)
	{
		double y1(Nodes[NodeIdx[i]][1]), x1(Nodes[NodeIdx[i]][0]);
		v1x = p[0] - x1;
		v1y = p[1] - y1;
		v2x = Nodes[NodeIdx[i + 1]][0] - x1;
		v2y = Nodes[NodeIdx[i + 1]][1] - y1;
		inprod = v1x*v2x + v1y*v2y;
		normv1 = v1x*v1x + v1y*v1y;

		if (inprod < 0) RV[i] = normv1;
		else {
			normv2 = v2x*v2x + v2y*v2y;
			if (normv1 <= normv2) RV[i] = pow(v1x*v2y - v1y*v2x, 2) / normv2;
			else {
				v1x = p[0] - Nodes[NodeIdx[i + 1]][0];
				v1y = p[1] - Nodes[NodeIdx[i + 1]][1];
				RV[i] = v1x*v1x + v1y*v1y;
			}
		}
	}

	R = *min_element(RV.begin(), RV.end());
	if (R > refR) return R;

	// Check if is inside of current cell
	double sgn(0.0);
	for (unsigned i = 0; i < Cellc.size(); ++i) {
		double y1(Nodes[NodeIdx[i]][1]), x1(Nodes[NodeIdx[i]][0]), outprod;
		
		v1x = p[0] - x1;
		v1y = p[1] - y1;
		v2x = Nodes[NodeIdx[i + 1]][0] - x1;
		v2y = Nodes[NodeIdx[i + 1]][1] - y1;

		outprod = v2x*v1y - v2y*v1x;

		if (fabs(outprod) < 1e-10)
		{   
			/* 
				Coordinate is along an edge. So if v1 and v2 had the same directions
				and ||v2|| - ||v1|| >= 0 then it is on boundary
			*/
			double normdiff(v2x*v2x + v2y*v2y - v1x*v1x - v1y*v1y);
			if (v2y*v1y >= 0 && v1x*v2x >= 0 && normdiff > -1e-10)
			{
				cout << "\t On boundary of a cell (error tolerance : 1e-10)" << endl; 
				return -1.0;
			}
			else return R;
		}

		if (i == 0) sgn = outprod / fabs(outprod);
		if (sgn*outprod < 0) return R;
	}

	cout << "\t Interior point" << endl;
	return -1.0;
}





void CellNodeFE::setMinMaxCells()
{
	minmaxCells.resize(nCell_);
	vector<double> xx(10), yy(10);

	for (size_t cc = 0; cc < nCell_; ++cc) {
		size_t cxy = 0;
		for (unsigned nn : Cells[cc]) {
			xx[cxy] = Nodes[nn][0];
			yy[cxy] = Nodes[nn][1];
			++cxy;
		}
		auto resx = minmax_element(xx.begin(), xx.begin() + cxy);
		auto resy = minmax_element(yy.begin(), yy.begin() + cxy);
		minmaxCells[cc] = { *resx.first, *resx.second, *resy.first, *resy.second };
	}
}




// It returns index of the closest cell to point p (R is the distance)
size_t CellNodeFE::findNearestCell(vector<double> &p, double &R)
{

	size_t nearsetIdx(0);
	vector<bool> checkit(nCell_, true);
	bool nearFound(false);
	double sqrtR;

	R = findDistance(p, Cells[0], 1e14);
	if (R < 0) { R = 0;  return 0; }
	checkit[0] = false;

	while (!nearFound) {
		for (size_t cc = 0; cc < nCell_; ++cc) {

			nearFound = true;
			if (!checkit[cc]) continue;
			nearFound = false;
			checkit[cc] = false;

			sqrtR = sqrt(R);
			if (minmaxCells[cc][0] >= p[0] + sqrtR || minmaxCells[cc][1] <= p[0] - sqrtR ||
				minmaxCells[cc][2] >= p[1] + sqrtR || minmaxCells[cc][3] <= p[1] - sqrtR)
				continue;

			else { // Potential candidate : find distance from this element
				double RN(findDistance(p, Cells[cc], R));
				if (RN < 0) { R = 0.0; return cc; }
				if (RN < R) { R = RN; nearsetIdx = cc; }
			}
		}
	}
	R = sqrt(R);
	return nearsetIdx;
}





// prints private memebers Cells, Nodes, N2C ...
template<typename T>
void CellNodeFE::printStructure(vector<vector<T>>& vecData) {
	
	size_t cntr(0);
	for (auto vecIt = vecData.begin(); vecIt != vecData.end(); ++vecIt) {
		cout << " #" << cntr++ << "\t";
		for (auto It2 = vecIt->begin(); It2 != vecIt->end(); ++It2) cout << setw(8) << *It2 << "\t";
		cout << endl;
	}
}





/*
Finds Neigbouring nodes of a node
A neigbouring node is not necessirly connected by an edge,
for example in a rectanglur element the diagonal nodes are neighbours, but not connected.
*/

void CellNodeFE::findNeigbourNodes() {

	NodeNeigb.resize(nNode_);

	vector<unsigned> NodeNb;
	NodeNb.reserve(100);

	for (unsigned cn = 0; cn != nNode_; ++cn)
	{
		vector<unsigned> &curN2C(N2C[cn]), locNodeNb;

		for (unsigned cc : curN2C)
		{
			locNodeNb = Cells[cc];
			for (unsigned jj = 0; jj < locNodeNb.size(); ++jj) NodeNb.push_back(locNodeNb[jj]);
		}

		sort(NodeNb.begin(), NodeNb.end());
		vector<unsigned>::iterator NIt(unique(NodeNb.begin(), NodeNb.end()));

		NodeNb.resize(distance(NodeNb.begin(), NIt));
		for (unsigned jj : NodeNb) if (jj != cn) NodeNeigb[cn].push_back(jj);
		NodeNb.clear();
	}
}





/*
Finds Neigbouring cells
If two cells share two nodes (or an edge), they are neighbour
*/

void CellNodeFE::findNeigbourCells() {

	CellNeigb.resize(nCell_);


	vector<unsigned> set1, set2, commonNodes(100), ThisCell;
	vector<unsigned>::iterator cNitEnd;

	for (unsigned cc = 0; cc != nCell_; ++cc)
	{
		ThisCell = Cells[cc];
		size_t ThisCellSize(ThisCell.size());

		for (size_t nodeCa = 0; nodeCa < ThisCellSize - 1; ++nodeCa)
			for (size_t nodeCb = nodeCa + 1; nodeCb < ThisCellSize; ++nodeCb)
			{
				set1 = N2C[ThisCell[nodeCa]];
				set2 = N2C[ThisCell[nodeCb]];
				sort(set1.begin(), set1.end());
				sort(set2.begin(), set2.end());

				/*
				This intersection includes Cells that have both
				Node[nodeCa] and Node[nodeCb], so a neighbour of current cell cc
				*/
				cNitEnd = set_intersection(set1.begin(), set1.end(),
					set2.begin(), set2.end(), commonNodes.begin());

				for (auto It2 = commonNodes.begin(); It2 != cNitEnd; ++It2)
					if (*It2 != cc) CellNeigb[cc].push_back(*It2);
			}
	}
}




/*
We traverse over elements and index of that cell
is set into N2C[node indices of that cell] = index of that cell
*/

void CellNodeFE::setNodeToCell() {

	N2C.resize(nNode_);

	for (unsigned c = 0; c != nCell_; ++c) {
		auto cellIt(&Cells[c]);
		for (auto It2 = cellIt->begin(); It2 != cellIt->end(); ++It2)
			N2C[*It2].push_back(c);
	}
}




/*
Reads cell and node files to initialize Cells and Nodes members.
Also sets nCell_ and nNode_
*/
bool CellNodeFE::readFiles_setNodesCells() {

	// Open Cell file which includes node indices of each cell
	ifstream fs;
	string fName;
	
	cout << " Input (Elements) connectivity file name: ";
	cin >> fName;
	while (true) {
		fs.open(fName);
		if (!fs.good())
			cout << "\n Could NOT open file to read ...\n" <<
                       "press (c) to end or enter a new file name ...\n";
		else { cout << "\n opening file succesfully\n\n";  break; }
		cin >> fName;
		if (fName.compare(string("c")) == 0) { cout << "Finished!" << endl; return false; }
	}

	// Now read node indices of each cell
	unsigned nCells, nd1, nd2, nd3;
	fs >> nCells;
	for (unsigned nc = 0; nc < nCells; ++nc) {
		fs >> nd1 >> nd2 >> nd3;
		Cells.push_back({ nd1, nd2, nd3 });
	}
	
	if (!fs.eof()) {
		unsigned nCell2, nd4;
		fs >> nCell2;
		if (!fs.eof()) {
			nCells += nCell2;
			for (unsigned nc = 0; nc < nCell2; ++nc) {
				fs >> nd1 >> nd2 >> nd3 >> nd4;
				Cells.push_back({ nd1, nd2, nd3, nd4 });
			}
		}
	}
	fs.close();


	// Open Node file which includes coordinates of each node
	
	std::cout << " Input Nodes file name: ";
	std::cin >> fName;
	while (true) {
		fs.open(fName);
		if (!fs.good())
			std::cout << 
			"\n Could NOT open file to read ...\n" <<
			"press (c) to end or enter a new file name ...\n";
		else { cout << " \nopening file succesfully\n\n" << endl;  break; }
		std::cin >> fName;
		if (fName.compare(string("c")) == 0) { cout << "Finished!" << endl; return false; }
	}

	// Now read coordinates of each Node
	unsigned nNodes;
	double ndx, ndy;
	fs >> nNodes;
	for (unsigned nn = 0; nn < nNodes; ++nn) {
		fs >> ndx >> ndy;
		Nodes.push_back({ ndx, ndy });
	}
	fs.close();

	nCell_ = nCells;
	nNode_ = nNodes;

	setMinMaxCells();

	return true;
}
