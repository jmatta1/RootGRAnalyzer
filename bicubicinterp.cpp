#include"bicubicinterp.h"

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
	float y0 = ySplines[lo-1](y);
	float y1 = ySplines[lo](y);
	float y2 = ySplines[hi](y);
	float y3 = ySplines[hi+1](y);
	float dy1=(y1-y0);
	float dy2=(y1-y0);
	float dy3=(y1-y0);
	float a = ((aTerm1[lo]*dy1+aTerm2[lo]*dy2+aTerm3[lo]*dy3)/denoms[lo]);
	float b = ((bTerm1[lo]*dy1+bTerm2[lo]*dy2+bTerm3[lo]*dy3)/denoms[lo]);
	float t = (x-xList[lo])*idX[hi];
	float mt = (1.0-t);
	float temp1 = (mt*y1+t*y2);
	float temp2 = (t*mt*(a*mt+b*t));
	return (temp1+temp2);
}

