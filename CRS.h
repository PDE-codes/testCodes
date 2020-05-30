/*
	CRS matrix class derived from Graph (although it is a weighted graph)
*/

#ifndef CRS_H
#define CRS_H

#include "WGraph.h"

class CRSMat : public WGraph {

public:
	CRSMat(std::vector<std::vector<unsigned>>& NodeNeighb, std::vector<double>& val);
	explicit CRSMat(WGraph &wgrapg);
	void MatVec(std::vector<double>& x, std::vector<double>& y); // y = A*x
	void write2matlab(std::string fname);
	void   readMatlab(std::string fname);
	size_t getSize() { return I_.size() - 1; }
	void Show(); // Just to silence clang++
	~CRSMat() { V_.clear(); J_.clear(); I_.clear(); }


private:
	std::vector<unsigned> I_, J_;
	std::vector<double> V_;
	

};


#endif // ! CRS_H

