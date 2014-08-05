#ifndef CUBICSPLINE_H
#define CUBICSPLINE_H

enum BndType{NeitherNatural, LowNatural, HighNatural, BothNatural};

#include<cstdlib>

class CubicSpline
{
public:

	inline CubicSpline(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt);
	CubicSpline():size(0), xArr(NULL), yArr(NULL), y2Arr(NULL){}
	~CubicSpline(){delete[] xArr; delete[] yArr; delete[] y2Arr;}

	inline void init(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt);

	float operator()(float x);

private:
	void setupSpline(float yp1, float ypn, BndType bt);
	int size;
	float* xArr;
	float* yArr;
	float* y2Arr;

};

inline void CubicSpline::init(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt)
{
	size = numPts;
	xArr = new float[numPts];
	yArr = new float[numPts];
	y2Arr = new float[numPts];
	
	for(int i=0; i<numPts; ++i)
	{
		xArr[i]=xPtArr[i];
		yArr[i]=yPtArr[i];
		y2Arr[i]=0.0;
	}
	
	this->setupSpline(yp1, ypn, bt);
}

inline CubicSpline::CubicSpline(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt):
	size(numPts), xArr( new float[numPts] ), yArr( new float[numPts] ), y2Arr( new float[numPts] )
{
	for(int i=0; i<size; ++i)
	{
		xArr[i]=xPtArr[i];
		yArr[i]=yPtArr[i];
		y2Arr[i]=0.0;
	}
	this->setupSpline(yp1, ypn, bt);
}

#endif

