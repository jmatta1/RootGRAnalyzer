//#include "cubicspline.h"
//produces a two dimensional interpolation on a regular grid of points
//the class is given function outputs on the grid of
//X and Y values {x0, x1, ..., x(n-1), xn} and {y0, y1, ..., y(n-1), yn}
//After it has constructed the class can provide interpolation for values satisfying:
// x1 <= X <= x(n-1)  and  y1 <= Y <= y(n-1)
#include"Math/Interpolator.h"
class BiCubicInterpolation
{
public:
	//takes an array of x values, an array of y values and and array of z values, as well as
	//the sizes of the arrays of x and y values, the size of the z value array must be numXPts*numYPts,
	//given that for Xi and Yj you have Zij then the z value array must be arranged as follows
	// Z11 Z12 Z13 ...
	// Z21 Z22 Z23 ...
	// Z31 Z32 Z33 ...
	// ... ... ... ...
	BiCubicInterpolation(float* xVals, int numXPts, float* yVals, int numYPts, float* zValGrid);
	~BiCubicInterpolation();
	
	float operator()(float x, float y);
	float getVal(float x, float y);

private:
	//holds the number of points
	int numX;
	//holds the number of interpolation retions
	int regions;
	//holds the list of x-values
	double* xList;//has length numX
	double yArr[4];
	ROOT::Math::Interpolator* overall;
	ROOT::Math::Interpolator* ySplines;//has length numX
	

};

BiCubicInterpolation::BiCubicInterpolation(float* xVals, int numXPts, float* yVals, int numYPts, float* zValGrid):
numX(numXPts), regions(numXPts-1)
{
	//set up the list of xValues
	xList = new double[numX];
	for(int i=0; i<numX; ++i)
	{
		xList[i] = xVals[i];
	}
	
	double* yValues = new double[numYPts];
	double* zValues = new double[numYPts];
	
	for(int i=0; i<numYPts; ++i)
	{
		yValues[i] = yVals[i];
	}
	
	overall = new ROOT::Math::Interpolator(4);
	
	ySplines = new ROOT::Math::Interpolator[numX];
	for(int i=0; i < numX; ++i)
	{
		for(int j=0; j<numYPts; ++j)
		{
			zValues[j] = zValGrid[ i*numYPts + j ];
		}
		ySplines[i].SetData(numYPts, yValues, zValues);
	}
	delete[] yValues;
	delete[] zValues;
}

BiCubicInterpolation::~BiCubicInterpolation()
{
	delete[] xList;	
	delete overall;
	delete[] ySplines;
}

float BiCubicInterpolation::operator()(float x, float y)
{
	//first find the indices of the x values that that bracket x
	int lo, hi, ind;
	
	lo = 0;
	hi = regions;//because regions is always numX-1
	while ((hi-lo)>1)
	{
		ind=((hi+lo)>>1);
		if(xList[ind] > x)
		{
			hi=ind;
		}
		else
		{
			lo=ind;
		}
	}
	//assume we are in the interpolation region and do the calculation
	yArr[0] = ySplines[lo-1].Eval(y);
	yArr[1] = ySplines[lo].Eval(y);
	yArr[2] = ySplines[hi].Eval(y);
	yArr[3] = ySplines[hi+1].Eval(y);
	overall->SetData(4,&(xList[lo-1]),yArr);
	return overall->Eval(x);
}

float BiCubicInterpolation::getVal(float x, float y)
{
	//first find the indices of the x values that that bracket x
	int lo, hi, ind;
	
	lo = 0;
	hi = regions;//because regions is always numX-1
	while ((hi-lo)>1)
	{
		ind=((hi+lo)>>1);
		if(xList[ind] > x)
		{
			hi=ind;
		}
		else
		{
			lo=ind;
		}
	}
	//assume we are in the interpolation region and do the calculation
	yArr[0] = ySplines[lo-1].Eval(y);
	yArr[1] = ySplines[lo].Eval(y);
	yArr[2] = ySplines[hi].Eval(y);
	yArr[3] = ySplines[hi+1].Eval(y);
	overall->SetData(4,&(xList[lo-1]),yArr);
	return overall->Eval(x);
}

