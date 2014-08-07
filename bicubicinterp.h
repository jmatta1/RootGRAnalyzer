#include "cubicspline.h"

//produces a two dimensional interpolation on a regular grid of points
//the class is given function outputs on the grid of
//X and Y values {x0, x1, ..., x(n-1), xn} and {y0, y1, ..., y(n-1), yn}
//After it has constructed the class can provide interpolation for values satisfying:
// x1 <= X <= x(n-1)  and  y1 <= Y <= y(n-1)
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
	float* xList;//has length numX
	//holds the list of delta x values
	float* dX;//has length numX but index 0 is set to 0.0;
	//holds the inverses of delta x values
	float* idX;//has length numX but index 0 is set to 0.0;
	//holds the denominators for the a and b terms
	float* denoms;//has length regions but index 0 and (regions-1) are set to 0.0;
	//holds the multiplicative part of the 1st term of the a numerator
	float* aTerm1;//has length regions but index 0 and (regions-1) are set to 0.0;
	//holds the multiplicative part of the 2nd term of the a numerator
	float* aTerm2;//has length regions but index 0 and (regions-1) are set to 0.0;
	//holds the multiplicative part of the 3rd term of the a numerator
	float* aTerm3;//has length regions but index 0 and (regions-1) are set to 0.0;
	//holds the multiplicative part of the 1st term of the b numerator
	float* bTerm1;//has length regions but index 0 and (regions-1) are set to 0.0;
	//holds the multiplicative part of the 2nd term of the b numerator
	float* bTerm2;//has length regions but index 0 and (regions-1) are set to 0.0;
	//holds the multiplicative part of the 3rd term of the b numerator
	float* bTerm3;//has length regions but index 0 and (regions-1) are set to 0.0;
	//holds the interpolations in the y direction
	CubicSpline* ySplines;//has length numX
	

};

BiCubicInterpolation::BiCubicInterpolation(float* xVals, int numXPts, float* yVals, int numYPts, float* zValGrid):
numX(numXPts), regions(numXPts-1)
{
	//set up the list of xValues
	xList = new float[numX];
	for(int i=0; i<numX; ++i)
	{
		xList[i] = xVals[i];
	}
	
	dX = new float[numX];
	dX[0]=0.0;
	for(int i=1; i<numX; ++i)
	{
		dX[i] = (xVals[i]-xVals[i-1]);
	}
	
	idX = new float[numX];
	idX[0]=0.0;
	for(int i=1; i<numX; ++i)
	{
		idX[i] = 1/(dX[i]);
	}
	
	int stop = (regions - 1);
	denoms = new float[regions];
	denoms[0]=0.0;
	denoms[stop]=0;
	
	aTerm1 = new float[regions];
	aTerm2 = new float[regions];
	aTerm3 = new float[regions];
	bTerm1 = new float[regions];
	bTerm2 = new float[regions];
	bTerm3 = new float[regions];
	aTerm1[0]=0.0;
	aTerm2[0]=0.0;
	aTerm3[0]=0.0;
	bTerm1[0]=0.0;
	bTerm2[0]=0.0;
	bTerm3[0]=0.0;
	aTerm1[stop]=0.0;
	aTerm2[stop]=0.0;
	aTerm3[stop]=0.0;
	bTerm1[stop]=0.0;
	bTerm2[stop]=0.0;
	bTerm3[stop]=0.0;
	
	for(int i=1; i<stop; ++i)
	{
		float dx1=dX[i];
		float dx2=dX[i+1];
		float dx3=dX[i+2];
		float temp1 = (4.0*dx1*(dx2+dx3));
		float temp2 = (dx2*(3.0*dx2+4.0*dx3));
		denoms[i] = (dx1*dx3*(temp1+temp2));
		aTerm1[i] = (dx2*dx2*dx3*(3.0*dx2+4.0*dx3));
		aTerm2[i] = (dx1*dx2*dx3*(2.0*dx1+3.0*dx2-4.0*dx3));
		aTerm3[i] = (-2.0*dx1*dx1*dx2*dx2);
		bTerm1[i] = (2.0*dx2*dx2*dx3*dx3);
		bTerm2[i] = (dx1*dx2*dx3*(4.0*dx1+3.0*dx2-2.0*dx3));
		bTerm3[i] = (-1.0*dx1*dx1*dx2*dx2);
	}
	//so to get a and b you do:
	//a[i] = ((aTerm1[i]*dy1+aTerm2[i]*dy2+aTerm3[i]*dy3)/denoms[i])
	//b[i] = ((bTerm1[i]*dy1+bTerm2[i]*dy2+bTerm3[i]*dy3)/denoms[i])
	//where dyn = yn - y(n-1) and the yi are obtained from the splines
	
	ySplines = new CubicSpline[numX];
	for(int i=0; i < numX; ++i)
	{
		ySplines[i].init(yVals, (zValGrid+(i*numYPts)), numYPts, 0.0, 0.0, BothNatural);
	}
}

BiCubicInterpolation::~BiCubicInterpolation()
{
	delete[] xList;	
	delete[] dX;
	delete[] idX;
	delete[] denoms;
	delete[] aTerm1;
	delete[] aTerm2;
	delete[] aTerm3;
	delete[] bTerm1;
	delete[] bTerm2;
	delete[] bTerm3;
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
	float y0 = ySplines[lo-1].getVal(y);
	float y1 = ySplines[lo].getVal(y);
	float y2 = ySplines[hi].getVal(y);
	float y3 = ySplines[hi+1].getVal(y);
	float dy1=(y1-y0);
	float dy2=(y2-y1);
	float dy3=(y3-y2);
	float a = ((aTerm1[lo]*dy1+aTerm2[lo]*dy2+aTerm3[lo]*dy3)/denoms[lo]);
	float b = ((bTerm1[lo]*dy1+bTerm2[lo]*dy2+bTerm3[lo]*dy3)/denoms[lo]);
	float t = (x-xList[lo])*idX[hi];
	float mt = (1.0-t);
	float temp1 = (mt*y1+t*y2);
	float temp2 = (t*mt*(a*mt+b*t));
	return (temp1+temp2);
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
	float y0 = ySplines[lo-1].getVal(y);
	float y1 = ySplines[lo].getVal(y);
	float y2 = ySplines[hi].getVal(y);
	float y3 = ySplines[hi+1].getVal(y);
	float dy1=(y1-y0);
	float dy2=(y2-y1);
	float dy3=(y3-y2);
	float a = ((aTerm1[lo]*dy1+aTerm2[lo]*dy2+aTerm3[lo]*dy3)/denoms[lo]);
	float b = ((bTerm1[lo]*dy1+bTerm2[lo]*dy2+bTerm3[lo]*dy3)/denoms[lo]);
	float t = (x-xList[lo])*idX[hi];
	float mt = (1.0-t);
	float temp1 = (mt*y1+t*y2);
	float temp2 = (t*mt*(a*mt+b*t));
	return (temp1+temp2);
}

