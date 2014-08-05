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

