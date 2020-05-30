#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <iomanip>
#include <math.h>
#include "Graph.h"
#include "WGraph.h"
#include "CRS.h"
#include "CellNodeFE.h"

using namespace std;

enum privateData {CELLS, NODES, N2C, CELLNEIGHB, NODENEIGHB};

int main() {

	cout << "\n This code is to answer programming tasks #1 and #2 presented in check_programming.pdf\n";

	// ********* TASK #1
	cout << "\n *** Task #1 *** \n";
	cout << "\n Please note that we considered unstructred grid (rectangles and triangles together),\n";
	cout << " so section 1.c is already implemented. Actually you can input 3x3_elements_2d_unstructured.txt and 3x3_coords_2d_unstructured.txt\n";
	cout << "\n In the first section we read (and can print) Element and Coordinates files. press a key to continue ...\n";
	getchar();

	CellNodeFE FE;

	if(!FE.readFiles_setNodesCells()) return 0;

	char yesorno;

	cout << "print cells and nodes? yes(y) or no(n)\n";
	cin >> yesorno;

	if (toupper(yesorno) == 'Y')
	{
		FE.printData(CELLS);
		FE.printData(NODES);
	}

	cout << "\n\n Then we construct data structure (and can print) needed in Section 1.a.\n";

	FE.setNodeToCell();

	cout << "\n print Node->Cell? yes(y) or no(n)\n";
	cin >> yesorno;

	if (toupper(yesorno) == 'Y')
		FE.printData(N2C);

	FE.findNeigbourCells();

	cout << "\n print cell->cell? yes(y) or no(n)\n";
	cin >> yesorno;

	if (toupper(yesorno) == 'Y')
		FE.printData(CELLNEIGHB);

	FE.findNeigbourNodes();

	cout << "\n print node->node? yes(y) or no(n)\n";
	cin >> yesorno;

	if (toupper(yesorno) == 'Y')
		FE.printData(NODENEIGHB);
	

	cout << endl << endl;
	cout << " To answer 1.b which finds the nearset cell to a coordinate, either inside or outside of the domain\n";
	cout << " If the coordinate lie inside the domain the distance would be zero, otherwise the nearest distance will be printed, as well\n";

	while (true) {
		vector<double> p(2);
		double R(0);
		size_t indxNear;
		cout << "\n Enter (coordinates) px and py" << endl;
		cin >> p[0] >> p[1];
		indxNear = FE.findNearestCell(p, R);
		cout << " Nearset Cell index : " << indxNear << " with distance : "
			<< R << " \n\n Another coordinate? yes(y) No(n)";
		
		cin >> yesorno;
		if (toupper(yesorno) == 'N') { getchar(); break; }
	}


// ***** TASK #2
	cout << "\n *** Task #2 *** \n";
	cout << "\n To address task #2 we construct and object of Graph with the Node -> Node from task #1 and can print it\n";
	

	Graph digraph(FE.getNodeNeigbour());
	
	cout << "\n print DiGraph? yes(y) or no(n)\n";
	cin >> yesorno;
	if (toupper(yesorno) == 'Y')
		digraph.Show();


	cout << "\n For 2.a we construct a weighted (inverse of squared distances) graph and can print it";
	cout << "\n Note that for diagonal elements (an edge with the same endpoints) we set 10 as weight.\n";
	

	WGraph wgraph(FE.getNodeNeigbour(), FE.getNodeCoordinate());

	cout << "\n print WGraph? yes(y) or no(n)\n";
	cin >> yesorno;
	if (toupper(yesorno) == 'Y')
		wgraph.Show();


	cout << "\n\n For 2.b we convert wgraph in 2.a to a CRS Matrix";
	CRSMat A(wgraph);


	cout << "\n We can send this matrix to matlab : CRS2Matlab.txt is an -ascii file that\n" <<
		" should be loaded in matlab with readFromCpp.m \n Writing CRS2Matlab.txt ...\n";


	A.write2matlab("CRS2Matlab.txt");
	cout << " Writing is done\n\n";



	cout << "Then we calculate A*ones and write it in CRS_MatVecOnes.txt (result can be checked in matlab)\n\n" <<
	"press a key ...\n";
	cin.ignore();

	vector<double> x(A.getSize(),1.0), y;
	A.MatVec(x, y);

	ofstream fs;
	fs.open("CRS_MatVecOnes.txt");
	if (!fs.good())
	{
		cout << "\nCould NOT open file to write. Press a key to end... " << endl; getchar();  return 0;
	}

	for (size_t i = 0; i < y.size(); ++i) fs << " " << setprecision(16) << scientific << y[i] << endl;
	fs.close();



	cout << "\n\n Then we calculate A*[1;2;3;...] and write it in CRS_MatVec1234.txt (result can be checked in matlab)" << 
	" Press a key ...\n\n";
	cin.ignore();
	
	for (size_t i = 0; i < x.size(); ++i) x[i] = double(i) + 1.0;
	A.MatVec(x, y);

	fs.open("CRS_MatVec1234.txt");
	if (!fs.good())
	{
		cout << "\nCould NOT open file to write. Press a key to end... " << endl; getchar();  return 0;
	}
	for (size_t i = 0; i < y.size(); ++i) fs << " " << scientific << y[i] << endl;
	fs.close();




	cout << "\n\n Finally we read a matrix from matlab. Use write2cpp.m script and copy\n" <<
		" Matlab2CRS.txt in current folder (wherever you found CRS2Matlab.txt)\n" <<
		" press a key when you copied file ...\n";
	cin.ignore();
	

	A.readMatlab("Matlab2CRS.txt");
	cout << " Reading is done\n";



	cout << "\n Then we calculate A*ones and write it in CRSfromMatlab_MatVecOnes.txt (result can be checked in matlab)\n\n";
	cout << "press a key ...\n";
	cin.ignore();

	x.resize(A.getSize());
	for (size_t i = 0; i < x.size(); ++i) x[i] = 1.0;
	A.MatVec(x, y);
	
	
	fs.open("CRSfromMatlab_MatVecOnes.txt");
	if (!fs.good())
	{
		cout << "\nCould NOT open file to write. Press a key to end... " << endl; getchar();  return 0;
	}
	for (size_t i = 0; i < y.size(); ++i) fs << " " << scientific << y[i] << endl;
	fs.close();



	cout << "\n\n Then we calculate A*[1;2;3;4;...] and write it in CRSfromMatlab_MatVec1234.txt (result can be checked in matlab)\n\n";
	cout << "press a key ...\n";
	cin.ignore();

	for (size_t i = 0; i < x.size(); ++i) x[i] = double(i) + 1.0;
	A.MatVec(x, y);

	fs.open("CRSfromMatlab_MatVec1234.txt");
	if (!fs.good())
	{
		cout << "\nCould NOT open file to write. Press a key to end... " << endl; getchar();  return 0;
	}
	for (size_t i = 0; i < y.size(); ++i) fs << " " << scientific << y[i] << endl;
	fs.close();



	cout << endl << "\n\n End of main. Press a key to end ..." << endl;
	getchar();
	return 0;
}
