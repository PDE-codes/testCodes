#include <fstream>
#include <iomanip>
#include <iostream>
#include "CRS.h"  

using namespace std;

/*
	Input argument "val" is the matrix entry (i,j) or 
	the weight of the edge with vertices (i,j)
*/

CRSMat::CRSMat(WGraph &wgrapg) : I_(0), J_(0), V_(0)
{
	I_.clear();
	J_.clear();
	V_.clear();

	unsigned iv(0);
	I_.push_back(iv);

	auto itj(wgrapg.getEdges().begin()), itjEnd(wgrapg.getEdges().end());
	auto itw = wgrapg.getWeigh().begin();

	unsigned rowp(itj->vhead), row(0);

	for (; itj != itjEnd; ++itj, ++itw)
	{
		row = itj->vhead;
		++iv;
		J_.push_back(itj->vtail);
		V_.push_back(*itw);
		if (row != rowp) { I_.push_back(iv - 1); rowp = row; }
	}
	if (row == rowp) I_.push_back(iv);
}


/*
	A general constructor that sets I_,J_,V_ of the CRS matrix, independent of the Graph
*/

CRSMat::CRSMat(vector<vector<unsigned>>& NodeNeighb, vector<double>& val) 
	: WGraph(NodeNeighb, val),  // Base class constructor
 	  I_(0), J_(0), V_(0)
	  
{
	I_.clear();
	J_.clear();
	V_.clear();
	
	unsigned iv(0), cv(0);
	I_.push_back(iv);

	for (unsigned head = 0; head < NodeNeighb.size(); ++head) {
		for (auto tailIt = NodeNeighb[head].begin(); tailIt != NodeNeighb[head].end(); ++tailIt)
		{
			J_.push_back(*tailIt);
			V_.push_back(val[cv]);
			++cv;
		}
		iv += NodeNeighb[head].size();
		I_.push_back(iv);
	}
}



// Calculates Matrix-Vector product
void CRSMat::MatVec(vector<double>& x, vector<double>& y) // y = A*x
{
	y.resize(x.size());
	
	size_t n(I_.size() - 1), cv(0);
	for (size_t i = 0; i < n; ++i)
	{
		y[i] = 0;
		for (size_t j = I_[i]; j < I_[i + 1]; ++j)
		{
			y[i] += V_[cv] * x[J_[cv]];
			++cv;
		}
	}
}





// Read a sparse matrix which is the output of script write2cpp.m

void CRSMat::readMatlab(string fName)
{
	ifstream fs;
	fs.open(fName);
	if (!fs.good())
	{
		cout << "\n Could NOT open file to read ... " << endl; return;
	}

	I_.clear();
	J_.clear();
	V_.clear();

	unsigned iv(0), rowp(1), row(0), col, nline;
	double val;
	I_.push_back(0);
	fs >> nline;
	for (size_t cnn = 0; cnn < nline; ++cnn) {
		fs >> row >> col >> val;
		++iv;
		if (row - rowp > 1) { cout << " A row is entirely zero....\n Will be end ...\n" << endl; return; }
		if (row != rowp) { I_.push_back(iv - 1); rowp = row; }
		J_.push_back(col - 1);
		V_.push_back(val);
	}
	if(row == rowp) I_.push_back(iv);
}



/* 
	Write a matrix to a file and "spconvert" in matlab converts it to a matrix.
	"readFromCpp.m" does this task.
*/

void CRSMat::write2matlab(string fName)
{
	ofstream fs;
	fs.open(fName);
	if (!fs.good())
	{
		cout << "\n Could NOT open file to write ... " << endl; return;
	}

	size_t n(I_.size() - 1), cv(0);
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = I_[i]; j < I_[i + 1]; ++j)
		{
			fs << i + 1 << "\t" << J_[j] + 1 << "\t" << setprecision(16) << scientific << V_[cv] << endl;
			++cv;
		}
	}
	fs.close();
}


void CRSMat::Show() {} // Just to silence clang++
