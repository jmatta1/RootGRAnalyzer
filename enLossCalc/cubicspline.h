#ifndef CUBICSPLINE_H
#define CUBICSPLINE_H

//this code was lifted, with a little adaptation, from Numerical Recipes
enum BndType{NeitherNatural, LowNatural, HighNatural, BothNatural};

#include<cstdlib>
#include<limits>

class CubicSpline
{
public:

	CubicSpline(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt);
	CubicSpline():size(0), xArr(NULL), yArr(NULL), y2Arr(NULL){}
	~CubicSpline(){delete[] xArr; delete[] yArr; delete[] y2Arr;}

	void init(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt);

	float operator()(float x);
	float getVal(float x);

private:
	void setupSpline(float yp1, float ypn, BndType bt);
	int size;
	float* xArr;
	float* yArr;
	float* y2Arr;

};

#endif

