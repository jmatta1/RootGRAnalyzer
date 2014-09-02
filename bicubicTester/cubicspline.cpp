#include"cubicspline.h"

CubicSpline::CubicSpline(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt):
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

void CubicSpline::init(float* xPtArr, float* yPtArr, int numPts, float yp1, float ypn, BndType bt)
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

void CubicSpline::setupSpline(float yp1, float ypn, BndType bt)
{//algorithm taken from numerical recipes in C
	float p, qn, sig, un;
	float* y1Arr;
	//allocate the temporary first derivative array
	y1Arr = new float[this->size];
	
	//setup the lower boundary edge
	if ( (bt == LowNatural) || (bt == BothNatural) )
	{//set this edge as a natural boundary
		y2Arr[0] = 0.0;
		y1Arr[0] = 0.0;
	}
	else
	{//fix the first derivative
		y2Arr[0] = -0.5;
		y1Arr[0] = ( (3.0/(xArr[1]-xArr[0]))*((yArr[1]-yArr[0])/(xArr[1]-xArr[0])-yp1));
	}
	
	int last = (this->size)-1;

	//this is the decomposition loop of the tridiagonal algorithm
	for( int i=1; i<last; ++i)
	{
		sig = (xArr[i] - xArr[i-1])/(xArr[i+1] - xArr[i-1]);
		p = sig*y2Arr[i-1]+2.0;
		y2Arr[i]=(sig-1.0)/p;
		float term1 = ((yArr[i+1]-yArr[i])/(xArr[i+1]-xArr[i]));
		float term2 = ((yArr[i]-yArr[i-1])/(xArr[i]-xArr[i-1]));
		float temp = (term1-term2);
		term1 = ( (6.0*temp) / (xArr[i+1]-xArr[i-1]) );
		term2 = (sig*y1Arr[i-1]);
		y1Arr[i] = ( ( term1 - term2 )/p);
	}

	//setup the upper boundary edge
	if ( (bt == HighNatural) || (bt == BothNatural) )
	{//set this edge as a natural boundary
		qn = 0.0;
		un = 0.0;
	}
	else
	{//fix the first derivative
		qn = -0.5;
		un = ( (3.0/(xArr[last]-xArr[last-1]))*(ypn-(yArr[last]-yArr[last-1])/(xArr[last]-xArr[last-1])) );
	}
	y2Arr[last] = ((un-(qn*y1Arr[last-1]))/((qn*y2Arr[last-1])+1.0));
	
	
	//this is the back substitution loop of the tridiagonal algorithm
	for(int i=(last-1); i>=0; --i)
	{
		y2Arr[i] = y2Arr[i]*y2Arr[i+1]+y1Arr[i];
	}

	//free the temporarily assigned memory
	delete[] y1Arr;
}

float CubicSpline::operator()(float x)
{
	if(xArr == NULL)
	{
		return std::numeric_limits<float>::quiet_NaN();
	}
	
	int klo, khi, k;
	float output, h, b, a;
	
	klo = 0;
	khi = (this->size)-1;
	while ((khi-klo)>1)
	{
		k=(khi+klo)>>1;
		if(xArr[k] > x)
		{
			khi=k;
		}
		else
		{
			klo=k;
		}
	}//klow and khi now bracket the input value for x in the array of xvalues
	h = xArr[khi]-xArr[klo];
	//technically we should make certain that h!=0.0 here but whatever
	a = (xArr[khi]-x)/h;
	b = (x-xArr[klo])/h;
	output = ((a*yArr[klo])+(b*yArr[khi])+(((((a*a*a-a)*y2Arr[klo])+((b*b*b-b)*y2Arr[khi]))*(h*h))/6.0));
	return output;
}

float CubicSpline::getVal(float x)
{
	if(xArr == NULL)
	{
		return std::numeric_limits<float>::quiet_NaN();
	}
	
	int klo, khi, k;
	float output, h, b, a;
	
	klo = 0;
	khi = (this->size)-1;
	while ((khi-klo)>1)
	{
		k=(khi+klo)>>1;
		if(xArr[k] > x)
		{
			khi=k;
		}
		else
		{
			klo=k;
		}
	}//klow and khi now bracket the input value for x in the array of xvalues
	h = xArr[khi]-xArr[klo];
	//technically we should make certain that h!=0.0 here but whatever
	a = (xArr[khi]-x)/h;
	b = (x-xArr[klo])/h;
	output = ((a*yArr[klo])+(b*yArr[khi])+(((((a*a*a-a)*y2Arr[klo])+((b*b*b-b)*y2Arr[khi]))*(h*h))/6.0));
	return output;
}

